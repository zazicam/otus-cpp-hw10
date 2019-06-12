#pragma once

#include <vector>
#include <ostream>

class Observable;

class Observer {
public:
	virtual void handle(Observable& subject) = 0;
	virtual ~Observer() {}
};
//------------------------------------------------------------

class Observable {
public:
	virtual void notify(int i) = 0;
	virtual void notify() = 0;
	virtual void add(Observer *obs) = 0;
	virtual ~Observable() {}
};
//------------------------------------------------------------

class Bulk: public Observable {
	std::vector<Observer*> observers;	
	std::ostream& output;
	int time;

public:
	Bulk();
	void notify(int i) override; 
	void notify() override; 
	void add(Observer *obs) override;

	std::ostream& get_output();
	void set_output(std::ostream& out);

	bool is_last(const Observer* obs);

	void process(std::ostream& out); 
	int count(); 

	void clear();
	~Bulk();
};
//------------------------------------------------------------

class Command: public Observer {
	std::string name;
public:
	Command(std::string name);
	virtual void handle(Observable& subject) override;
	virtual ~Command() override;
};
