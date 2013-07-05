// Euphoria - Copyright (c) Gustav

#ifndef EUPHORIA_STRINGMERGER_H_
#define EUPHORIA_STRINGMERGER_H_

#include <string>
#include <vector>

class StringMerger {
  public:
    static const StringMerger& English();
    static const StringMerger& Array();

    StringMerger& between(const std::string& seperator,
                          const std::string finalSeperator);

    StringMerger& between(const std::string& seperator);

    StringMerger& empty(const std::string& empty);

    StringMerger& startend(const std::string& astart,
                           const std::string& aend);

    std::string generate(const std::vector<std::string>& strings) const;

  private:
    std::string sep;
    std::string fisep;
    std::string mempty;
    std::string start;
    std::string end;
};

// move to a external util function and move whole class implementation
// to a cpp file?
template<class C>
std::vector<std::string> iterate(const C& c) {
  std::vector<std::string> strings;
  for (auto i = c.begin(); i != c.end(); ++i) {
    strings.push_back(Str() << *i);
  }
  return strings;
}

#endif  // EUPHORIA_STRINGMERGER_H_
