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

extern str           getcwd(void);
extern str           gethm(void);
extern vec<str>      split(const char *s, char sep);
extern vec<vec<str>> split(vec<str> ss, str sep);
extern str           rep(str s, const char *old, const char *new_);
extern int           ex(const vec<str>&);
extern int           cd(const str);
extern int           history(const char *histdir, size_t n=1000);

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
  /* INIT */
  vec<vec<str>> coms;
  str cwd = getcwd(), clean_keywords, lastdir = ".";
  char *inp;

  /* SETTINGS OF SIGNAL HANDLER */
  struct sigaction sa;
  sa.sa_handler = sigint_handler;
  sa.sa_flags = 0;
  sigemptyset(&sa.sa_mask);
  sigaction(SIGINT, &sa, nullptr);

  read_history(HISTDIR);
  for (;;cwd = getcwd()) {
    /* GET INPUT */
    inp = readline(CONV);
    if (!inp) {std::cout << "\n"; break;}  /* Ctrl+D */
    if (*inp) {add_history(inp);}          /* HISTORY */

    /* GET CLEAN COMMAND */
    clean_keywords = inp; free(inp);
    clean_keywords = rep(clean_keywords, "~", gethm().c_str()); /* ~ -> HOME variable */
    clean_keywords = rep(clean_keywords, "-", lastdir.c_str()); /* ~ -> HOME variable */
    clean_keywords = rep(clean_keywords, "; ", " && ");         /* ; -> && */
    clean_keywords = rep(clean_keywords, ";", " && ");          /* ; -> && */

    coms = split(split(clean_keywords.c_str(), ' '), "&&");

    /* RUN COMMANDS */
    for (vec<str> com : coms) {
      /* NO PROMPT */
      if (com.size() == 0) continue;

      /* RUN */
      if      (com[0] == "cd")      {lastdir = cwd; cd((com.size() != 1) ? com[1]:gethm());}
      else if (com[0] == "history") {if (com.size() > 1) history(HISTDIR, atoi(com[1].c_str())); else history(HISTDIR);}
      else if (com[0] == "exit")    {goto end;}
      else                          {ex(com);}
    }
  }

end:
  write_history(HISTDIR);
  return 0;
}
