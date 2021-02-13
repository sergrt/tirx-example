#pragma once

#include "ObjectsRegistry.h"

extern ObjectsRegistry registry;

template<typename T>
std::shared_ptr<T> findObject(const std::string& name) {
    return registry.findObject<T>(name);
}