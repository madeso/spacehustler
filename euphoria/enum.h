// Euphoria - Copyright (c) Gustav

/** @file
Enum related code.
 */

#ifndef EUPHORIA_ENUM_H_
#define EUPHORIA_ENUM_H_

#include <string>
#include <map>
#include <vector>


class EnumValue;

/** Represents a enum type.
Declare globally grab specific enums before or after load, and load adds new,
verifies created and asserts misspelled values.
 */
class EnumType {
  public:
    /** Constructor.
     */
    EnumType();

    /** Destructor.
     */
    ~EnumType();

    /** Creates or gets a enum value.
    @param name the unique name of the value.
    @returns the enum value.
     */
    const EnumValue toEnum(const std::string& name);

    /** Adds a valid enum value.
    Pretty much only usable during loading.
    @param name the unique name.
     */
    void addEnum(const std::string& name);

    /** Stops adding values to the enum.
    Puts the type in static mode, meaning no new values can be added and added
    values are verified against the valid values.
     */
    void stopAdding();

  protected:
    friend class EnumValue;

    /** Internal.
    Construct a string from the enum type representation.
    @param v the enum.
    @returns the string representation.
     */
    const std::string& toString(size_t v) const;

  private:
    typedef std::map<size_t, std::string> List;
    typedef std::map<std::string, size_t> Map;
    List list;
    Map map;

    List createdButNotAddedList;
    Map createdButNotAddedMap;
    bool isAdding;
    size_t nextIndex;
};

/** Adds values from a file to a EnumType.
@param type the EnumType to add to.
@param filename the filename to load values from.
 */
void Load(EnumType* type, const std::string& filename);

/** A enum value.
 */
class EnumValue {
  public:
    /** Create a string representation.
    Useful for debugging and displaying stuff.
    @returns the string representation.
     */
    const std::string toString() const;

    /** Gets the actual value.
    @return the value.
     */
    const size_t toValue() const;

    /** Equal operator.
    @param other the other value.
    @returns true if equal, false if not.
     */
    bool operator==(const EnumValue& other) const;

    /** Not equal operator.
    @param other the other value.
    @returns true if not equal, false if not.
     */
    bool operator!=(const EnumValue& other) const;

    /** Less than operator.
    @param other the other value.
    @returns true if this is less than the other, false if not.
     */
    bool operator<(const EnumValue& other) const;

  protected:
    friend class EnumType;

    /** Internal constructor.
      @param type the type of the value.
      @param value the actual value.
       */
    EnumValue(const EnumType* const type, size_t value);

  private:
    const EnumType* const type;
    const size_t value;
};

/** Output the EnumValue to a stream.
@param s the stream to output to.
@param v the value to output.
@returns the stream
 */
std::ostream& operator<<(std::ostream& s, const EnumValue& v);


#endif  // EUPHORIA_ENUM_H_
