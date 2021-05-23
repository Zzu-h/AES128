#pragma once

#include "pch.h"
#include "Sbox.h"
/*
* Round0: key[0] - key[15]
* Round1: key[16] - key[31]
* Round2: key...
* ...
* Round10: key[160] - key[175]
*/

class KeyExpansion
{
private:
	// 전체 계산된 key 값들 저장
	Key key[KeySize * (Round + 1)];

	ifstream ReadKey;
	Sbox sbox;

	// 각 라운드마다 g-function의 결과값을 저장
	Key gValue[4];
	const char* key_path;

	// 각 round key 출력 여부 flag
	bool printFlag;

	errno_t do_KeyExpansion();
	void G_function(int);
public:
	KeyExpansion(const char*, bool);
	Key operator[](int);
};