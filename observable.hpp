#pragma once

#include "observer.hpp"

class Observable {
public:
	virtual void notify(std::ostream& output) = 0;
	virtual void add(Observer *obs) = 0;
	virtual ~Observable() {}
};
