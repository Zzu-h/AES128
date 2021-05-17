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

	Key curKey[KeySize];
	char ciphertext[CipherSize] = { 0 };
	char plaintext[PlainSize] = { 0 };

	bool ver = polynomial_standard;

	void Copy();
	void Substitute();
	void ShiftRows();
	errno_t MixColumns();
	char Scalar(int, char*);
	void AddRoundKey();
	void getCurKey(int);
public:
	decoding(const char*, const char*, const char*);
	errno_t doDecoding();
};

