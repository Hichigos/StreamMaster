#pragma once

#include <fstream>

namespace utils {
	static void logAndPost(const std::string &str) {
		std::ofstream out;
		std::cout << str << std::endl;
		out.open("log.txt", std::ofstream::out | std::ofstream::app);
		out << str.c_str() << std::endl;

		out.close();
	}

}



