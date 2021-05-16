#pragma once
#include "pch.h"

class Sbox
{
private:
public:
	uint8_t my_sbox[16][16];
	uint8_t my_inv_sbox[16][16];
	Sbox();
};