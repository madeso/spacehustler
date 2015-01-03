// Euphoria - Copyright (c) Gustav

#include "euphoria/graphics/shader.h"

#include <memory>
#include <cassert>
#include <algorithm>
#include <string>

#include "euphoria/graphics/ogldebug.h"
#include "euphoria/str.h"

namespace euphoria {

Shader::Shader(const Type& type)
    : object_(glCreateShader(type == Shader::Type::VERTEX
                                 ? GL_VERTEX_SHADER
                                 : GL_FRAGMENT_SHADER)) {
  assert(this);

  if (object_ == 0) {
    const std::string error = Str() << "glCreateShader failed";
    throw error;
  }
}

std::shared_ptr<Shader> Shader::CreateFromSource(const std::string& source,
                                                 const Type& type) {
  std::shared_ptr<Shader> shader(new Shader(type));
  shader->Compile(source);
  return shader;
}

void Shader::Compile(const std::string& source) {
  assert(this);
  assert(object_ != 0);

  const char* code = source.c_str();
  glShaderSource(object_, 1, (const GLchar**)&code, NULL);
  glCompileShader(object_);

  GLint status;
  glGetShaderiv(object_, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE) {
    std::string msg("Compile failure in shader:\n");

    GLint infoLogLength;
    glGetShaderiv(object_, GL_INFO_LOG_LENGTH, &infoLogLength);
    std::unique_ptr<char[]> strInfoLog(new char[infoLogLength + 1]);
    glGetShaderInfoLog(object_, infoLogLength, NULL, strInfoLog.get());
    msg += strInfoLog.get();
    throw msg;
  }
}

Shader::~Shader() {
  assert(this);
  assert(object_ != 0);

  glDeleteShader(object_);
}

GLuint Shader::object() const {
  assert(this);
  assert(object_ != 0);

  return object_;
}

/////////////////////

ShaderList& ShaderList::operator()(std::shared_ptr<Shader> shader) {
  assert(this);
  shaders_.push_back(shader);
  return *this;
}

const std::vector<std::shared_ptr<Shader>> ShaderList::shaders() const {
  assert(this);
  return shaders_;
}

/////////////////////

Program::Program() : object_(glCreateProgram()) {
  assert(this);

  if (object_ == 0) {
    const std::string error = Str() << "glCreateProgram failed";
    throw error;
  }
}

std::shared_ptr<Program> Program::FromShaderList(const ShaderList& shaders) {
  if (shaders.shaders().empty()) {
    const std::string error =
        Str() << "No shaders were provided to create the program";
    throw error;
  }

  std::shared_ptr<Program> program(new Program());

  for (auto sh : shaders.shaders()) {
    glAttachShader(program->object(), sh->object());
  }
  glLinkProgram(program->object());
  for (auto sh : shaders.shaders()) {
    glDetachShader(program->object(), sh->object());
  }

  GLint status;
  glGetProgramiv(program->object(), GL_LINK_STATUS, &status);
  if (status == GL_FALSE) {
    std::string msg("Program linking failure: ");

    GLint infoLogLength;
    glGetProgramiv(program->object(), GL_INFO_LOG_LENGTH, &infoLogLength);
    std::unique_ptr<char[]> strInfoLog(new char[infoLogLength + 1]);
    glGetProgramInfoLog(program->object(), infoLogLength, NULL,
                        strInfoLog.get());
    msg += strInfoLog.get();
    throw msg;
  }

  return program;
}

GLuint Program::object() const {
  assert(this);
  assert(object_ != 0);

  return object_;
}

GLint Program::LookupAttribute(const std::string& name) const {
  assert(this);
  assert(object_ != 0);

  const GLint attrib = glGetAttribLocation(object_, name.c_str());
  if (attrib == -1) {
    const std::string error = Str() << "Program attribute not found: ";
    throw error + name;
  }
  return attrib;
}

GLint Program::LookupUniform(const std::string& name) const {
  assert(this);
  assert(object_ != 0);

  const GLint attrib = glGetUniformLocation(object_, name.c_str());
  if (attrib == -1) {
    const std::string error = Str() << "Program uniform not found: ";
    throw error + name;
  }
  return attrib;
}

Program::~Program() {
  assert(this);
  assert(object_ != 0);

  glDeleteProgram(object_);
}

namespace {
const Program*& CurrentBoundProgram() {
  static const Program* prog = 0;
  return prog;
}
}

void Program::SetUniform(const std::string& name, int i) const {
  assert(this);
  assert(object_ != 0);
  assert(CurrentBoundProgram() == this);
  const GLint uni = LookupUniform(name);
  glUniform1i(uni, i);
}

void Program::SetUniform(const std::string& name, const Mat44& m) const {
  assert(this);
  assert(object_ != 0);
  assert(CurrentBoundProgram() == this);
  const GLint uni = LookupUniform(name);
  glUniformMatrix4fv(uni, 1, GL_FALSE, m.data());
}

void Program::SetUniform(const std::string& name, const Vec2& v) const {
  assert(this);
  assert(object_ != 0);
  assert(CurrentBoundProgram() == this);
  const GLint uni = LookupUniform(name);
  glUniform2f(uni, v[0], v[1]);
}
void Program::SetUniform(const std::string& name, const Vec4& v) const {
  assert(this);
  assert(object_ != 0);
  assert(CurrentBoundProgram() == this);
  const GLint uni = LookupUniform(name);
  glUniform4f(uni, v[0], v[1], v[2], v[3]);
}

void Program::Bind() const {
  assert(this);
  assert(object_ != 0);

  assert(CurrentBoundProgram() == 0);
  CurrentBoundProgram() = this;

  glUseProgram(object_);
}

void Program::Unbind() const {
  assert(this);
  assert(object_ != 0);

  assert(CurrentBoundProgram() == this);
  CurrentBoundProgram() = 0;

  glUseProgram(0);
}
}  // namespace euphoria
