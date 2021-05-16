#include "decoding.h"

decoding::decoding(const char* _cipher, const char* _plain, const char* _key)
	:keys(_key) {
	this->cipher_Path = _cipher;
	this->plain_Path = _plain;
}

errno_t decoding::doDecoding() {

	cipher.open(cipher_Path, ios::in | ios::binary);
	if (!cipher.is_open()) {
		cout << "Cipher bin File was not open. \nPlease Check the ciphertext" << endl;
		plain.close();
		return 1;
	}
	plain.open(plain_Path, ios::out | ios::binary);
	if (!plain.is_open()) {
		cout << "Plain bin file had error." << endl;
		return 1;
	}


	// plain.bin을 읽고 plain[0] ~ plain[15] 까지 byte단위로 저장
	cipher.seekg(0, cipher.beg);
	cipher.read(ciphertext, CipherSize);
	cipher.close();
	// end

	AddRoundKey();

	for (size_t round = 1; round <= Round; round++) {
		ShiftRows();
		Substitute();
		AddRoundKey();
		(round == 10) ? true : MixColumns();
	}

	// cipher.bin에 ciphertext 쓰기
	plain.seekp(0, plain.beg);
	plain.write(plaintext, PlainSize);
	plain.close();

	return 0;
}

errno_t decoding::binCopy() {
	return 0;
}
void decoding::Substitute() {

}
void decoding::ShiftRows() {

}
errno_t decoding::MixColumns() {
	return 0;
}
void decoding::AddRoundKey() {

}