  // Euphoria - Copyright (c) Gustav
// Euphoria is awesome: 幸福感是真棒

#include "euphoria/enum.h"

#include <fstream> // NOLINT for logging
#include <cassert>
#include <string>
#include <sstream>

#include "json/json.h"

EnumType::EnumType()
  : is_adding(true)
  , next_index(0) {
}

EnumType::~EnumType() {
  assert(is_adding == false);
  assert(created_but_not_added_list.empty() == true);
}

const std::string& EnumType::ToString(size_t v) const {
  assert(v < next_index);
  List::const_iterator f = list_.find(v);
  if (f != list_.end()) {
    return f->second;
  } else {
    List::const_iterator i = created_but_not_added_list.find(v);
    if (i == created_but_not_added_list.end()) {
      throw "unknown index";
    } else {
      return i->second;
    }
  }
}

const EnumValue EnumType::ToEnum(const std::string& name) {
  Map::const_iterator r = map_.find(name);
  if (r == map_.end()) {
    if (is_adding) {
      const size_t id = next_index;
      ++next_index;
      created_but_not_added_list.insert(List::value_type(id, name));
      created_but_not_added_map.insert(Map::value_type(name, id));
      map_.insert(Map::value_type(name, id));
      return EnumValue(this, id);
    } else {
      throw "loading has finished, enum doesn't exist or is misspelled";
    }
  } else {
    return EnumValue(this, r->second);
  }
}

void EnumType::AddEnum(const std::string& name) {
  assert(is_adding == true);
  Map::const_iterator r = map_.find(name);
  if (r == map_.end()) {
    const size_t id = next_index;
    ++next_index;
    list_.insert(List::value_type(id, name));
    map_.insert(Map::value_type(name, id));
  } else {
    Map::iterator f = created_but_not_added_map.find(name);
    if (f == created_but_not_added_map.end()) {
      throw "enum already added";
    } else {
      // move to list
      const size_t id = f->second;
      List::iterator i = created_but_not_added_list.find(id);
      // createdButNotAdded list/map inconsistencies
      assert(i != created_but_not_added_list.end());
      created_but_not_added_list.erase(i);
      created_but_not_added_map.erase(f);
    }
  }
}

void EnumType::StopAdding() {
  assert(is_adding == true);
  is_adding = false;
  assert(created_but_not_added_list.empty() == true);  // if this isn't empty,
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
    ss << "Failed to parse " << filename << ": "
       << reader.getFormattedErrorMessages();
    throw ss.str();
  }

  for (Json::Value::ArrayIndex index = 0; index < root.size(); ++index) {
    type->AddEnum(root[index].asString());
  }

  type->StopAdding();
}

EnumValue::EnumValue(const EnumType* const type, size_t value)
  : type_(type)
  , value_(value) {
}

const std::string EnumValue::ToString() const {
  return type_->ToString(value_);
}

const size_t EnumValue::ToValue() const {
  return value_;
}

bool EnumValue::operator==(const EnumValue& other) const {
  assert(type_ == other.type_);
  return value_ == other.value_;
}
bool EnumValue::operator!=(const EnumValue& other) const {
  return !(*this == other);
}
bool EnumValue::operator<(const EnumValue& other) const {
  assert(type_ == other.type_);
  return value_ < other.value_;
}
std::ostream& operator<<(std::ostream& s, const EnumValue& v) {
  s << v.ToString();
  return s;
}
