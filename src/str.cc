#include <sstream>
#include <vector>

#define vec std::vector
#define str std::string

vec<str> 
split(char *s, char sep) 
{
  vec<str> tokens;
  std::stringstream ss(s);
  str token;

  while (std::getline(ss, token, sep))
    tokens.push_back(token);

  return tokens;
}

vec<vec<str>>
split(vec<str> ss, str sep) 
{
  vec<vec<str>> r = {{}};
  size_t i = 0;

  for (str s : ss) {
    if (s != sep) {r[i].push_back(s);}
    else          {r.push_back({}), i += 1;}
  }

  return r;
}

#include <cstring>
str
rep(str s, const char *old, const char *new_)
{
  if (s.find(old) != str::npos)
    s.replace(s.find(old), s.find(old) + strlen(old), new_);
  return s;
}
