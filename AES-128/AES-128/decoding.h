#pragma once
#include "pch.h"
#include "Sbox.h"
#include "KeyExpansion.h"

class decoding
{
private:
	ifstream cipher;
	ofstream plain;

	// 확장된 key와 sbox를 생성
	KeyExpansion keys;
	Sbox sbox;

	// plaintext와 ciphertext 경로 저장
	const char* cipher_Path;
	const char* plain_Path;

	// 각 라운드마다 해당하는 key를 저장
	Key curKey[KeySize]; 

	char ciphertext[CipherSize] = { 0 };
	char plaintext[PlainSize] = { 0 };

	// 복호화 과정 출력 여부 flag
	bool printFlag;

	// 각 라운드마다 ciphertext를 plaintext로 변환하기 위해 변환된 plaintext를 ciphertext로 복사
	void Copy(); 

	void Substitute();			//Inverse  Substitute bytes 수행 
	void ShiftRows();				// Inverse Shift Rows 수행
	errno_t MixColumns();	// Inverse Mix Columns 수행
	void AddRoundKey();		// Add Round Key 수행

		// 각 라운드에 해당하는 key를 계산해서 curKey에 저장
	void getCurKey(int);
public:
	decoding(const char*, const char*, const char*, bool);
	errno_t doDecoding();
};