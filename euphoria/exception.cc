// Euphoria - Copyright (c) Gustav

#include "euphoria/exception.h"

#include <cassert>
#include <string>

namespace euphoria {

std::string GrabExceptionInformation() {
  try {
    throw;
  } catch (const std::exception& e) {
    return e.what();
  } catch (const char* const err) {
    return err;
  } catch (const std::string& err) {
    return err;
  } catch (...) {
    return "unknown error";
  }
  return "missing exception";
}
}  // namespace euphoria
