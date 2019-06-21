#pragma once

#include <vector>
#include <ostream>
#include <memory>

class Observable;

class Observer {
public:
	virtual void handle(Observable& subject, std::ostream& output) = 0;
	virtual ~Observer() {}
};
//------------------------------------------------------------

class Observable {
public:
	virtual void notify(std::ostream& output) = 0;
	virtual void add(Observer *obs) = 0;
	virtual ~Observable() {}
};
//------------------------------------------------------------

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

	int count(); 

	void clear();
	~Bulk();
};
//------------------------------------------------------------

class Command: public Observer {
	std::string name;
public:
	Command(std::string name);
	virtual void handle(Observable& subject, std::ostream& output) override;
	virtual ~Command() override;
};
