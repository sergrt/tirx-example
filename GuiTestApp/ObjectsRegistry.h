#pragma once

#include "ILoadable.h"

#include <map>
#include <string>
#include <memory>
#include <any>
#include <stdexcept>

class ObjectsRegistry {
public:
    ObjectsRegistry();
    ~ObjectsRegistry();

    template<typename T>
    std::shared_ptr<T> findObject(const std::string& name) {
        const auto i = registry_.find(name);
        if (i == registry_.end())
            //throw std::logic_error("No object found");
            return nullptr;

        return std::dynamic_pointer_cast<T>(i->second);
    }

private:
    std::map<std::string, std::shared_ptr<ILoadable>> registry_;
};