// Charles Baxter
// FibLFSR.h for PS1a
// created on 9/20/2021

#ifndef FibLFSR_h
#define FibLFSR_h

#include <string>
#include <iostream>

class FibLFSR
{
public:
	FibLFSR(std::string seed);

	int step();
	int generate(int k);

	friend std::ostream& operator<<(std::ostream& out, const FibLFSR& f);

private:
	std::string buffer;
	const int taps[4] = { 0, 2, 3, 5 };
};

#endif