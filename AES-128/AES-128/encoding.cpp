#include "encoding.h"
encoding::encoding(const char* _plain, const char* _cipher, const char* _key, bool flag)
	:keys(_key, flag) {
	this->plain_Path = _plain;
	this->cipher_Path = _cipher;
	this->printFlag = flag;
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
			printFlag && cout <<dec << round<< " round" << endl;

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
		ciphertext[i] = sbox.my_aes_sbox[(unsigned char)plaintext[i]];

	if (printFlag) {
		cout << "BS: ";
		for (size_t i = 0; i < KeySize; i++)
			cout << hex << (short)ciphertext[i] << ' ';
		cout << endl;
	}

	Copy();
}
void encoding::ShiftRows() {
	char state[4][4];
	size_t idx = 0;
	for (size_t i = 0; i < 4; i++)
		for (size_t k = 0; k < 4; k++, idx++)
			state[k][i] = plaintext[idx];

	char rotateState[4][4];
	for (size_t i = 0; i < 4; i++)
		for (size_t k = 0; k < 4; k++)
			rotateState[i][k] = state[i][(i + k) % 4];

	idx = 0;
	for (size_t i = 0; i < 4; i++)
		for (size_t k = 0; k < 4; k++, idx++)
			ciphertext[idx] = rotateState[k][i];

	if (printFlag) {
		cout << "SR: ";
		for (size_t i = 0; i < KeySize; i++)
			cout << hex << (short)ciphertext[i] << ' ';
		cout << endl;
	}
	Copy();
}
errno_t encoding::MixColumns() {

	char state[4][4];
	size_t idx = 0;
	for (size_t i = 0; i < 4; i++)
		for (size_t k = 0; k < 4; k++, idx++)
			state[k][i] = plaintext[idx];

	char mixState[4][4];
	for (size_t i = 0; i < 4; i++)
		for (size_t k = 0; k < 4; k++)
			mixState[k][i] = Multiply(state[0][i], mix_col_y[k][0])
			^ Multiply(state[1][i], mix_col_y[k][1])
			^ Multiply(state[2][i], mix_col_y[k][2])
			^ Multiply(state[3][i], mix_col_y[k][3]);

	idx = 0;
	for (size_t i = 0; i < 4; i++)
		for (size_t k = 0; k < 4; k++, idx++)
			ciphertext[idx] = mixState[k][i];

	if (printFlag) {
		cout << "MR: ";
		for (size_t i = 0; i < KeySize; i++)
			cout << hex << (short)ciphertext[i] << ' ';
		cout << endl;
	}

	Copy();
	return 0;
}
void encoding::AddRoundKey() {
	for (size_t i = 0; i < KeySize; i++)
		ciphertext[i] = plaintext[i] ^ curKey[i];

	if (printFlag) {
		cout << "AR: ";
		for (size_t i = 0; i < KeySize; i++)
			cout << hex << (short)ciphertext[i] << ' ';
		cout << endl;
	}

	Copy();
}

void encoding::getCurKey(int round) {
	for (size_t i = 0; i < KeySize; i++)
		curKey[i] = keys[i + KeySize * round];
}