#pragma once

#include <vector>
#include <ostream>
#include <memory>

#include "observer.hpp"
#include "observable.hpp"

class Bulk: public Observable {
	std::vector<std::shared_ptr<Observer>> observers;
	int id;
	int time;

public:
	Bulk();
	void notify(std::ostream& output) override; 
	void add(Observer *obs) override;

	bool is_last(const Observer* obs);

	void print_to_log(); 
	void print_to_file(); 

	size_t count() const; 

	void clear();
	~Bulk();
};
