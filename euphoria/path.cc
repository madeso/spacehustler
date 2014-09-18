// Euphoria - Copyright (c) Gustav

#include "euphoria/path.h"

#include <cassert>
#include <utility>

#include "euphoria/stringutils.h"

namespace euphoria {
const Path Path::ROOT = Path("");

Path::Path(const std::string& path) : path_(path) {
  assert(this);
  assert(StringReplace(Trim(ToLower(path)), "\\", "/") == path);
}

bool Path::IsFile() const {
  assert(this);
  return IsDirectory() == false;
}

bool Path::IsDirectory() const {
  assert(this);
  return path_.empty() || EndsWith(path_, "/");
}

namespace {
std::pair<Path, std::string> SplitFolderFile(const std::string& path) {
  const Path pathpath(path);
  if (pathpath.IsDirectory()) return std::make_pair(pathpath, "");
  const auto lastSlash = path.find_last_of('/');
  if (lastSlash == -1) {
    // there isn't any directory, return root.
    return std::make_pair(Path::ROOT, path);
  } else {
    const Path p(path.substr(0, lastSlash));
    const auto f = path.substr(lastSlash);
    assert(p.IsDirectory());
    assert(StartsWith(f, "/") == false);
    return std::make_pair(p, f);
  }
}
std::pair<std::string, std::string> SplitFileExtension(
    const std::string& filename) {
  assert(Path(filename).IsFile());
  const auto lastdot = filename.find_last_of('.');

  // catches not found and filenames starting with ".", these are
  // considered to have no extension
  if (lastdot < 1) return std::make_pair(filename, "");

  const auto name = filename.substr(0, lastdot);
  const auto extension = filename.substr(lastdot);
  assert(EndsWith(name, ".") == false);
  assert(StartsWith(extension, "."));
  return std::make_pair(name, extension);
}
}  // namespace

const std::string Path::GetFileName() const {
  assert(this);
  assert(IsFile());
  return SplitFileExtension(GetFile()).first;
}

const std::string Path::GetExtension() const {
  assert(this);
  assert(IsFile());
  return SplitFileExtension(GetFile()).second;
}

const std::string Path::GetFile() const {
  assert(this);
  assert(IsFile());
  return SplitFolderFile(path_).second;
}

const Path Path::GetDirectory() const {
  assert(this);
  return SplitFolderFile(path_).first;
}

const Path Path::ChangeDirectory(const std::string& cd) const {
  assert(this);
  assert(IsDirectory());

  if (Path(cd).IsDirectory()) {
    return Path(path_ + cd);
  } else {
    return Path(path_ + cd + "/");
  }
}

const Path Path::File(const std::string& file) const {
  assert(this);
  assert(IsDirectory());
  assert(Path(file).IsFile());
  const Path ret(path_ + file);
  assert(ret.IsFile());
  return ret;
}

const Path Path::ChangeExtension(const std::string& ext) const {
  assert(this);
  assert(StartsWith(ext, "."));
  const auto folderfile = SplitFolderFile(path_);
  const auto filext = SplitFileExtension(folderfile.second);
  const Path ret = folderfile.first.File(filext.first + ext);
  assert(ret.IsFile());
  return ret;
}

const std::string Path::GetOsPath() const {
  assert(this);
  /// @todo fix this for other systems than windows
  return StringReplace(path_, "/", "\\");
}

}  // namespace euphoria
