#include "bulkmt.hpp"

#include <iostream>
#include <thread>
#include <fstream>
#include <atomic>
#include <mutex>
#include <chrono>

std::atomic<bool> block_ready(false);
std::atomic<bool> stop(false);
std::atomic<bool> log_done(false);
std::atomic<bool> file_done(false);

void log_func(Bulk& bulk) {
	int command_count = 0;
	int block_count = 0;
	while(!stop) {
		if(block_ready && !log_done) {
			bulk.print_to_log();
			log_done = true;
			++block_count;
			command_count+=bulk.count();
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	std::cout<<"log: "<<block_count<<" blocks, "<<command_count<<" commands."<<std::endl;
}

std::mutex file_mutex;

void file_func(int i, Bulk& bulk) {
	int command_count = 0;
	int block_count = 0;
	while(!stop) {
		std::lock_guard<std::mutex> guard{file_mutex};
		if(block_ready && !file_done) {
			bulk.print_to_file();
			file_done = true;
			++block_count;
			command_count+=bulk.count();
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	std::cout<<"file"<<i<<": "<<block_count<<" blocks, "<<command_count<<" commands."<<std::endl;
}

int block_count = 0;

void process(Bulk& bulk) {
	if(bulk.count()>0) {
		log_done = false;
		file_done = false;
		block_ready = true;
		while( !(log_done && file_done) )
			std::this_thread::sleep_for(std::chrono::milliseconds(1));

		bulk.clear();
		++block_count;
	}
}

int main(int argc, char **argv) {
	size_t N = 3;
	if(argc>=2)
		N = atoi(argv[1]);

	int line_count = 0;
	int command_count = 0;
	
	Bulk bulk;
	
	std::thread log(log_func, std::ref(bulk));
	std::thread file1(file_func, 1, std::ref(bulk));
	std::thread file2(file_func, 2, std::ref(bulk));

	int level = 0;
	
	std::string line;
	while(getline(std::cin, line)) {
		++line_count;
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
			++command_count;
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

	stop = true;
	log.join();
	file1.join();
	file2.join();

	return 0;
}
