#include "Sbox.h"

Sbox::Sbox() {
	for(auto i = 0; i<16; i++)
		for (auto k = 0; k < 16; k++) {
			my_inv_sbox[i][k] = stand_inv_sbox[i][k];
			my_sbox[i][k] = stand_aes_sbox[i][k];
		}
}
