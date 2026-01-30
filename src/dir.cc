#include <filesystem>
#include <string>

using str = std::string;

/* GET CURRENT WORK DIRECTORY */
str getcwd(void) { return std::filesystem::current_path().string(); }

/* GET HOME */
#include <cstdlib>
const char *gethm(void) {
    const char *home = std::getenv("HOME");
    return (home ? home:"~");
}

/* CD */
#include <iostream>
int cd(const str newp) { 
  /* IS DIRECTORY */
  if (!std::filesystem::is_directory(newp)) {
    std::cerr << "\"" << newp << "\" is not a directory\n";
    return 0;
  }

  /* IS EXIST */
  if (!std::filesystem::exists(newp)) {
    std::cerr << "\"" << newp << "\" does not exist\n";
    return 0;
  }

  /* CD */
  std::filesystem::current_path(newp); 
  return 1; 
}
