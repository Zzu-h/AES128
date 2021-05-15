#pragma once
#include "pch.h"
#include "Sbox.h"
#include "KeyExpansion.h"

class decoding
{
private:
	Sbox sbox;
	KeyExpansion keys;
	ifstream cipher;
	ofstream plain;

	const char* cipher_Path;
	const char* plain_Path;

	char ciphertext[CipherSize] = { 0 };
	char plaintext[PlainSize] = { 0 };

	errno_t binCopy();
	void Substitute();
	void ShiftRows();
	errno_t MixColumns();
	void AddRoundKey();
public:
	decoding(const char*, const char*, const char*);
	errno_t doDecoding();
};

