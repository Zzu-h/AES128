#include "KeyExpansion.h"

KeyExpansion::KeyExpansion(const char* _path){
	this->key_path = _path;
	if (do_KeyExpansion())
		cout << "Failed to Read the Key" << endl;
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

	// key.bin�� �а� key[0] ~ key[15] ���� byte������ ����
	ReadKey.seekg(0, ReadKey.beg);
	ReadKey.read(key, KeySize);
	ReadKey.close();
	// end

	size_t curIdx = 16;
	for (size_t round = 0; round < Round; round++) {
		G_function(round);
		for (size_t w0 = 0; w0 < 4; w0++, curIdx++)
			key[curIdx] = (key[curIdx - 16] ^ gValue[w0]); //XOR
		for (size_t w = 0; w < 12; w++, curIdx++)
			key[curIdx] = (key[curIdx - 16] ^ key[curIdx - 4]); //XOR

	}

	return 0;
}

void KeyExpansion::G_function(int round) {
	int idx = 12 + (KeySize * round);

	// left shift
	for (size_t i = 0; i < 4; i++, idx++)
		gValue[(3+i)%4] = key[idx];

	// sbox ���� �� ���


	// RCj XOR
	char RCj = 1;
	if (round >= 8) {
		// Ireducible �������� ������ Mod ���� �ʿ�
	}
	else
		RCj = RCj << round;

	 gValue[0] ^= RCj;
}