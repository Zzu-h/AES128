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

	// ciphertext 전체를 읽어올 때 까지 while문을 계속 돌린다.
	while(true) {
		cipher.read(ciphertext, CipherSize);
		if (cipher.eof()) // cipher.bin의 끝일 경우 이 while문을 끝냄
			break;

		// 복호화 전 ciphertext 출력
		cout << "ciphertext: ";
		for (size_t i = 0; i < KeySize; i++)
			cout << hex << short(ciphertext[i]) << " ";
		cout << endl;

		// 현재 round 10의 key를 받고 저장 및 Addround 수행
		getCurKey(10);
		AddRoundKey();

		// decrypt는 round 9부터 0까지 거꾸로 진행한다.
		for (int round = Round - 1; round >= 0; round--) {
			printFlag &&  cout << dec << round << " round" << endl;

			getCurKey(round);

			ShiftRows();								    // Inverse Shift Rows 수행
			Substitute();									// Inverse Substitute bytes 수행
			AddRoundKey();							    // Add Round Key 수행
			round == 0 ? true : MixColumns(); // Inverse Mix Columns 수행 단, round 0은 하지 않는다.
		}

		// 복호화 후 plaintext 출력
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
	// inv_sbox 사용해서 치환
	for (size_t i = 0; i < CipherSize; i++)
		plaintext[i] = sbox.my_inv_sbox[(unsigned char)ciphertext[i]];

	// Inv BS 출력
	if (printFlag) {
		cout << "Inv BS: ";
		for (size_t i = 0; i < KeySize; i++)
			cout << hex << (short)ciphertext[i] << ' ';
		cout << endl;
	}

	Copy();
}
void decoding::ShiftRows() {
	// 현재 ciphertext의 상태를 4x4 행렬로 저장
	char state[4][4];
	size_t idx = 0;
	for (size_t i = 0; i < 4; i++)
		for (size_t k = 0; k < 4; k++, idx++)
			state[k][i] = ciphertext[idx];

	// i행에 따라 i만큼 right shift진행
	char rotateState[4][4];
	for (size_t i = 0; i < 4; i++)
		for (size_t k = 0; k < 4; k++)
			rotateState[i][k] = state[i][(k - i + 4) % 4];

	// right shift진행 후 다시 plaintext에 저장
	idx = 0;
	for (size_t i = 0; i < 4; i++)
		for (size_t k = 0; k < 4; k++, idx++)
			plaintext[idx] = rotateState[k][i];

	// Inv SR출력
	if (printFlag) {
		cout << "Inv SR: ";
		for (size_t i = 0; i < KeySize; i++)
			cout << hex << (short)ciphertext[i] << ' ';
		cout << endl;
	}
	Copy();
}
errno_t decoding::MixColumns() {
	// 현재 ciphertext의 상태를 4x4 행렬로 저장
	char state[4][4];
	size_t idx = 0;
	for (size_t i = 0; i < 4; i++)
		for (size_t k = 0; k < 4; k++, idx++)
			state[k][i] = ciphertext[idx];

	// 각 열에 따라 state와 mix column을 위한 역행렬(mix_col_inv_y)을 곱하고 각 byte를 저장
	char mixState[4][4];
	for (size_t i = 0; i < 4; i++)
		for (size_t k = 0; k < 4; k++)
			mixState[k][i] = Multiply(state[0][i], mix_col_inv_y[k][0])
			^ Multiply(state[1][i], mix_col_inv_y[k][1])
			^ Multiply(state[2][i], mix_col_inv_y[k][2])
			^ Multiply(state[3][i], mix_col_inv_y[k][3]);

	// Inverse MixColumns진행 후 다시 plaintext에 저장
	idx = 0;
	for (size_t i = 0; i < 4; i++)
		for (size_t k = 0; k < 4; k++, idx++)
			plaintext[idx] = mixState[k][i];

	// Inv MR출력
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
	// 현재 Key와 ciphertext를 xor연산 수행한다.
	for (size_t i = 0; i < KeySize; i++)
		plaintext[i] = ciphertext[i] ^ curKey[i];

	// AR출력
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