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
void LogError(const std::string& info);
void LogDebug(const std::string& info);
void LogCritical(const std::string& info);

#define LOGINFO(x) LogInfo(Str() << x)
#define LOGERROR(x) LogInfo(Str() << x)
#define LOGDEBUG(x) LogInfo(Str() << x)
#define LOGCRITICAL(x) LogInfo(Str() << x)

}  // namespace euphoria

#endif  // EUPHORIA_LOG_H_
