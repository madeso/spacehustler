// Euphoria - Copyright (c) Gustav

#ifndef EUPHORIA_STRINGSEPERATOR_H_
#define EUPHORIA_STRINGSEPERATOR_H_

#include <string>
#include <vector>

class StringSeperator {
  public:
    typedef std::vector<std::string> Vec;

    std::string toString() const;

    StringSeperator& english();

    StringSeperator& array();

    StringSeperator& between(const std::string& seperator,
                             const std::string finalSeperator);

    StringSeperator& between(const std::string& seperator);

    StringSeperator& empty(const std::string& empty);

    StringSeperator& startend(const std::string& astart,
                              const std::string& aend);

    // move to a external util function and move whole class implementation
    // to a cpp file?
    template<class C>
    StringSeperator& iterate(const C& c) {
      for (typename C::const_iterator i = c.begin(); i != c.end(); ++i) {
        strings.push_back(Str() << *i);
      }
      return *this;
    }

  private:
    Vec strings;

    std::string sep;
    std::string fisep;
    std::string mempty;
    std::string start;
    std::string end;
};

#endif  // EUPHORIA_STRINGSEPERATOR_H_
