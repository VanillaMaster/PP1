#include <vector>
#include <cmath>

constexpr int BiL = sizeof(long) * 8;

class CombinatorV2 {
public:
	CombinatorV2(int length, int count);

	int size();

	bool next();

	unsigned int at(int index);

	void set(int index, unsigned int value);

private:
	std::vector<unsigned long> data;

	const int blockSize;
	const int blockCount;
	const int MAX_VALUE;
};
