// Euphoria - Copyright (c) Gustav

#include "euphoria/enum.h"

#include <fstream> // NOLINT for logging
#include <cassert>
#include <string>
#include <sstream>

#include "json/json.h"

EnumType::EnumType()
  : isAdding(true)
  , nextIndex(0) {
}

EnumType::~EnumType() {
  assert(isAdding == false);
  assert(createdButNotAddedList.empty() == true);
}

const std::string& EnumType::toString(size_t v) const {
  assert(v < nextIndex);
  List::const_iterator f = list.find(v);
  if (f != list.end()) {
    return f->second;
  } else {
    List::const_iterator i = createdButNotAddedList.find(v);
    if (i == createdButNotAddedList.end()) {
      throw "unknown index";
    } else {
      return i->second;
    }
  }
}

const EnumValue EnumType::toEnum(const std::string& name) {
  Map::const_iterator r = map.find(name);
  if (r == map.end()) {
    if (isAdding) {
      const size_t id = nextIndex;
      ++nextIndex;
      createdButNotAddedList.insert(List::value_type(id, name));
      createdButNotAddedMap.insert(Map::value_type(name, id));
      map.insert(Map::value_type(name, id));
      return EnumValue(this, id);
    } else {
      throw "loading has finished, enum doesnt exist or is misspelled";
    }
  } else {
    return EnumValue(this, r->second);
  }
}

void EnumType::addEnum(const std::string& name) {
  assert(isAdding == true);
  Map::const_iterator r = map.find(name);
  if (r == map.end()) {
    const size_t id = nextIndex;
    ++nextIndex;
    list.insert(List::value_type(id, name));
    map.insert(Map::value_type(name, id));
  } else {
    Map::const_iterator f = createdButNotAddedMap.find(name);
    if (f == createdButNotAddedMap.end()) {
      throw "enum already added";
    } else {
      // move to list
      const size_t id = f->second;
      List::const_iterator i = createdButNotAddedList.find(id);
      // createdButNotAdded list/map inconsistencies
      assert(i != createdButNotAddedList.end());
      createdButNotAddedList.erase(i);
      createdButNotAddedMap.erase(f);
    }
  }
}

void EnumType::stopAdding() {
  assert(isAdding == true);
  isAdding = false;
  assert(createdButNotAddedList.empty() == true);  // if this isn't empty,
  // some enums have not been added or misspelling has occurred,
  // see throw above
}

void Load(EnumType* type, const std::string& filename) {
  assert(type);
  // assert(type->isAdding());

  Json::Value root;   // will contains the root value after parsing.
  Json::Reader reader;
  std::ifstream file(filename.c_str());
  if (!file) {
    throw "Failed to open json";
  }
  bool parsingSuccessful = reader.parse(file, root);
  if (!parsingSuccessful) {
    std::stringstream ss;
    ss << "Failed to parse " << file << ": "
       << reader.getFormattedErrorMessages();
    throw ss.str();
  }

  for (Json::Value::ArrayIndex index = 0; index < root.size(); ++index) {
    type->addEnum(root[index].asString());
  }

  type->stopAdding();
}

EnumValue::EnumValue(EnumType* t, size_t v)
  : type(t)
  , value(v) {
}

const std::string EnumValue::toString() const {
  return type->toString(value);
}

const size_t EnumValue::toValue() const {
  return value;
}

bool EnumValue::operator==(const EnumValue& other) const {
  assert(type == other.type);
  return value == other.value;
}
bool EnumValue::operator!=(const EnumValue& other) const {
  return !(*this == other);
}
bool EnumValue::operator<(const EnumValue& other) const {
  assert(type == other.type);
  return value < other.value;
}
std::ostream& operator<<(std::ostream& s, const EnumValue& v) {
  s << v.toString();
  return s;
}
