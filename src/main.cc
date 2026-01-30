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
extern const char   *gethm(void);
extern vec<str>      split(const char *s, char sep);
extern vec<vec<str>> split(vec<str> ss, str sep);
extern str           rep(str s, const char *old, const char *new_);
extern int           ex(const vec<str>&);
extern int           cd(const str);
extern int           history(const char *histdir, size_t n=1000);

#define YELLOW   "\x1b[33m"
#define WHITE    "\x1b[37m"
#define HISTDIR  (str(gethm()) + "/.ish_history").c_str()
#define CONV     (YELLOW + rep(cwd, gethm(), "~") + WHITE + "❯ ").c_str()

void sigint_handler(int sig);
int  cleanup_readline(void);
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
    clean_keywords = rep(clean_keywords, "~", gethm()); /* ~ -> HOME variable */
    clean_keywords = rep(clean_keywords, "-", lastdir.c_str()); /* ~ -> HOME variable */
    clean_keywords = rep(clean_keywords, "; ", " && ");         /* ; -> && */
    clean_keywords = rep(clean_keywords, ";", " && ");          /* ; -> && */

    coms = split(split(clean_keywords.c_str(), ' '), "&&");

    /* RUN COMMANDS */
    for (vec<str> com : coms) {
      /* NO PROMPT */
      if (com.size() == 0) continue;

      /* RUN */
      if      (com[0] == "cd")      {lastdir = cwd; cd((com.size() != 1) ? com[1]:str(gethm()));}
      else if (com[0] == "history") {if (com.size() > 1) history(HISTDIR, atoi(com[1].c_str())); else history(HISTDIR);}
      else if (com[0] == "exit")    {goto end;}
      else                          {ex(com);}
    }
  }

end:
  write_history(HISTDIR);
  cleanup_readline();
  return 0;
}



/* FLAG FOR HANDLING C-C */
volatile sig_atomic_t interrupted = 0;
void
sigint_handler(int sig) 
{
  interrupted = 1;

  rl_replace_line("", 0); /* CLEAN CURRENT LINE FOR readline */
  std::cout << "\n"; /* MOVE CURSOR TO NEW LINE */

  /* REDRAW PROMPT */
  rl_on_new_line();
  rl_redisplay();
}



/* CLEAN READLINE REACHABLE */
int
cleanup_readline(void) 
{
  /* 1. CLEAR HISTORY */
  clear_history();

  /* 2. CLEAN UP INNER STRUCTURES OF HISTORY */
  HIST_ENTRY** list = history_list();
  if (list) {
    for (int i = 0; list[i]; i++) {
      free_history_entry(list[i]);
    }
  }

  /* 3. RESET TERMINAL */
  rl_deprep_terminal();

  /* 4. DISABLE readline */
  rl_cleanup_after_signal();
  rl_clear_message();

  /* 5. FREE completion */
  rl_clear_pending_input();
  rl_free_line_state();

  return 1;
}
