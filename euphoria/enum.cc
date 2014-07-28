// Euphoria - Copyright (c) Gustav

#include "euphoria/enum.h"

#include <fstream>  // NOLINT for logging
#include <cassert>
#include <string>
#include <sstream>

#include "json/json.h"
#include "euphoria/str.h"

EnumType::EnumType() : is_adding_(true), next_index_(0) {}

EnumType::~EnumType() {
  assert(is_adding_ == false);
  assert(created_but_not_added_list_.empty() == true);
}

const std::string& EnumType::ToString(size_t v) const {
  assert(v < next_index_);
  List::const_iterator f = list_.find(v);
  if (f != list_.end()) {
    return f->second;
  } else {
    List::const_iterator i = created_but_not_added_list_.find(v);
    if (i == created_but_not_added_list_.end()) {
      const std::string error = Str() << "unknown index " << v;
      throw error;
    } else {
      return i->second;
    }
  }
}

const EnumValue EnumType::ToEnum(const std::string& name) {
  Map::const_iterator r = map_.find(name);
  if (r == map_.end()) {
    if (is_adding_) {
      const size_t id = next_index_;
      ++next_index_;
      created_but_not_added_list_.insert(List::value_type(id, name));
      created_but_not_added_map_.insert(Map::value_type(name, id));
      map_.insert(Map::value_type(name, id));
      return EnumValue(this, id);
    } else {
      const std::string error =
          Str() << "loading has finished, enum doesn't exist or is misspelled "
                << name;
      throw error;
    }
  } else {
    return EnumValue(this, r->second);
  }
}

void EnumType::AddEnum(const std::string& name) {
  assert(is_adding_ == true);
  Map::const_iterator r = map_.find(name);
  if (r == map_.end()) {
    const size_t id = next_index_;
    ++next_index_;
    list_.insert(List::value_type(id, name));
    map_.insert(Map::value_type(name, id));
  } else {
    Map::iterator f = created_but_not_added_map_.find(name);
    if (f == created_but_not_added_map_.end()) {
      const std::string error = Str() << "enum " << name << " already added";
      throw error;
    } else {
      // move to list
      const size_t id = f->second;
      List::iterator i = created_but_not_added_list_.find(id);
      // createdButNotAdded list/map inconsistencies
      assert(i != created_but_not_added_list_.end());
      created_but_not_added_list_.erase(i);
      created_but_not_added_map_.erase(f);
    }
  }
}

void EnumType::StopAdding() {
  assert(is_adding_ == true);
  is_adding_ = false;
  assert(created_but_not_added_list_.empty() == true);  // if this isn't empty,
  // some enums have not been added or misspelling has occurred,
  // see throw above
}

void Load(EnumType* type, const std::string& filename) {
  assert(type);
  // assert(type->isAdding());

  Json::Value root;  // will contains the root value after parsing.
  Json::Reader reader;
  std::ifstream file(filename.c_str());
  if (!file) {
    const std::string error = Str() << "Failed to open json " << filename;
    throw error;
  }
  bool parsingSuccessful = reader.parse(file, root);
  if (!parsingSuccessful) {
    const std::string error = Str() << "Failed to parse " << filename << ": "
                                    << reader.getFormattedErrorMessages();
    throw error;
  }

  for (Json::Value::ArrayIndex index = 0; index < root.size(); ++index) {
    type->AddEnum(root[index].asString());
  }

  type->StopAdding();
}

EnumValue::EnumValue(const EnumType* const type, size_t value)
    : type_(type), value_(value) {}

const std::string EnumValue::ToString() const {
  return type_->ToString(value_);
}

const size_t EnumValue::ToValue() const { return value_; }

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
