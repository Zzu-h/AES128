#pragma once
#include "pch.h"

class Sbox
{
private:
public:
	uint8_t aes_sbox[256];
	uint8_t inv_sbox[256];
	Sbox();
};