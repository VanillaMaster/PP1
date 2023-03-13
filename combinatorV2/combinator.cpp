#include "combinator.h"

CombinatorV2::CombinatorV2(int length, int count) : blockCount(length), blockSize(std::ceil(std::log2(count))), MAX_VALUE(count - 1) {
    const int data_length = std::ceil(blockSize * blockCount / (double)BiL);
    data.resize(data_length);
    std::fill(data.begin(), data.end(), 0);
};

int CombinatorV2::size() {
    return this->blockCount;
}

bool CombinatorV2::next() {
    if (this->MAX_VALUE == 0) return true;
    bool overflow = true;

    for (int i = 0; i < this->blockCount; i++) if (overflow) {
        unsigned int val = this->at(i);
        val++;
        overflow = (val > this->MAX_VALUE);
        if (overflow) val = 0u;
        this->set(i, val);
    }

    return overflow;
};

unsigned int CombinatorV2::at(int index) {
    if (this->MAX_VALUE == 0 && index < blockCount) { return 0; }

    const int position = index * blockSize;
    const int block = position / BiL;
    const int shift = position % BiL;

    const int SLShift = BiL - (blockSize + shift);

    const int RLShift = std::max(SLShift, 0); // Remaining Left Shift
    const int ORShift = std::max(-SLShift, 0);// Overflow Right Shift

    const unsigned long LMask = ((~0UL) << (shift + RLShift)) >> RLShift;// Lower Mask

    const unsigned int lowerBits = (data.at(block) & LMask) >> shift;

    if (ORShift == 0) return lowerBits;

    const unsigned long HMask = (~0UL) >> (BiL - ORShift);// Higher Mask

    const unsigned int higherBits = (data.at(block + 1) & HMask) << (blockSize - ORShift);

    return higherBits | lowerBits;
};

void CombinatorV2::set(int index, unsigned int value) {
    const int position = index * blockSize;
    const int block = position / BiL;
    const int shift = position % BiL;

    const int SLShift = BiL - (blockSize + shift);

    const int RLShift = std::max(SLShift, 0);
    const int ORShift = std::max(-SLShift, 0);

    const unsigned long LRMask = ~(((~0UL) << (shift + RLShift)) >> RLShift);
    const unsigned long LSMask = ((unsigned long)value) << shift;

    data.at(block) &= LRMask;
    data.at(block) |= LSMask;

    if (ORShift == 0) return;

    const int LBLen = blockSize - ORShift;

    const unsigned long HRMask = (~0UL) << LBLen;
    const unsigned long HSMask = ((unsigned long)value) >> LBLen;

    const int nextBlock = block + 1;

    data.at(nextBlock) &= HRMask;
    data.at(nextBlock) |= HSMask;

};