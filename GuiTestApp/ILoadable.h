#pragma once

class ILoadable {
public:
    virtual ~ILoadable() = default;
    virtual void initialize() {}
    virtual void uninitialize() {}
};
