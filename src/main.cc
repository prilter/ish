#include <string>
#include <vector>
#include <iostream>

#include <cstdlib>
#include <csignal>

#include <readline/readline.h>
#include <readline/history.h>

#define str std::string
#define vec std::vector
#define out std::cout
#define in  std::cin

extern str         getcwd(void);
extern str         gethm(void);
extern vec<str>    split(char *s, char sep);
extern str         rep(std::string s, const char *old, const char *new_);
extern int         ex(const vec<str>&);
extern int         cd(const str);

#define YELLOW   "\x1b[33m"
#define WHITE    "\x1b[37m"
#define HISTDIR  (gethm() + "/.ish_history").c_str()
#define CONV     (YELLOW + rep(cwd, gethm().c_str(), "~") + WHITE + "❯ ").c_str()

/* FLAG FOR HANDLING C-C */
volatile sig_atomic_t interrupted = 0;
void sigint_handler(int sig) {
  interrupted = 1;

  rl_replace_line("", 0); /* CLEAN CURRENT LINE FOR readline */
  std::cout << "\n"; /* MOVE CURSOR TO NEW LINE */

  /* REDRAW PROMPT */
  rl_on_new_line();
  rl_redisplay();
}

int
main(void)
{
  vec<str> com;
  str cwd, lastdir = ".";
  char *inp;

  /* SETTINGS OF SIGNAL HANDLER */
  struct sigaction sa;
  sa.sa_handler = sigint_handler;
  sa.sa_flags = 0;
  sigemptyset(&sa.sa_mask);
  sigaction(SIGINT, &sa, nullptr);

  for (cwd = getcwd();; cwd = getcwd()) {
    /* READ HISTORY */
    read_history(HISTDIR);

    /* GET INPUT */
    inp = readline(CONV);
    if (!inp) {std::cout << "\n"; break;}  /* Ctrl+D */
    if (*inp) add_history(inp); /* HISTORY */

    /* GET CLEAN COMMAND */
    com = split(inp, ' ');
    free(inp);
    for (str &s : com) s = rep(  rep(s, "~", gethm().c_str())  , "-", lastdir.c_str()); 

    /* NO PROMPT */
    if (com.size() == 0) continue;

    /* CD */
    if (com[0] == "cd")           {lastdir = getcwd(); cd((com.size() != 1) ? com[1]:gethm());}
    else if (com[0] == "exit")    {break;}
    else                          {ex(com);}

    /* SAVE HISTORY */
    write_history(HISTDIR);
  }

  return 0;
}
