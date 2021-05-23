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
	Key key[KeySize * (Round + 1)];
	ifstream ReadKey;
	Sbox* sbox;

	Key gValue[4];
	const char* key_path;

	errno_t do_KeyExpansion();
	void G_function(int);
public:
	KeyExpansion(const char*, Sbox*);
	Key operator[](int);
};