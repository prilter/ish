#include <string>
#include <vector>
#include <iostream>

#include <readline/readline.h>
#include <readline/history.h>

#define str std::string
#define vec std::vector
#define out std::cout
#define in  std::cin

extern str         getcwd(void);
extern str         gethm(void);
extern vec<str>    split(char *s, char sep);
extern int         ex(const vec<str>&);
extern int         cd(const str);

#define YELLOW   "\x1b[33m"
#define WHITE    "\x1b[37m"
#define HISTDIR  ".ish_history"

int
main(void)
{
  vec<str> com;
  char *inp;

  read_history(HISTDIR);
  for (str cwd = getcwd();; cwd = getcwd()) {
    /* GET INPUT */
    inp = readline((YELLOW + cwd + WHITE + "❯ ").c_str());
    if (!inp) {std::cout << "\n"; break;}  /* Ctrl+D */
    if (*inp) add_history(inp); /* HISTORY */

    /* GET COMMAND */
    com = split(inp, ' ');
    free(inp);

    /* NO PROMPT */
    if (com.size() == 0) continue;

    /* CD */
    if (com[0] == "cd") cd(com[1]);
    else if (com[0] == "exit") break;
    else                ex(com);
  }
  write_history(HISTDIR);

  return 0;
}
