#include "bulkmt.hpp"

#include <iostream>
#include <thread>
#include <fstream>
#include <atomic>
#include <chrono>

std::atomic<bool> block_ready(false);

std::atomic<bool> log_ready(true);
std::atomic<bool> file_ready(true);

void log_func(Bulk& bulk) {
	while( !block_ready )
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	bulk.print_to_log();
	log_ready = true;
}

void file_func(Bulk& bulk) {
	while( !block_ready )
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	bulk.print_to_file();
	file_ready = true;
}

void process(Bulk& bulk) {
	while( !(log_ready && file_ready) )
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		
	block_ready = true;
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
	std::thread file(file_func, std::ref(bulk));

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
	file.join();

	return 0;
}
