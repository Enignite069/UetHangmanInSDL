#ifndef OPTIMIZE_H_INCLUDED
#define OPTIMIZE_H_INCLUDED

#include <string>

std::string normalize(const std::string& s);
std::string chooseWord(const std::string& fileName, int difficult);
bool contains(std::string word, char guess);

#endif // OPTIMIZE_H_INCLUDED
