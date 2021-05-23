#pragma once
#include "pch.h"
#include "Sbox.h"
#include "KeyExpansion.h"

class decoding
{
private:
	KeyExpansion keys;
	ifstream cipher;
	ofstream plain;
	Sbox sbox;

	const char* cipher_Path;
	const char* plain_Path;

	Key curKey[KeySize];
	char ciphertext[CipherSize] = { 0 };
	char plaintext[PlainSize] = { 0 };

	bool printFlag;

	void Copy();
	void Substitute();
	void ShiftRows();
	errno_t MixColumns();
	void AddRoundKey();
	void getCurKey(int);
public:
	decoding(const char*, const char*, const char*, bool);
	errno_t doDecoding();
};