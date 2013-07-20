// Euphoria - Copyright (c) Gustav

#include "euphoria/shader.h"

#include <memory>
#include <cassert>
#include <algorithm>
#include <string>

#include "euphoria/ogldebug.h"

Shader::Shader(const Type& type)
  : shader(glCreateShader(type == Shader::Vertex ? GL_VERTEX_SHADER :
                          GL_FRAGMENT_SHADER)) {
  assert(this);

  if (shader == 0) {
    throw "glCreateShader failed";
  }
}

std::shared_ptr<Shader> Shader::FromSource(const std::string& source,
    const Type& type) {
  std::shared_ptr<Shader> shader(new Shader(type));
  shader->compile(source);
  return shader;
}

void Shader::compile(const std::string& source) {
  assert(this);
  assert(shader != 0);

  const char* code = source.c_str();
  glShaderSource(shader, 1, (const GLchar**)&code, NULL);
  glCompileShader(shader);

  GLint status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE) {
    std::string msg("Compile failure in shader:\n");

    GLint infoLogLength;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
    std::unique_ptr<char[]> strInfoLog(new char[infoLogLength + 1]);
    glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog.get());
    msg += strInfoLog.get();
    throw msg;
  }
}

Shader::~Shader() {
  assert(this);
  assert(shader != 0);

  glDeleteShader(shader);
}

GLuint Shader::get() const {
  assert(this);
  assert(shader != 0);

  return shader;
}

/////////////////////

ShaderList& ShaderList::operator()(std::shared_ptr<Shader> shader) {
  assert(this);

  shaders.push_back(shader);
  return *this;
}

/////////////////////

Program::Program()
  : program(glCreateProgram()) {
  assert(this);

  if (program == 0) {
    throw "glCreateProgram failed";
  }
}

std::shared_ptr<Program> Program::FromShaderList(const ShaderList& shaders) {
  if (shaders.shaders.empty()) {
    throw "No shaders were provided to create the program";
  }

  std::shared_ptr<Program> program(new Program());

  for (auto sh : shaders.shaders) {
    glAttachShader(program->get(), sh->get());
  }
  glLinkProgram(program->get());
  for (auto sh : shaders.shaders) {
    glDetachShader(program->get(), sh->get());
  }

  GLint status;
  glGetProgramiv(program->get(), GL_LINK_STATUS, &status);
  if (status == GL_FALSE) {
    std::string msg("Program linking failure: ");

    GLint infoLogLength;
    glGetProgramiv(program->get(), GL_INFO_LOG_LENGTH, &infoLogLength);
    std::unique_ptr<char[]> strInfoLog(new char[infoLogLength + 1]);
    glGetProgramInfoLog(program->get(), infoLogLength, NULL, strInfoLog.get());
    msg += strInfoLog.get();
    throw msg;
  }

  return program;
}

GLuint Program::get() const {
  assert(this);
  assert(program != 0);

  return program;
}

GLint Program::attrib(const std::string& name) const {
  assert(this);
  assert(program != 0);

  const GLint attrib = glGetAttribLocation(program, name.c_str());
  if (attrib == -1) {
    throw "Program attribute not found: " + name;
  }
  return attrib;
}

GLint Program::uniform(const std::string& name) const {
  assert(this);
  assert(program != 0);

  const GLint attrib = glGetUniformLocation(program, name.c_str());
  if (attrib == -1) {
    throw "Program uniform not found: " + name;
  }
  return attrib;
}

Program::~Program() {
  assert(this);
  assert(program != 0);

  glDeleteProgram(program);
}

namespace {
  const Program*& CurrentBoundProgram() {
    static const Program* prog = 0;
    return prog;
  }
}

void Program::setUniform(const std::string& name, int i) const {
  assert(this);
  assert(program != 0);
  assert(CurrentBoundProgram() == this);
  const GLint uni = uniform(name);
  glUniform1i(uni, i);
}

void Program::setUniform(const std::string& name, const mat44& m) const {
  assert(this);
  assert(program != 0);
  assert(CurrentBoundProgram() == this);
  const GLint uni = uniform(name);
  glUniformMatrix4fv(uni, 1, GL_FALSE, m.data());
}

void Program::bind() const {
  assert(this);
  assert(program != 0);

  assert(CurrentBoundProgram() == 0);
  CurrentBoundProgram() = this;

  glUseProgram(program);
}

void Program::unbind() const {
  assert(this);
  assert(program != 0);

  assert(CurrentBoundProgram() == this);
  CurrentBoundProgram() = 0;

  glUseProgram(0);
}
