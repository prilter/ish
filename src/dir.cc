#include <filesystem>
#include <string>

std::string getcwd(void) { return std::filesystem::current_path().string(); }

#include <cstdlib>
std::string gethm(void) {
    const char *home = std::getenv("HOME");
    return (home ? std::string(home):std::string("~"));
}
