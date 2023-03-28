#include <vector>
#include <cmath>
#include <algorithm>

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

	unsigned long long next_call_index = -1;
};

class CombinatorV3 {
public:
	CombinatorV3(int length, int count, std::string prefix = "");

	int size();

	bool next();

	unsigned int at(int index);

	void set(int index, unsigned int value);

	void useRange(int partsCount, int partIndex);

	std::string toString();
private:

	void setRange(std::vector<unsigned int>& self, int partsCount, int partIndex);

	std::vector<unsigned int> state;
	std::vector<unsigned int> limit;

	const int MAX_VALUE;

	unsigned long long next_call_index = -1;

	const std::string prefix;
};