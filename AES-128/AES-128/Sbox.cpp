#include "Sbox.h"

Sbox::Sbox() {
	// sbox 계산을 위한 행렬 선계산
	forward[0] = 0xf1;
	inverse[0] = 0xA4;
	for (size_t i = 1; i < 8; i++) {
		forward[i] = (forward[i - 1] << 1) | (forward[i - 1] >> 7);		 // left shift
		inverse[i] = (inverse[i - 1] << 1) | (inverse[i - 1] >> 7);		// left shift
	}
	make_Sbox();
}
uint8_t Sbox::get_Sbox(uint8_t e){
	uint8_t inv_e = bin_inv(e);// 입력받은 요소의 역원 구하기
	
	uint8_t temp = 0x00, c = 0x63;											
	uint8_t b_Matrix_Cal[8] = { 0, };
	uint8_t b_output[8] = { 0, };

	// sbox 연산 중 행렬 연산
	for (size_t k = 0; k < 8; k++)
		b_Matrix_Cal[k] = (forward[k] & inv_e);
	for (size_t k = 0; k < 8; k++) {
		for (size_t t = 0; t < 8; t++)
			temp ^= ((b_Matrix_Cal[k] >> t) & 0x01);

		temp ^= ((c >> k) & 0x01);
		b_output[k] = temp;
		temp = 0x00;
	}

	// 행렬 연산결과와 0x63 상수 XOR연산
	for (size_t k = 0; k < 8; k++)
		temp |= (b_output[k] << k);

	// 연산된 결과 값 리턴
	return temp;
}
uint8_t Sbox::get_inv_Sbox(uint8_t d) {
	uint8_t temp = 0x00, c = 0x05;
	uint8_t b_Matrix_Cal[8] = { 0, };
	uint8_t b_output[8] = { 0, };

	// Inverse sbox 연산 중 행렬 연산
	for (size_t k = 0; k < 8; k++)
		b_Matrix_Cal[k] = (inverse[k] & d);
	for (size_t k = 0; k < 8; k++) {
		for (size_t t = 0; t < 8; t++)
			temp ^= ((b_Matrix_Cal[k] >> t) & 0x01);

		temp ^= ((c >> k) & 0x01);
		b_output[k] = temp;
		temp = 0x00;
	}

	// 행렬 연산결과와 0x63 상수의 역원 0x05 XOR연산
	for (size_t k = 0; k < 8; k++)
		temp |= (b_output[k] << k);

	// 연산된 요소의 역원 구하고 리턴
	return bin_inv(temp);
}
void Sbox::make_Sbox() {
	for (size_t i = 0; i < 256; i++) {
		my_aes_sbox[i] = get_Sbox(i);
		my_inv_sbox[i] = get_inv_Sbox(i);
	}
}

int Sbox::deg(int bp) {
	for (int i = 31; i >= 0; i--)
		if ((bp & (1 << i)) != 0)
			return i;
	return 0;
}
void Sbox::bin_ext_euclid(uint8_t a){
	int b = (0x100 | polynomial[ver]);
	int u = a, v = b;
	int g[2] = { 1, 0 };
	int h[2] = { 0, 1 };

	while (u != 0){
		int j = (deg(u) - deg(v));
		if (j < 0) {
			swap(u, v);
			swap(g[0], g[1]);
			swap(h[0], h[1]);
			j = -j;
		}
		u ^= v << j;
		g[0] ^= g[1] << j;
		h[0] ^= h[1] << j;
	}
	dgh[0] = v;
	dgh[1] = g[1];
	dgh[2] = h[1];
}
uint8_t Sbox::bin_inv(uint8_t a){
	bin_ext_euclid(a);
	return dgh[1];
}

void Sbox::Print_Sbox() {
	// Sbox 연산 결과 출력
	cout << "--------------------------------------\nsbox" << endl;
	for (auto i = 0; i < 16; i++) {
		for (auto k = 0; k < 16; k++)
			cout << hex << (short)this->my_aes_sbox[k + i * 16] << ' ';
		cout << endl;
	}
	cout << "\ninverse_sbox" << endl;
	for (auto i = 0; i < 16; i++) {
		for (auto k = 0; k < 16; k++)
			cout << hex << (short)my_inv_sbox[k + i * 16] << ' ';
		cout << endl;
	}
	cout << "--------------------------------------" << endl;
}