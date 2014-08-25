// Euphoria - Copyright (c) Gustav

#include "euphoria/ui-table.h"

#include <cassert>
#include <stdexcept>
#include <fstream>  // NOLINT for loading data

#include "euphoria/str.h"
#include "euphoria/stringutils.h"
#include "euphoria/ui-cell.h"
#include "euphoria/ui-layout.h"
#include "euphoria/texturecache.h"

#include "euphoria/ui-progressbar.h"
#include "euphoria/ui-image.h"

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

LayoutType GetLayoutType(const std::string& layout_name) {
  const auto name = ToLower(Trim(layout_name));
  if (name == "null")
    return LayoutType::NULL;
  else if (name == "box")
    return LayoutType::BOX;
  else if (name == "fill")
    return LayoutType::FILL;
  else
    throw std::logic_error(Str() << "Unable to determine layout name from "
                                 << name);
}

std::shared_ptr<Widget> CreateWidget(const Json::Value data,
                                     TextureCache* tcache,
                                     const Settings& settings) {
  const std::string type = ToLower(Trim(data.get("type", "").asCString()));
  if (type == "image") {
    const std::string& path = data.get("src", "").asCString();
    return std::shared_ptr<Widget>(new Image(tcache->GetOrCreate(
        TextureLoadingInstruction(path, WrapMode::CLAMP_TO_EDGE,
                                  WrapMode::CLAMP_TO_EDGE),
        settings)));
  } else if (type == "progressbar") {
    return std::shared_ptr<Widget>(new ProgressBar());
  } else {
    throw std::logic_error(Str() << "Unknown widget type " << type);
  }
}
}  // namespace

void LoadTable(Table* table, const std::string& filename, TextureCache* tcache,
               const Settings& settings) {
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
  Json::Value cells = root["cells"];
  for (Json::ArrayIndex i = 0; i < cells.size(); ++i) {
    Json::Value data = cells[i];
    const int x = data.get("x", 0).asInt();
    const int y = data.get("y", 0).asInt();
    const int w = data.get("cols", 0).asInt();
    const int h = data.get("rows", 0).asInt();
    const std::string layout_name = data.get("layout", "").asCString();

    std::shared_ptr<Cell> cell(new Cell());
    cell->Begin();
    cell->set_tile_position(Vec2i(x, y));
    cell->set_tile_size(Vec2i(w, h));
    // give the cell some dummy size and dummy position
    cell->set_position(Vec2(0, 0));
    cell->set_size(Vec2(10, 10));
    cell->set_layout(CreateLayout(GetLayoutType(layout_name)));

    Json::Value widgets = data["widgets"];
    for (Json::ArrayIndex w = 0; w < widgets.size(); ++w) {
      Json::Value wdata = widgets[w];
      cell->Add(CreateWidget(wdata, tcache, settings));
    }

    cell->End();
    table->AddCell(cell);
  }
}

}  // namespace ui
}  // namespace euphoria
