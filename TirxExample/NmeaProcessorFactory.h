#pragma once

#include "NmeaProcessor.h"

inline NmeaProcessor* CreateNmeaProcessor() {
    return new NmeaProcessor();
}
