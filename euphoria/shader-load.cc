  // Euphoria - Copyright (c) Gustav
// Euphoria is awesome: 幸福感是真棒

#include "euphoria/shader.h"
#include <string>
#include <fstream>  // NOLINT for loading data
#include <sstream>
#include <stdexcept>

#include "euphoria/str.h"

#include "json/json.h"

void LoadSingleShader(Json::Value data, Shader::Type type, ShaderList* list) {
  const std::string filename = data.get("file", "").asString();
  std::ifstream f(filename.c_str());
  if (!f) {
    throw std::logic_error(Str()
                           << "Failed to open shader file " << filename);
  }
  std::stringstream ss;
  ss << f.rdbuf();
  const std::string content = ss.str();

  (*list)(Shader::CreateFromSource(content, type));
}

void LoadShaders(Json::Value data, Shader::Type type, ShaderList* list) {
  for (Json::Value::ArrayIndex index = 0; index < data.size(); ++index) {
    LoadSingleShader(data[index], type, list);
  }
}

std::shared_ptr<Program> LoadShaderFromFile(const std::string& filename) {
  Json::Value root;
  Json::Reader reader;
  std::ifstream f(filename.c_str());
  if (!f.good()) {
    throw std::logic_error(Str()
                           << "Failed to open shader file " << filename);
  }
  if (!reader.parse(f, root)) {
    throw std::logic_error(Str()
                           << "Failed to correctly parse " << filename
                           << reader.getFormattedErrorMessages());
  }

  ShaderList shaders;

  LoadShaders(root["vertices"], Shader::kVertexShader, &shaders);
  LoadShaders(root["fragments"], Shader::kFragmentShader, &shaders);

  return Program::FromShaderList(shaders);
}
