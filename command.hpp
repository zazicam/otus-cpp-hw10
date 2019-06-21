#pragma once

#include <vector>
#include <ostream>

#include "observer.hpp"

class Command: public Observer {
	std::string name;
public:
	explicit Command(std::string& name);
	virtual void handle(Observable& subject, std::ostream& output) override;
	virtual ~Command() override;
};
