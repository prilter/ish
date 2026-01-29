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

#include <cstring>
std::string
rep(std::string s, const char *old, const char *new_)
{
  if (s.find(old) != std::string::npos)
    s.replace(s.find(old), s.find(old) + strlen(old), new_);
  return s;
}
