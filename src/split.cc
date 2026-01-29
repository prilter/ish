#include <sstream>
#include <vector>

std::vector<std::string> 
split(char *s, char sep) 
{
  std::vector<std::string> tokens;
  std::stringstream ss(s);
  std::string token;

  while (std::getline(ss, token, sep)) {
    tokens.push_back(token);
  }

  return tokens;
}

