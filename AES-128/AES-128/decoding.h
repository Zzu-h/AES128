#pragma once
#include "pch.h"
#include "Sbox.h"
#include "KeyExpansion.h"

class decoding
{
private:
	ifstream cipher;
	ofstream plain;

	// Ȯ��� key�� sbox�� ����
	KeyExpansion keys;
	Sbox sbox;

	// plaintext�� ciphertext ��� ����
	const char* cipher_Path;
	const char* plain_Path;

	// �� ���帶�� �ش��ϴ� key�� ����
	Key curKey[KeySize]; 

	char ciphertext[CipherSize] = { 0 };
	char plaintext[PlainSize] = { 0 };

	// ��ȣȭ ���� ��� ���� flag
	bool printFlag;

	// �� ���帶�� ciphertext�� plaintext�� ��ȯ�ϱ� ���� ��ȯ�� plaintext�� ciphertext�� ����
	void Copy(); 

	void Substitute();			//Inverse  Substitute bytes ���� 
	void ShiftRows();				// Inverse Shift Rows ����
	errno_t MixColumns();	// Inverse Mix Columns ����
	void AddRoundKey();		// Add Round Key ����

		// �� ���忡 �ش��ϴ� key�� ����ؼ� curKey�� ����
	void getCurKey(int);
public:
	decoding(const char*, const char*, const char*, bool);
	errno_t doDecoding();
};