// Charles Baxter
// FibLFSR.cpp for PS1a
// created on 9/20/2021

#include "FibLFSR.h"

FibLFSR::FibLFSR(std::string seed)
	: buffer{ seed } {}

int FibLFSR::step()
{
	int i = 0;
	char carry = buffer[taps[i]];
	// implementation of xor operation over each tap bit
	for (i = 1; i < 4; i++)
		carry = (carry - buffer[taps[i]] == 0) ? '0' : '1';

	// remove the first bit, then append with the carry bit
	buffer.erase(0, 1);
	buffer += carry;

	return carry - '0';
}

int FibLFSR::generate(int k)
{
	// start as 0
	int retVal = 0;
	for (int i = 0; i < k; i++) {
		// this is the same as moving each bit to the left by 1, or << 1
		// the new bit added will be 0
		retVal *= 2;
		// adding the step to the new bit of 0 will assign it to whatever step returns
		retVal += this->step();
	}
	return retVal;
}

std::ostream& operator<<(std::ostream& out, const FibLFSR& f) {
	for (int i = 0; i < int(f.buffer.size()); i++) {
		out << f.buffer[i];
	}
	return out;
}