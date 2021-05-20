#include "KeyExpansion.h"
#include <bitset>

KeyExpansion::KeyExpansion(const char* _path, int version){
	this->key_path = _path;
	if (do_KeyExpansion())
		cout << "Failed to Read the Key" << endl;
	this->ver = version;
}
Key KeyExpansion::operator[](int index) {
	return key[index];
}

errno_t KeyExpansion::do_KeyExpansion() {
	ReadKey.open(key_path, ios::in | ios::binary);

	if (!ReadKey.is_open()) {
		cout << "Key bin File was not open. \nPlease Check the Key" << endl;
		return 1;
	}

	// key.bin을 읽고 key[0] ~ key[15] 까지 byte단위로 저장
	ReadKey.seekg(0, ReadKey.beg);
	ReadKey.read(key, KeySize);
	ReadKey.close();
	// end

	size_t curIdx = 16;
	for (size_t round = 1; round <= Round; round++) {
		G_function(round-1);

		for (size_t w0 = 0; w0 < 4; w0++)
			key[round*KeySize + w0*4] = (key[(round-1) * KeySize + w0 * 4] ^ gValue[w0]); //XOR

		for (size_t i = 1; i < 4; i++)
			for (size_t k = 0; k < 4; k++)
				key[round * KeySize + i+k*4] = (key[(round-1) * KeySize + i + k * 4] ^ key[round * KeySize + i-1 + k * 4]); //XOR

	}
	for (auto r = 0; r <= Round; r++) {
		cout << "round " << r << ": ";
		for (auto i = 0; i < KeySize; i++)
			cout << hex << (short)key[r*KeySize + i] << ' ';
		cout << endl;
	}
	cout << "-------------------------------------------------------------" << endl;
	return 0;
}

void KeyExpansion::G_function(int round) {
	int idx = 3 + (KeySize * round);

	// left shift
	for (size_t i = 0; i < 4; i++)
		gValue[(3+i)%4] = key[idx + i*4];

	// sbox 구현 후 통과
	for (size_t i = 0; i < 4; i++) 
		gValue[i] = aes_sbox[gValue[i]];

	// RCj XOR
	uint8_t RCj[4] = { 0x01, 0, 0, 0 };
	if (round >= 8) {
		// Ireducible 방정식을 가지고 Mod 연산 필요
		RCj[0] = polynomial[ver];
		RCj[0] = RCj[0] << (round-8);
	}
	else
		RCj[0] = RCj[0] << round;

	for (size_t i = 0; i < 4; i++)
		 gValue[i] ^= RCj[i];
}