#pragma once
#include "pch.h"
#include "Sbox.h"
#include "KeyExpansion.h"

class encoding
{
private:
	ifstream plain;
	ofstream cipher;

	// Ȯ��� key�� sbox�� ����
	KeyExpansion keys;
	Sbox sbox;

	// plaintext�� ciphertext ��� ����
	const char* plain_Path;
	const char* cipher_Path;

	// �� ���帶�� �ش��ϴ� key�� ����
	Key curKey[KeySize];

	char plaintext[PlainSize] = { 0 };
	char ciphertext[CipherSize] = { 0 };

	// ��ȣȭ ���� ��� ���� flag
	bool printFlag;

	// �� ���帶�� plaintext�� ciphertext�� ��ȯ�ϱ� ���� ��ȯ�� ciphertext�� plaintext�� ����
	void Copy();

	void Substitute();			// Substitute bytes ���� 
	void ShiftRows();				// Shift Rows ����
	errno_t MixColumns();	// Mix Columns ����
	void AddRoundKey();		// Add Round Key ����

	// �� ���忡 �ش��ϴ� key�� ����ؼ� curKey�� ����
	void getCurKey(int);
public:
	encoding(const char*, const char*, const char*, bool);
	errno_t doEncoding();
};

