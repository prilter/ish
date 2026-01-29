/* INCLUDES */
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <string>

/* MACROSES */
#define vec std::vector
#define str std::string

/* EXECUTE */
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

/* HISTORY */
#include <readline/history.h>
int
history(const char *histdir, size_t n=1000)
{
  /* INIT */
  HIST_ENTRY** hist = history_list();
  size_t histlen = 0;

  if (!hist) { std::cout << "No history\n"; return 0; } /* NO HISTORY */
  for (;hist[histlen];) histlen++; /* COUNT LENGTH */
  if (histlen < n) n = histlen; /* CHECK n */

  /* OUTPUT */
  for (size_t i = histlen - n; n--; ++i)
    std::cout << i + 1 << "\t" << hist[i]->line << '\n';

  /* END */
  return 1;
}
