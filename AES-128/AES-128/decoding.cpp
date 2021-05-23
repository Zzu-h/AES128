#include "decoding.h"
decoding::decoding(const char* _cipher, const char* _plain, const char* _key, bool flag)
	:keys(_key, flag) {
	this->cipher_Path = _cipher;
	this->plain_Path = _plain;
	this->printFlag = flag;
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
			printFlag &&  cout << dec << round << " round" << endl;

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
		plaintext[i] = sbox.my_inv_sbox[(unsigned char)ciphertext[i]];

	if (printFlag) {
		cout << "BS: ";
		for (size_t i = 0; i < KeySize; i++)
			cout << hex << (short)ciphertext[i] << ' ';
		cout << endl;
	}

	Copy();
}
void decoding::ShiftRows() {
	char state[4][4];
	size_t idx = 0;
	for (size_t i = 0; i < 4; i++)
		for (size_t k = 0; k < 4; k++, idx++)
			state[k][i] = ciphertext[idx];

	char rotateState[4][4];
	for (size_t i = 0; i < 4; i++)
		for (size_t k = 0; k < 4; k++)
			rotateState[i][k] = state[i][(k - i + 4) % 4];

	idx = 0;
	for (size_t i = 0; i < 4; i++)
		for (size_t k = 0; k < 4; k++, idx++)
			plaintext[idx] = rotateState[k][i];

	if (printFlag) {
		cout << "SR: ";
		for (size_t i = 0; i < KeySize; i++)
			cout << hex << (short)ciphertext[i] << ' ';
		cout << endl;
	}
	Copy();
}
errno_t decoding::MixColumns() {
	// GF 사용

	char state[4][4];
	size_t idx = 0;
	for (size_t i = 0; i < 4; i++)
		for (size_t k = 0; k < 4; k++, idx++)
			state[k][i] = ciphertext[idx];

	char mixState[4][4];
	for (size_t i = 0; i < 4; i++)
		for (size_t k = 0; k < 4; k++)
			mixState[k][i] = Multiply(state[0][i], mix_col_inv_y[k][0])
			^ Multiply(state[1][i], mix_col_inv_y[k][1])
			^ Multiply(state[2][i], mix_col_inv_y[k][2])
			^ Multiply(state[3][i], mix_col_inv_y[k][3]);

	idx = 0;
	for (size_t i = 0; i < 4; i++)
		for (size_t k = 0; k < 4; k++, idx++)
			plaintext[idx] = mixState[k][i];

	if (printFlag) {
		cout << "MR: ";
		for (size_t i = 0; i < KeySize; i++)
			cout << hex << (short)ciphertext[i] << ' ';
		cout << endl;
	}

	Copy();
	return 0;
}
void decoding::AddRoundKey() {
	for (size_t i = 0; i < KeySize; i++)
		plaintext[i] = ciphertext[i] ^ curKey[i];

	if (printFlag) {
		cout << "AR: ";
		for (size_t i = 0; i < KeySize; i++)
			cout << hex << (short)ciphertext[i] << ' ';
		cout << endl;
	}

	Copy();
}

void decoding::getCurKey(int round) {
	for (size_t i = 0; i < KeySize; i++)
		curKey[i] = keys[i + KeySize * round];
}