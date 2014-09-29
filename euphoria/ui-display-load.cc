// Euphoria - Copyright (c) Gustav

#include <cassert>
#include <stdexcept>
#include <fstream>  // NOLINT for loading data

#include "json/json.h"

#include "euphoria/str.h"
#include "euphoria/stringutils.h"
#include "euphoria/texturecache.h"
#include "euphoria/shadercache.h"
#include "euphoria/ninepatch.h"

#include "euphoria/ui-container-fill.h"
#include "euphoria/ui-container-table.h"
#include "euphoria/ui-widget-image.h"
#include "euphoria/ui-widget-progressbar.h"

namespace euphoria {
namespace ui {
namespace detail {
namespace {
bool EndsWithThenStore(std::string* ret, const std::string& to_test,
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
  if (EndsWithThenStore(&value_string, data, "px")) {
    return Value(std::stof(value_string), Unit::PIXEL);
  } else if (EndsWithThenStore(&value_string, data, "%")) {
    return Value(std::stof(value_string) / 100.0f, Unit::RELATIVE);
  } else if (EndsWithThenStore(&value_string, data, "*")) {
    return Value(std::stof(value_string), Unit::AVAILABLE);
  } else {
    throw std::logic_error(Str() << "Unable to parse size data " << data);
  }
}

std::vector<Size> ReadSizeVector(const Json::Value& v) {
  std::vector<Size> ret;
  for (Json::ArrayIndex i = 0; i < v.size(); ++i) {
    const std::string size_string = v.get(i, "").asString();
    Size size;
    size.size = ParseSize(size_string);
    ret.push_back(size);
  }
  return ret;
}

SizeRule ParseSizeRule(const std::string& sizerule_name) {
  const auto name = ToLower(Trim(sizerule_name));
  if (name == "aspectfill")
    return SizeRule::ASPECT_FILL;
  else if (name == "aspectfit")
    return SizeRule::ASPECT_FIT;
  else if (name == "fill")
    return SizeRule::FILL;
  else
    throw std::logic_error(
        Str() << "Unable to determine sizerule name name from " << name);
}

Alignment ParseAlignment(const std::string& alignment_name) {
  const auto name = ToLower(Trim(alignment_name));

  if (name == "upperleft") {
    return Alignment::UPPER_LEFT;
  } else if (name == "left") {
    return Alignment::LEFT;
  } else if (name == "lowerleft") {
    return Alignment::LOWER_LEFT;
  } else if (name == "top") {
    return Alignment::TOP;
  } else if (name == "center") {
    return Alignment::CENTER;
  } else if (name == "down") {
    return Alignment::DOWN;
  } else if (name == "upperright") {
    return Alignment::UPPER_RIGHT;
  } else if (name == "right") {
    return Alignment::RIGHT;
  } else if (name == "lowerright") {
    return Alignment::LOWER_RIGHT;
  } else {
    throw std::logic_error(
        Str() << "Unable to determine alignment name name from " << name);
  }
}

//////////////////////////////////////////////////////////////////////////

std::shared_ptr<Container> ReadContainer(const Json::Value data,
                                         TextureCache* tcache,
                                         ShaderCache* scache,
                                         const Settings& settings);

//////////////////////////////////////////////////////////////////////////

void ReadWidgetCommonData(std::shared_ptr<Widget> widget,
                          const Json::Value data) {
  const std::string sizerule_name = data.get("sizerule", "").asCString();
  if (sizerule_name.empty() == false) {
    widget->set_sizerule(ParseSizeRule(sizerule_name));
  }

  const std::string alignment_name = data.get("alignment", "").asCString();
  if (alignment_name.empty() == false) {
    widget->set_alignment(ParseAlignment(alignment_name));
  }
}

std::shared_ptr<Widget> ReadImageWidget(const Json::Value data,
                                        TextureCache* tcache,
                                        ShaderCache* scache,
                                        const Settings& settings) {
  const std::string& path =
      data.get("src", "no-texture-in-ui-script").asCString();
  const TextureLoadingInstruction texture(path, WrapMode::CLAMP_TO_EDGE,
                                          WrapMode::CLAMP_TO_EDGE,
                                          ImageStorage::COMPRESSED_RGBA);
  std::shared_ptr<Widget> widget(new ImageWidget(
      settings, scache, tcache->GetOrCreate(texture, settings)));
  ReadWidgetCommonData(widget, data);
  return widget;
}

std::shared_ptr<Widget> ReadProgressbarWidget(const Json::Value data,
                                              TextureCache* tcache,
                                              ShaderCache* scache,
                                              const Settings& settings) {
  const std::string ninepatch_path = data.get("ninepatch", "").asString();
  const float value = data.get("progress", 0.5).asFloat();
  const Ninepatch ninepatch = LoadNinepatch(ninepatch_path);
  std::shared_ptr<ProgressbarWidget> widget(
      new ProgressbarWidget(ninepatch, tcache, scache, settings));
  widget->set_value(value);
  ReadWidgetCommonData(widget, data);
  return widget;
}

std::shared_ptr<Container> ReadTableContainer(const Json::Value root,
                                              TextureCache* tcache,
                                              ShaderCache* scache,
                                              const Settings& settings) {
  std::shared_ptr<TableContainer> table(new TableContainer());

  std::vector<Size> rows = ReadSizeVector(root["rows"]);
  std::vector<Size> cols = ReadSizeVector(root["cols"]);

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

    std::shared_ptr<Container> container =
        ReadContainer(data, tcache, scache, settings);
    const Cell cell(container, Vec2i(x, y), Vec2i(w, h));
    table->AddCell(cell);
  }

  return table;
}

std::shared_ptr<Container> ReadFillContainer(const Json::Value root,
                                             TextureCache* tcache,
                                             ShaderCache* scache,
                                             const Settings& settings) {
  std::shared_ptr<FillContainer> fill(new FillContainer());
  Json::Value cells = root["layers"];
  for (Json::ArrayIndex i = 0; i < cells.size(); ++i) {
    Json::Value data = cells[i];
    std::shared_ptr<Container> container =
        ReadContainer(data, tcache, scache, settings);
    fill->Add(container);
  }
  return fill;
}

std::shared_ptr<Container> ReadContainer(const Json::Value data,
                                         TextureCache* tcache,
                                         ShaderCache* scache,
                                         const Settings& settings) {
  const std::string type =
      ToLower(Trim(data.get("type", "no-type-specified").asCString()));
  if (type == "image") {
    return ReadImageWidget(data, tcache, scache, settings);
  } else if (type == "progressbar") {
    return ReadProgressbarWidget(data, tcache, scache, settings);
  } else if (type == "table") {
    return ReadTableContainer(data, tcache, scache, settings);
  } else if (type == "fill") {
    return ReadFillContainer(data, tcache, scache, settings);
  } else {
    throw std::logic_error(Str() << "Unknown widget/container type " << type);
  }
}
}  // namespace

std::shared_ptr<Container> LoadContainer(const std::string& filename,
                                         TextureCache* tcache,
                                         ShaderCache* scache,
                                         const Settings& settings) {
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

  return ReadContainer(root, tcache, scache, settings);
}
}  // namespace detail

}  // namespace ui
}  // namespace euphoria
