#include <sstream>
#include <string>

#include "bulk.hpp"
#include "command.hpp"

// Class Command

Command::Command(std::string& name) : name(name) {}

void Command::handle(Observable& subject, std::ostream& output) {
	Bulk& bulk = dynamic_cast<Bulk&>(subject);
	const std::string separator = (bulk.is_last(this))? "\n" : ", ";
	output << name << separator;
}

Command::~Command()  {}
