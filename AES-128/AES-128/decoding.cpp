#include "decoding.h"

decoding::decoding(const char* _cipher, const char* _plain, const char* _key, int version)
	:keys(_key, version) {
	this->cipher_Path = _cipher;
	this->plain_Path = _plain;
	this->ver = version;
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

	size_t offset = 0;

	cipher.seekg(offset, cipher.beg);
	plain.seekp(offset, plain.beg);

	// cipher.bin을 읽고 cipher[0] ~ cipher[15] 까지 byte단위로 저장

	// end
	while(true) {
		cipher.read(ciphertext, CipherSize);
		if (cipher.eof())
			break;
		cout << "ciphertext: ";
		for (size_t i = 0; i < KeySize; i++)
			cout << hex << short(ciphertext[i]) << " ";
		cout << endl;

		getCurKey(10);
		AddRoundKey();

		for (int round = Round - 1; round >= 0; round--) {
			getCurKey(round);

			ShiftRows();
			Substitute();
			AddRoundKey();
			round == 0 ? true : MixColumns();
		}

		cout << "decrypt: ";
		for (size_t i = 0; i < KeySize; i++)
			cout << hex << short(plaintext[i]) << " ";
		cout << endl;
		cout << "-------------------------------------------------------------" << endl;
		// plain.bin에 plaintext 쓰기
		plain.write(plaintext, PlainSize);
	}

	plain.close();
	cipher.close();

	return 0;
}

void decoding::Copy() {
	for (size_t i = 0; i < CipherSize; i++)
		ciphertext[i] = plaintext[i];
}
void decoding::Substitute() {
	// sbox 사용
	for (size_t i = 0; i < CipherSize; i++)
		plaintext[i] = inv_sbox[(unsigned char)ciphertext[i]];
	
	Copy();
}
void decoding::ShiftRows() {
	size_t idx = 0;
	for (size_t i = 0; i < 4; i++)
		for (size_t k = 0; k < 4; k++, idx++)
			plaintext[idx] = ciphertext[((k - i + 4) % 4) + (4 * i)];
	Copy();
}
errno_t decoding::MixColumns() {
	// GF 사용
	for (size_t i = 0; i < 4; i++)
		for (size_t k = 0; k < 4; k++)
			plaintext[i + k * 4] = Multiply(mix_col_inv_y[k][0], ciphertext[i + (0 * 4)])
			^ Multiply(mix_col_inv_y[k][1], ciphertext[i + (1 * 4)])
			^ Multiply(mix_col_inv_y[k][2], ciphertext[i + (2 * 4)])
			^ Multiply(mix_col_inv_y[k][3], ciphertext[i + (3 * 4)]);

	Copy();
	return 0;
}
void decoding::AddRoundKey() {
	for (size_t i = 0; i < KeySize; i++)
		plaintext[i] = ciphertext[i] ^ curKey[i];
	Copy();
}
void decoding::getCurKey(int round) {
	for (size_t i = 0; i < KeySize; i++)
		curKey[i] = keys[i + KeySize * round];
}