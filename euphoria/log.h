// Euphoria - Copyright (c) Gustav

/** @file
Basic class for logging.
 */

#ifndef EUPHORIA_LOG_H_
#define EUPHORIA_LOG_H_

#include <string>

#include "euphoria/str.h"

namespace euphoria {

void LogInfo(const std::string& info);

#define LOGINFO(x) LogInfo(Str() << x)

}  // namespace euphoria

#endif  // EUPHORIA_LOG_H_
