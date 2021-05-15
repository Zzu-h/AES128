#pragma once
#include "pch.h"
#include "Sbox.h"
#include "KeyExpansion.h"

class encoding
{
private:
	Sbox sbox;
	KeyExpansion keys;
	ifstream plain;
	ofstream cipher;

	const char* plain_Path;
	const char* cipher_Path;

	Key curKey[KeySize];
	char plaintext[PlainSize] = { 0 };
	char ciphertext[CipherSize] = { 0 };

	errno_t binCopy();
	void Substitute();
	void ShiftRows();
	errno_t MixColumns();
	void AddRoundKey();
	void getCurKey(int);
public:
	encoding(const char*, const char*, const char*);
	errno_t doEncoding();
};

