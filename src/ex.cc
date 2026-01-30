/* INCLUDES */
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <string>
#include <vector>

/* MACROSES */
#define vec std::vector
#define str std::string

/* EXECUTE */
extern vec<vec<str>> split(vec<str> ss, str sep);
int ex(const vec<str> &com) {
  vec<vec<str>> coms = split(com, "|"); /* SPLIT BY PIPES */

  if (coms.empty())
    return 0;

  /* ANALY PIPELINE */
  int comslen = coms.size();
  vec<int> pids;
  int input_fd = STDIN_FILENO;

  for (int i = 0; i < comslen; ++i) {
    int pipefd[2];

    /* MAKE PIPE FOR ALL COMMANDS EXCEPT LAST ONE */
    if (i < comslen - 1) {
      if (pipe(pipefd) == -1) {
        std::cerr << "Pipe failed!" << std::endl;
        return -1;
      }
    }

    pid_t pid = fork();

    if (pid == 0) {  /* CHILD */
      /* INPUT STREAM */
      if (input_fd != STDIN_FILENO) {
        dup2(input_fd, STDIN_FILENO);
        close(input_fd);
      }

      /* OUTPUT STREAM */
      if (i < comslen - 1) {
        close(pipefd[0]); /* CLOSE READING */
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
      }

      /* EXECUTE */
      vec<char*> argv;
      for (const auto& arg : coms[i]) {
        argv.push_back(const_cast<char*>(arg.c_str()));
      }
      argv.push_back(nullptr);

      execvp(argv[0], argv.data());
      std::cerr << "Failed to execute: " << argv[0] << std::endl;
      _exit(EXIT_FAILURE);

    } else if (pid > 0) { /* PARENT */
      pids.push_back(pid);

      if (input_fd != STDIN_FILENO) /* CLOSE OLD input_fd */
        close(input_fd);

      /* CLOSE write end OF PIPE */
      if (i < comslen - 1) {
        close(pipefd[1]);
        input_fd = pipefd[0]; /* NEXT COMMAND START READING FROM HERE */
      }
    } else {
      std::cerr << "Fork failed!" << std::endl;
      return -1;
    }
  }

  /* WAIT ENDING OF PROCESS */
  int last_status = 0;
  for (pid_t pid : pids) {
    int status;
    waitpid(pid, &status, 0);
    last_status = WEXITSTATUS(status);
  }

  return last_status;
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
