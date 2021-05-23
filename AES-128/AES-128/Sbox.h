#pragma once
#include "pch.h"

class Sbox{
private:
	// sbox를 만들기 위한 행렬
	uint8_t forward[8];
	uint8_t inverse[8];

	// Sbox와 inverse sbox를 생성하는 메소드
	void make_Sbox();
	uint8_t get_Sbox(uint8_t);
	uint8_t get_inv_Sbox(uint8_t);

	int dgh[3];
	int deg(int);

	// GF(2)상에서 확장된 유클리드 알고리즘 구현
	void bin_ext_euclid(uint8_t);
	// GF(2)상에서 역원 구하기
	uint8_t bin_inv(uint8_t);

public:
	Sbox();
	
	void Print_Sbox();

	uint8_t my_aes_sbox[256];
	uint8_t my_inv_sbox[256];
};