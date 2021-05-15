#include "encoding.h"

encoding::encoding(const char* _plain, const char* _cipher, const char* _key)
	:keys(_key) {
	this->plain_Path = _plain;
	this->cipher_Path = _cipher;
}

errno_t encoding::doEncoding() {

	plain.open(plain_Path, ios::in | ios::binary);
	if (!plain.is_open()) {
		cout << "Plain bin File was not open. \nPlease Check the plaintext" << endl;
		return 1;
	}
	
	cipher.open(cipher_Path, ios::out | ios::binary);
	if (!cipher.is_open()) {
		cout << "cipher bin file had error." << endl;
		plain.close();
		return 1;
	}

	// plain.bin을 읽고 plain[0] ~ plain[15] 까지 byte단위로 저장
	plain.seekg(0, plain.beg);
	plain.read(plaintext, PlainSize);
	plain.close();
	// end

	getCurKey(0);
	AddRoundKey();

	for (size_t round = 1; round <= Round; round++) {
		getCurKey(round);

		// 암호화
		Substitute();
		ShiftRows();
		(round == (Round)) ? true : MixColumns();
		AddRoundKey();
	}

	// cipher.bin에 ciphertext 쓰기
	cipher.seekp(0, cipher.beg);
	cipher.write(ciphertext, CipherSize);
	cipher.close();

	return 0;
}

errno_t encoding::binCopy() {
	return 0;
}
void encoding::Substitute() {
	// sbox 사용
}
void encoding::ShiftRows() {

}
errno_t encoding::MixColumns() {
	// GF 사용
}
void encoding::AddRoundKey() {

}

void encoding::getCurKey(int round) {
	for (size_t i = 0; i < KeySize; i++)
		curKey[i] = keys[i + KeySize * round];
}