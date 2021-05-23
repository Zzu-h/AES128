#pragma once
#include "pch.h"

class Sbox{
private:
	uint8_t forward[8];
	uint8_t inverse[8];

	void make_Sbox();
	uint8_t get_Sbox(uint8_t);
	uint8_t get_inv_Sbox(uint8_t);

	int dgh[3];
	int deg(int);

	void bin_ext_euclid(uint8_t);
	uint8_t bin_inv(uint8_t);

public:
	Sbox();
	
	void Print_Sbox();

	uint8_t my_aes_sbox[256];
	uint8_t my_inv_sbox[256];
};