#include "bulkmt.hpp"

#include <iostream>
#include <thread>
#include <fstream>

void log_func(Bulk& bulk) {
	
}

void file_func(int i, Bulk& bulk) {

}

void process(Bulk& bulk) {
	if(bulk.count()==0) return;

	std::cout<<"bulk: ";
	bulk.set_output(std::cout); // prepare empty output 
	bulk.notify();

	bulk.clear(); // clear processed observers
}

int main(int argc, char **argv) {
	int N = 3;
	if(argc>=2)
		N = atoi(argv[1]);

	int line_count = 0;
	int command_count = 0;
	int block_count = 0;
	
	Bulk bulk;
	
	std::thread log(log_func, std::ref(bulk));
	std::thread file1(file_func, 1, std::ref(bulk));
	std::thread file2(file_func, 2, std::ref(bulk));

	int level = 0;
	
	std::string line;
	while(getline(std::cin, line)) {
		if(line=="{") {
			if(level==0)
				process(bulk);
			++level;
		}
		else if(line=="}") {
			if(level>0)
				--level;
			if(level==0)
				process(bulk);
		}
		else {
			bulk.add(new Command(line));
			if(level==0 && bulk.count()==N)
				process(bulk);
		}
	}

	if(level==0)
		process(bulk);
	else
		bulk.clear();

	std::cout<<"main: "<<line_count<<" lines, "<<command_count<<" commands, "<<block_count<<" blocks."<<std::endl;

	log.join();
	file1.join();
	file2.join();

	return 0;
}
