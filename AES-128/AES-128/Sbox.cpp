#include "Sbox.h"

Sbox::Sbox() {
	for(auto i = 0; i<256; i++){
			this->aes_sbox[i] = stand_aes_sbox[i];
			this->inv_sbox[i]= stand_inv_sbox[i];
	}
	cout << "--------------------------------------" << endl;
	for (auto i = 0; i < 16; i++) {
		for (auto k = 0; k < 16; k++)
			cout << hex << (short)this->aes_sbox[k + i*16] << ' ';
		cout << endl;
	}
	cout << endl << endl;
	for (auto i = 0; i < 16; i++) {
		for (auto k = 0; k < 16; k++)
			cout << hex << (short)stand_aes_sbox[k + i * 16] << ' ';
		cout << endl;
	}
	cout << "--------------------------------------" << endl;
	//memcpy(aes_sbox, stand_aes_sbox, 256);
	//memcpy(inv_sbox, stand_inv_sbox, 256);
}
