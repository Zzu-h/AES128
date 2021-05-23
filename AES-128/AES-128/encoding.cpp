#include "encoding.h"
encoding::encoding(const char* _plain, const char* _cipher, const char* _key, Sbox* sb)
	:keys(_key, sb) {
	this->plain_Path = _plain;
	this->cipher_Path = _cipher;
	sbox = sb;
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
	size_t offset = 0;

	plain.seekg(offset, plain.beg);
	cipher.seekp(offset, cipher.beg);

	// end
	while(true){
		plain.read(plaintext, PlainSize);
		if(plain.eof())
			break;
		cout << "plaintext: ";
		for (size_t i = 0; i < KeySize; i++)
			cout << hex << short(plaintext[i]) << " ";
		cout << endl;
		getCurKey(0);
		AddRoundKey();

		for (size_t round = 1; round <= Round; round++) {
			getCurKey(round);

			Substitute();
			ShiftRows();
			round == Round ? true : MixColumns();
			AddRoundKey();

		}

		cout << "encrypte: ";
		for (size_t i = 0; i < KeySize; i++)
			cout << hex << short(ciphertext[i]) << " ";
		cout << endl;
		cout << "-------------------------------------------------------------" << endl;

		// cipher.bin에 ciphertext 쓰기
		cipher.write(ciphertext, CipherSize);
	}
	plain.close();
	cipher.close();

	return 0;
}

void encoding::Copy() {
	for (size_t i = 0; i < PlainSize; i++)
		plaintext[i] = ciphertext[i];
}
void encoding::Substitute() {
	// sbox 사용
	for (size_t i = 0; i < PlainSize; i++)
		ciphertext[i] = sbox->my_aes_sbox[(unsigned char)plaintext[i]];

	Copy();
}
void encoding::ShiftRows() {
	size_t idx = 0;
	for (size_t i = 0; i < 4; i++)
		for (size_t k = 0; k < 4; k++, idx++)
			ciphertext[idx] = plaintext[((k + i) % 4) + (4 * i)];
	Copy();
}
errno_t encoding::MixColumns() {
	// GF 사용
	for (size_t i = 0; i < 4; i++) 
		for (size_t k = 0; k < 4; k++)
			ciphertext[i + k * 4] = Multiply(plaintext[i + 0 * 4], mix_col_y[k][0])
			^ Multiply(plaintext[i + 1 * 4], mix_col_y[k][1])
			^ Multiply(plaintext[i + 2 * 4], mix_col_y[k][2])
			^ Multiply(plaintext[i + 3 * 4], mix_col_y[k][3]);
	
	Copy();
	return 0;
}

void encoding::AddRoundKey() {
	for (size_t i = 0; i < KeySize; i++)
		ciphertext[i] = plaintext[i] ^ curKey[i];
	Copy();
}

void encoding::getCurKey(int round) {
	for (size_t i = 0; i < KeySize; i++)
		curKey[i] = keys[i + KeySize * round];
}