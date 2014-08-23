// Euphoria - Copyright (c) Gustav

#include "euphoria/ui-table.h"

#include <cassert>
#include <stdexcept>
#include <fstream>  // NOLINT for loading data

#include "euphoria/str.h"
#include "euphoria/stringutils.h"
#include "json/json.h"

namespace euphoria {
namespace ui {

namespace {

bool StoreEndsWith(std::string* ret, const std::string& to_test,
                   const std::string& ends) {
  assert(ret);
  const bool ew = EndsWith(to_test, ends);
  if (ew) {
    *ret = Trim(to_test.substr(0, to_test.length() - ends.length()));
  }
  return ew;
}

Value ParseSize(const std::string& data) {
  std::string value_string;
  if (StoreEndsWith(&value_string, data, "px")) {
    return Value(std::stof(value_string), Unit::PIXEL);
  } else if (StoreEndsWith(&value_string, data, "%")) {
    return Value(std::stof(value_string) / 100.0f, Unit::RELATIVE);
  } else if (StoreEndsWith(&value_string, data, "*")) {
    return Value(std::stof(value_string), Unit::AVAILABLE);
  } else {
    throw std::logic_error(Str() << "Unable to parse size data " << data);
  }
}

std::vector<Size> ReadSizes(const Json::Value& v) {
  std::vector<Size> ret;
  for (Json::ArrayIndex i = 0; i < v.size(); ++i) {
    const std::string size_string = v.get(i, "").asString();
    Size size;
    size.size = ParseSize(size_string);
    ret.push_back(size);
  }
  return ret;
}
}  // namespace

void LoadTable(Table* table, const std::string& filename) {
  assert(table);
  std::ifstream in(filename.c_str());
  if (!in.good()) {
    throw std::logic_error(Str() << "Unable to load ui table from  "
                                 << filename);
  }
  Json::Value root;
  Json::Reader reader;
  if (false == reader.parse(in, root)) {
    throw std::logic_error(Str() << "Unable to parse " << filename << ": "
                                 << reader.getFormattedErrorMessages());
  }

  std::vector<Size> rows = ReadSizes(root["rows"]);
  std::vector<Size> cols = ReadSizes(root["cols"]);

  table->set_rows(rows);
  table->set_columns(cols);

  /// @todo load cells
}

}  // namespace ui
}  // namespace euphoria
