#include "combinator.h"

CombinatorV3::CombinatorV3(int length, int count, std::string prefix) : MAX_VALUE(count - 1), prefix(prefix) {
    state.resize(length);
    limit.resize(length);
    std::fill(state.begin(), state.end(), 0);
    std::fill(limit.begin(), limit.end(), count);
};

int CombinatorV3::size() {
    return state.size();
}

bool CombinatorV3::next() {
    if (this->MAX_VALUE == 0) return true;
    bool overflow = true;
    bool overlap = true;

    for (int i = 0; (i < this->state.size()) && (overflow || overlap); i++) {
        unsigned int val = this->at(i);
        if (overlap) overlap = (val == this->limit.at(i));
        if (overflow) {
            val++;
            overflow = (val > this->MAX_VALUE);
            if (overflow) val = 0u;
            this->set(i, val);
        }
    }

    if (++(this->next_call_index) % 10000000 == 0) {
        std::string res = "";
        for (int i = 0; i < this->size(); i++) {
            res += std::to_string(this->at(i));
        }
        std::cout << prefix + res + "::" + std::to_string(this->next_call_index) + "\n";
    }

    return overlap;
}

std::string CombinatorV3::toString() {
    std::string res = "";
    for (int i = 0; i < this->size(); i++) {
        res += std::to_string(this->at(i));
    }
    return res;
}

unsigned int CombinatorV3::at(int index) {
    if (this->MAX_VALUE == 0 && index < state.size()) {
        return 0;
    }
    return state.at(index);
}

void CombinatorV3::set(int index, unsigned int value) {
    if (this->MAX_VALUE == 0 && index < state.size()) {
        return;
    }
    state.at(index) = value;
}

void CombinatorV3::setRange(std::vector<unsigned int>& self, int partsCount, int partIndex) {
    const double startTarget = (double)partIndex / (double)partsCount;

    const double bitDelta = 1.0 / (double)(this->MAX_VALUE + 1);

    double rangeStart = 0.0;
    double rangeEnd = 1.0;

    for (int i = this->state.size() - 1; i >= 0; i--) {
        const double r = (rangeEnd - rangeStart) * bitDelta;
        const double t = startTarget - rangeStart;
        const int bitValue = std::clamp((int)std::trunc(t / r), 0, MAX_VALUE);
        rangeEnd = rangeStart + (r * (bitValue + 1));
        rangeStart += r * bitValue;
        self.at(i) = bitValue;
    }
}

void CombinatorV3::useRange(int partsCount, int partIndex) {
    setRange(this->state, partsCount, partIndex);
    setRange(this->limit, partsCount, partIndex + 1);
}
