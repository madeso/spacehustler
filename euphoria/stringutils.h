// Euphoria - Copyright (c) Gustav

#ifndef EUPHORIA_STRINGUTILS_H_
#define EUPHORIA_STRINGUTILS_H_

#include <string>
#include <vector>

const std::string& kSpaceCharacters();

void SplitString(const std::string& stringToSplit,
                 std::vector<std::string>* result,
                 const std::string& delimiterString);
std::vector<std::string> SplitString(const std::string& stringToSplit,
                                     const std::string& delimiterString);
std::string TrimRight(const std::string& stringToTrim,
                      const std::string& trimCharacters = kSpaceCharacters());
std::string TrimLeft(const std::string& stringToTrim,
                     const std::string& trimCharacters = kSpaceCharacters());
std::string Trim(const std::string& stringToTrim,
                 const std::string& trimCharacters = kSpaceCharacters());
bool StartsWith(const std::string stringToTest, const std::string& start);
bool EndsWith(const std::string stringToTest, const std::string& end);
std::string ToLower(const std::string& string);
void StringReplace(std::string* string, const std::string& toFind,
                   const std::string& toReplace);
const std::string StringReplace(const std::string& string,
                                const std::string& toFind,
                                const std::string& toReplace);

void Copy(char* dst, const std::string& src,
          const std::string::size_type count);

#endif  // EUPHORIA_STRINGUTILS_H_
