#pragma once
#include "pch.h"
#include "Sbox.h"
#include "KeyExpansion.h"

class encoding
{
private:
	ifstream plain;
	ofstream cipher;

	// 확장된 key와 sbox를 생성
	KeyExpansion keys;
	Sbox sbox;

	// plaintext와 ciphertext 경로 저장
	const char* plain_Path;
	const char* cipher_Path;

	// 각 라운드마다 해당하는 key를 저장
	Key curKey[KeySize];

	char plaintext[PlainSize] = { 0 };
	char ciphertext[CipherSize] = { 0 };

	// 암호화 과정 출력 여부 flag
	bool printFlag;

	// 각 라운드마다 plaintext를 ciphertext로 변환하기 위해 변환된 ciphertext를 plaintext로 복사
	void Copy();

	void Substitute();			// Substitute bytes 수행 
	void ShiftRows();				// Shift Rows 수행
	errno_t MixColumns();	// Mix Columns 수행
	void AddRoundKey();		// Add Round Key 수행

	// 각 라운드에 해당하는 key를 계산해서 curKey에 저장
	void getCurKey(int);
public:
	encoding(const char*, const char*, const char*, bool);
	errno_t doEncoding();
};

