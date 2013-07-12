// Euphoria - Copyright (c) Gustav

/** @file
Contains the Str class.
 */

#ifndef EUPHORIA_STR_H_
#define EUPHORIA_STR_H_

#include <string>
#include <sstream>

/** Basic string builder.
 */
class Str {
  public:
    /** Generate a string.
    @see toString()
    @return the generated string.
     */
    operator std::string() const;

    /** Generate a string.
    @see operator std::string()
    @return the generated string.
     */
    std::string toString() const;

    /** Add stuff to the stream.
    @param t object to add to the stream.
    @return this
     */
    template<typename Type>
    Str& operator<<(const Type& t) {
      stream << t;
      return *this;
    }

  private:
    std::ostringstream stream;
};

#endif  // EUPHORIA_STR_H_