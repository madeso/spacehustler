  // Euphoria - Copyright (c) Gustav
// Euphoria is awesome: 幸福感是真棒

#include "euphoria/exception.h"

#include <cassert>
#include <string>

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
