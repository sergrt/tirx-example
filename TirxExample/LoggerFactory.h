#pragma once
#include "Logger.h"

#include <memory>

inline std::shared_ptr<ILogger> CreateLogger() {
    return std::make_shared<Logger>();
}

inline Logger* CreateLogger1() {
    return new Logger();
}
