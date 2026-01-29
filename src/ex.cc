#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <string>

#define vec std::vector
#define str std::string

int
ex(const vec<str>& args)
{
  pid_t pid = fork();

  if (pid == 0) {  /* CHILD PROCESS */
    /* vec<str> -> char *argv[] */
    std::vector<char*> argv;
    for (const auto& arg : args)
      argv.push_back(const_cast<char*>(arg.c_str()));
    argv.push_back(nullptr);  // NULL-terminated

    /* RUN COMMAND */
    execvp(argv[0], argv.data());

    /* ERROR OUTPUT */
    std::cerr << "Failed to execute: " << argv[0] << std::endl;
    _exit(EXIT_FAILURE);
  } else if (pid > 0) {  /* PARRENT PROCESS */
    int status;
    waitpid(pid, &status, 0);
  } else {
    std::cerr << "Fork failed!" << std::endl;
  }

  return 0;
}

