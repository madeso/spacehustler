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
  const std::string GetExtension();

  const Path GetDirectory() const;

  const std::string GetOsPath() const;

  static const Path ROOT;

 private:
  std::string path_;
};

}  // namespace euphoria

#endif  // EUPHORIA_PATH_H_
