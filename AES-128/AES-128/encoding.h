#pragma once
#include <fstream>
#include "Sbox.h"
#include "KeyExpansion.h"
#define TotalRound 10

class encoding
{
private:
	Sbox sbox;
	KeyExpansion keys;
	ifstream plain;
	ofstream cipher;

	const char* plain_Path;
	const char* cipher_Path;

	// char input[MAX_LEN + 1] = { 0 };

	errno_t binCopy();
	void Substitute();
	void ShiftRows();
	void MixColumns();
	void AddRoundKey();
public:
	encoding(const char*, const char*, const char*);
	errno_t doEncoding();
};

