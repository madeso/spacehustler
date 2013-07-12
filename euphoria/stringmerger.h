// Euphoria - Copyright (c) Gustav

/** @file
Contains the string merger class.
 */

#ifndef EUPHORIA_STRINGMERGER_H_
#define EUPHORIA_STRINGMERGER_H_

#include <string>
#include <vector>
#include <map>

/** Util class for merging strings.
 */
class StringMerger {
  public:
    /** English merger. Combines with a and.
    @returns the merger.
     */
    static const StringMerger& EnglishAnd();

    /** English merger. Combines with a or.
    @returns the merger.
     */
    static const StringMerger& EnglishOr();

    /** Array merger. Generate strings that looks like [a, b, c].
    @returns the merger.
     */
    static const StringMerger& Array();

    /** Sets the seperator arguments.
    @param seperator the seperator to use.
    @param finalSeperator the seperator to use at the last element.
    @returns this for easy chaining.
     */
    StringMerger& between(const std::string& seperator,
                          const std::string finalSeperator);

    /** Sets the seperator arguments.
    @param seperator the seperator to use.
    @returns this for easy chaining.
     */
    StringMerger& between(const std::string& seperator);

    /** Sets the empty string.
    This string is returned when a request is made on a a empty set.
    @param empty the empty string.
    @returns this for easy chaining.
     */
    StringMerger& empty(const std::string& empty);

    /** Sets the start and end text. This data is always added.
    @param astart the start string.
    @param aend the end string.
    @returns this for easy chaining.
     */
    StringMerger& startend(const std::string& astart,
                           const std::string& aend);

    /** Generate a string representation based on the current settings.
    @param strings the strings.
    @returns the combined string.
     */
    std::string generate(const std::vector<std::string>& strings) const;

  private:
    std::string sep;
    std::string fisep;
    std::string mempty;
    std::string start;
    std::string end;
};

/** Generate a string representation for each of the elements in a vector.
@todo move to a better place.
@param c the container.
@returns the string vector.
 */
template<typename C>
std::vector<std::string> Iterate(const C& c) {
  std::vector<std::string> strings;
  for (const auto & x : c) {
    strings.push_back(Str() << x);
  }
  return strings;
}

/** Generate a string representation for each of the keys in a map.
@todo move to a better place.
@param m the map.
@returns the string vector.
 */
template<typename K, typename V>
std::vector<std::string> Keys(const std::map<K, V>& m) {
  std::vector<std::string> strings;
  for (const auto & x : m) {
    strings.push_back(Str() << x.first);
  }
  return strings;
}

#endif  // EUPHORIA_STRINGMERGER_H_
