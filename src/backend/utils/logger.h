#include <fstream>

static void log_string(const std::string &str) {
    std::ofstream out;
    out.open("log.txt");
    out << str.c_str() << std::endl;

    out.close();
}

