// Euphoria - Copyright (c) Gustav

/** @file
Path related code.
 */

#ifndef EUPHORIA_PATH_H_
#define EUPHORIA_PATH_H_

#include <string>

namespace euphoria {

class Path {
 public:
  explicit Path(const std::string& path);

  bool IsFile() const;

  bool IsDirectory() const;

  const std::string GetFileName() const;
  const std::string GetExtension() const;
  const std::string GetFile() const;

  const Path GetDirectory() const;

  const Path ChangeDirectory(const std::string& cd) const;
  const Path File(const std::string& cd) const;

  /** Change the extension of the filename.
  @param ext the extension, should start with a dot
  @returns the path with the new extension
   */
  const Path ChangeExtension(const std::string& ext) const;

  const std::string GetOsPath() const;

  static const Path ROOT;

 private:
  const std::string path_;
};

}  // namespace euphoria

#endif  // EUPHORIA_PATH_H_
