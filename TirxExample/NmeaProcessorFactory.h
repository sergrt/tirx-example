#pragma once

#include "NmeaProcessor.h"

inline NmeaProcessor* CreateNmeaProcessor1() {
    return new NmeaProcessor();
}
