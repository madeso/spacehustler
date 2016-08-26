// Euphoria - Copyright (c) Gustav

#include <SDL_log.h>
#include "euphoria/log.h"

namespace euphoria {

void LogError(const std::string& info) {
  SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", info.c_str());
}

void LogDebug(const std::string& info) {
  SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "%s", info.c_str());
}

void LogCritical(const std::string& info) {
  SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "%s", info.c_str());
}

void LogInfo(const std::string& info) {
  SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "%s", info.c_str());
}

}  // namespace euphoria
