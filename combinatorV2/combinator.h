#include <vector>
#include <cmath>

#include <iostream>
#include <string>

constexpr int BiL = sizeof(unsigned long long) * 8;


class CombinatorV2 {
public:
	CombinatorV2(int length, int count);

	int size();

	bool next();

	unsigned int at(int index);

	void set(int index, unsigned int value);

	std::string toString();

private:
	std::vector<unsigned long long> data;

	const int blockSize;
	const int blockCount;
	const int MAX_VALUE;

	int next_call_index = -1;
};
