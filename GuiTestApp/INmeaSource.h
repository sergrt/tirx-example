#pragma once

#include "Observable.h"

#include <string>

class INmeaSource {
public:
    virtual ~INmeaSource() = default;
    virtual std::string get_next_message() const = 0;
    virtual tirx::Observable<std::string>* get_sentences_stream() = 0;
};
