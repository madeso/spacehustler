// Euphoria - Copyright (c) Gustav

/** @file
Contains the shader logic.
 */

#ifndef EUPHORIA_SHADER_H_
#define EUPHORIA_SHADER_H_

#include <boost/noncopyable.hpp>

#include <memory>
#include <vector>
#include <string>

#include "euphoria/opengl.h"
#include "euphoria/math.h"

/** A compiled OpenGL vertex or fragment shader.
 */
class Shader
  : boost::noncopyable {
  public:
    /** The type of the shader.
     */
    enum Type {
      /** The vertex shader type.
       */
      kVertexShader,

      /** The fragment shader type.
       */
      kFragmentShader
    };

    /** Create a shader from source.
    @param source the source code for the shader.
    @param type the shader type, if it's a vertex or a fragment shader.
    @return a new shader.
     */
    static std::shared_ptr<Shader> CreateFromSource(const std::string& source,
        const Type& type);

    /** Destructs the shader.
     */
    ~Shader();

    /** Gets the OpenGL shader object.
    @return the shader object.
     */
    GLuint object() const;
  private:
    explicit Shader(const Type& type);
    void Compile(const std::string& source);
    GLuint object_;
};

/** Helper combiner class for specifying multiple shaders.
 */
class ShaderList {
  public:
    /** Adds another shader to the list.
    @param shader the shader to add to the list
    @return this object for easily chaining function calls.
     */
    ShaderList& operator()(std::shared_ptr<Shader> shader);

    /** The actual list of the shaders.
     */
    std::vector<std::shared_ptr<Shader> > shaders;
};

/** A OpenGl shader program full of shaders.
@see Shader
 */
class Program
  : boost::noncopyable {
  public:
    /** Create program from a list of shaders.
    @param shaders the list of shaders.
    @return the created program.
     */
    static std::shared_ptr<Program> FromShaderList(const ShaderList& shaders);

    /** Destructs the program.
     */
    ~Program();

    /** Get the OpenGL program id.
    @return the program id.
     */
    GLuint object() const;

    /** Do a attribute lookup.
    @param name the attribute name.
    @return the OpenGL attribute id.
     */
    GLint LookupAttribute(const std::string& name) const;

    /** Do a uniform lookup.
    @param name the uniform name.
    @return the OpenGL uniform id.
     */
    GLint LookupUniform(const std::string& name) const;

    /** Set a uniform integer.
    @param name the uniform name.
    @param i the integer value.
     */
    void SetUniform(const std::string& name, int i) const;

    /** Set a uniform 4x4 matrix.
    @param name the uniform name.
    @param m the 4x4 matrix value.
     */
    void SetUniform(const std::string& name, const mat44& m) const;

    /** Set a uniform vec2.
    @param name the uniform name.
    @param v the vec2 value.
     */
    void SetUniform(const std::string& name, const vec2& v) const;

    /** Set a uniform vec4.
    @param name the uniform name.
    @param v the vec4 value
     */
    void SetUniform(const std::string& name, const vec4& v) const;

    /** Binds this program.
     */
    void Bind() const;

    /** Unbinds this program.
     */
    void Unbind() const;

  private:
    Program();
    GLuint object_;
};

/** Load a shader from a file.
@param filename the path to the shader file.
@returns the newly loaded shader.
 */
std::shared_ptr<Program> LoadShaderFromFile(const std::string& filename);

#endif  // EUPHORIA_SHADER_H_
