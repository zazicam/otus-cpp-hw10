#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>

#include "bulkmt.hpp"
#include "command.hpp"

// Class Bulk

Bulk::Bulk() : id(0) {
}

void Bulk::notify(std::ostream& output)  {
	for(auto obs : observers) {
		obs->handle(*this, output);
	}
}

void Bulk::add(Observer *obs)  {
	using namespace std::chrono;
	if(count()==0) {  // start new bulk
		++id;
		// get time of first command in the bulk
		milliseconds ms = duration_cast< milliseconds > (
			system_clock::now().time_since_epoch()
		);
		time = ms.count();
	}
	observers.emplace_back(obs);
}

bool Bulk::is_last(const Observer* obs) {
	auto last_iter = --observers.end();	
	return (obs == last_iter->get());
}

void Bulk::print_to_log() {
	if(count()==0) return;
	std::cout<<"bulk: ";
	notify(std::cout);
}

void Bulk::print_to_file() {
	if(count()==0) return;
	std::stringstream filename;
	filename<<"bulk"<<time<<"_"<<id<<".log";
	std::ofstream file(filename.str());
	file<<"bulk: ";
	notify(file);
}

size_t Bulk::count() const {
	return observers.size();
}

void Bulk::clear() {
	observers.clear();
}

Bulk::~Bulk() { clear(); }
// -------------------------------------------------------------

// Class Command

Command::Command(std::string& name) : name(name) {}

void Command::handle(Observable& subject, std::ostream& output) {
	Bulk& bulk = dynamic_cast<Bulk&>(subject);
	const std::string separator = (bulk.is_last(this))? "\n" : ", ";
	output << name << separator;
}

Command::~Command()  {}
