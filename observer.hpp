#pragma once

class Observable;

class Observer {
public:
	virtual void handle(Observable& subject, std::ostream& output) = 0;
	virtual ~Observer() {}
};
