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

	size_t offset = 0;

	plain.seekg(offset, plain.beg);
	cipher.seekp(offset, cipher.beg);

	// plaintext ��ü�� �о�� �� ���� while���� ��� ������.
	while(true){
		plain.read(plaintext, PlainSize);
		if(plain.eof()) // plain.bin�� ���� ��� �� while���� ����
			break;

		// ��ȣȭ �� plaintext ���
		cout << "plaintext: ";
		for (size_t i = 0; i < KeySize; i++)
			cout << hex << short(plaintext[i]) << " ";
		cout << endl;

		// ���� round 0�� key�� �ް� ���� �� Addround ����
		getCurKey(0);
		AddRoundKey();

		// encrypt�� round 1���� 10���� �����Ѵ�.
		for (size_t round = 1; round <= Round; round++) {
			printFlag && cout <<dec << round<< " round" << endl;

			getCurKey(round);

			Substitute(); 												 // Substitute bytes ����
			ShiftRows();												// Shift Rows ����
			round == Round ? true : MixColumns();	   // Mix Columns ���� ��, round 10�� ���� �ʴ´�.
			AddRoundKey();											// Add Round Key ����

		}

		// ��ȣȭ �� ciphertext ���
		cout << "encrypte: ";
		for (size_t i = 0; i < KeySize; i++)
			cout << hex << short(ciphertext[i]) << " ";
		cout << endl;
		cout << "-------------------------------------------------------------" << endl;

		// cipher.bin�� ciphertext ����
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
	// sbox ����ؼ� ġȯ
	for (size_t i = 0; i < PlainSize; i++)
		ciphertext[i] = sbox.my_aes_sbox[(unsigned char)plaintext[i]];

	// BS ���
	if (printFlag) {
		cout << "BS: ";
		for (size_t i = 0; i < KeySize; i++)
			cout << hex << (short)ciphertext[i] << ' ';
		cout << endl;
	}

	Copy();
}
void encoding::ShiftRows() {
	// ���� plaintext�� ���¸� 4x4 ��ķ� ����
	char state[4][4];
	size_t idx = 0;
	for (size_t i = 0; i < 4; i++)
		for (size_t k = 0; k < 4; k++, idx++)
			state[k][i] = plaintext[idx];

	// i�࿡ ���� i��ŭ left shift����
	char rotateState[4][4];
	for (size_t i = 0; i < 4; i++)
		for (size_t k = 0; k < 4; k++)
			rotateState[i][k] = state[i][(i + k) % 4];

	// left shift���� �� �ٽ� ciphertext�� ����
	idx = 0;
	for (size_t i = 0; i < 4; i++)
		for (size_t k = 0; k < 4; k++, idx++)
			ciphertext[idx] = rotateState[k][i];

	// SR ���
	if (printFlag) {
		cout << "SR: ";
		for (size_t i = 0; i < KeySize; i++)
			cout << hex << (short)ciphertext[i] << ' ';
		cout << endl;
	}
	Copy();
}
errno_t encoding::MixColumns() {
	// ���� plaintext�� ���¸� 4x4 ��ķ� ����
	char state[4][4];
	size_t idx = 0;
	for (size_t i = 0; i < 4; i++)
		for (size_t k = 0; k < 4; k++, idx++)
			state[k][i] = plaintext[idx];

	// �� ���� ���� state�� mix column�� ���� ���(mix_col_y)�� ���ϰ� �� byte�� ����
	char mixState[4][4];
	for (size_t i = 0; i < 4; i++)
		for (size_t k = 0; k < 4; k++)
			mixState[k][i] = Multiply(state[0][i], mix_col_y[k][0])
			^ Multiply(state[1][i], mix_col_y[k][1])
			^ Multiply(state[2][i], mix_col_y[k][2])
			^ Multiply(state[3][i], mix_col_y[k][3]);

	// MixColumns���� �� �ٽ� ciphertext�� ����
	idx = 0;
	for (size_t i = 0; i < 4; i++)
		for (size_t k = 0; k < 4; k++, idx++)
			ciphertext[idx] = mixState[k][i];

	// MR���
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
	// ���� Key�� plaintext�� xor���� �����Ѵ�.
	for (size_t i = 0; i < KeySize; i++)
		ciphertext[i] = plaintext[i] ^ curKey[i];

	// AR���
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