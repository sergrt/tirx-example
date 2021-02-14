#include "Guid.h"

#include <random>
#include <ostream>
#include <sstream>

namespace tirx::utils {

/*
class Uuid {
public:
    Uuid() {
        static std::random_device dev;
        static std::mt19937 rng(dev());

        std::uniform_int_distribution<int> dist(0x0, 0xFF);
        for (auto& c : data_) {
            c = dist(rng);
        }
    }
    bool operator==(const Uuid& other) const {
        for (size_t i = 0; i < data_.size(); ++i) {
            if (data_[i] != other.data_[i])
                return false;
        }
        return true;
    }

    friend std::ostream& operator<<(std::ostream& stream, const Uuid& uuid) {
        std::stringstream sstream;
        sstream << std::hex;
        for (int i = 0; i < uuid.data_.size(); ++i) {
            sstream << std::hex << std::setw(2) << std::setfill('0') <<(int)uuid.data_[i];
            if (i == 3 || i == 5 || i == 7 || i == 9)
                sstream << "-";
        }
        stream << sstream.str();
        return stream;
    }

private:
    std::array<unsigned char, 16> data_;
};
*/

std::string get_uuid() {
#ifdef _DEBUG
    static int i = 1;
    return std::to_string(i++);
#endif

    static std::random_device dev;
    static std::mt19937 rng(dev());

    std::uniform_int_distribution<int> dist(0, 15);

    const char* v = "0123456789abcdef";
    const bool dash[] = { 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0 };

    std::string res;
    for (int i = 0; i < 16; i++) {
        if (dash[i]) res += "-";
        res += v[dist(rng)];
        res += v[dist(rng)];
    }
    return res;
}

}
