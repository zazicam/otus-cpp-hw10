#include "bulkmt.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

// Class Bulk

Bulk::Bulk() : output(std::cout) {
}

void Bulk::notify(int index) {
	observers[index]->handle(*this);
}

void Bulk::notify()  {
	for(auto obs : observers) {
		obs->handle(*this);
	}
}

void Bulk::add(Observer *obs)  {
	// get time of first command in bulk
	using namespace std::chrono;
	if(count()==0) {
		milliseconds ms = duration_cast< milliseconds > (
			system_clock::now().time_since_epoch()
		);
		time = ms.count();
	}
	observers.push_back(obs);
}

std::ostream& Bulk::get_output() {
	return output;
}

void Bulk::set_output(std::ostream& out) {
	output.tie(&out);
}

bool Bulk::is_last(const Observer* obs) {
	auto last_iter = --observers.end();	
	return (obs == *last_iter);
}

void Bulk::process(std::ostream& out) {

	if(count()==0) return;

	out<<"bulk: ";
	set_output(out);
	notify();

	clear(); // clear processed observers
}

int Bulk::count() {
	return observers.size();
}

void Bulk::clear() {
	for(auto obs : observers)
		delete obs;
	observers.clear();
}

Bulk::~Bulk() { clear(); }
// -------------------------------------------------------------

// Class Command

Command::Command(std::string name) : name(name) {}

void Command::handle(Observable& subject)  {
	Bulk& bulk = dynamic_cast<Bulk&>(subject);
	std::string separator = (bulk.is_last(this))? "\n" : ", ";
	bulk.get_output() << name << separator;
}

Command::~Command()  {}
