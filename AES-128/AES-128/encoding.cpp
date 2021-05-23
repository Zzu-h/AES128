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

	// plaintext 전체를 읽어올 때 까지 while문을 계속 돌린다.
	while(true){
		plain.read(plaintext, PlainSize);
		if(plain.eof()) // plain.bin의 끝일 경우 이 while문을 끝냄
			break;

		// 암호화 전 plaintext 출력
		cout << "plaintext: ";
		for (size_t i = 0; i < KeySize; i++)
			cout << hex << short(plaintext[i]) << " ";
		cout << endl;

		// 현재 round 0의 key를 받고 저장 및 Addround 수행
		getCurKey(0);
		AddRoundKey();

		// encrypt는 round 1부터 10까지 진행한다.
		for (size_t round = 1; round <= Round; round++) {
			printFlag && cout <<dec << round<< " round" << endl;

			getCurKey(round);

			Substitute(); 												 // Substitute bytes 수행
			ShiftRows();												// Shift Rows 수행
			round == Round ? true : MixColumns();	   // Mix Columns 수행 단, round 10은 하지 않는다.
			AddRoundKey();											// Add Round Key 수행

		}

		// 암호화 후 ciphertext 출력
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
	// sbox 사용해서 치환
	for (size_t i = 0; i < PlainSize; i++)
		ciphertext[i] = sbox.my_aes_sbox[(unsigned char)plaintext[i]];

	// BS 출력
	if (printFlag) {
		cout << "BS: ";
		for (size_t i = 0; i < KeySize; i++)
			cout << hex << (short)ciphertext[i] << ' ';
		cout << endl;
	}

	Copy();
}
void encoding::ShiftRows() {
	// 현재 plaintext의 상태를 4x4 행렬로 저장
	char state[4][4];
	size_t idx = 0;
	for (size_t i = 0; i < 4; i++)
		for (size_t k = 0; k < 4; k++, idx++)
			state[k][i] = plaintext[idx];

	// i행에 따라 i만큼 left shift진행
	char rotateState[4][4];
	for (size_t i = 0; i < 4; i++)
		for (size_t k = 0; k < 4; k++)
			rotateState[i][k] = state[i][(i + k) % 4];

	// left shift진행 후 다시 ciphertext에 저장
	idx = 0;
	for (size_t i = 0; i < 4; i++)
		for (size_t k = 0; k < 4; k++, idx++)
			ciphertext[idx] = rotateState[k][i];

	// SR 출력
	if (printFlag) {
		cout << "SR: ";
		for (size_t i = 0; i < KeySize; i++)
			cout << hex << (short)ciphertext[i] << ' ';
		cout << endl;
	}
	Copy();
}
errno_t encoding::MixColumns() {
	// 현재 plaintext의 상태를 4x4 행렬로 저장
	char state[4][4];
	size_t idx = 0;
	for (size_t i = 0; i < 4; i++)
		for (size_t k = 0; k < 4; k++, idx++)
			state[k][i] = plaintext[idx];

	// 각 열에 따라 state와 mix column을 위한 행렬(mix_col_y)을 곱하고 각 byte를 저장
	char mixState[4][4];
	for (size_t i = 0; i < 4; i++)
		for (size_t k = 0; k < 4; k++)
			mixState[k][i] = Multiply(state[0][i], mix_col_y[k][0])
			^ Multiply(state[1][i], mix_col_y[k][1])
			^ Multiply(state[2][i], mix_col_y[k][2])
			^ Multiply(state[3][i], mix_col_y[k][3]);

	// MixColumns진행 후 다시 ciphertext에 저장
	idx = 0;
	for (size_t i = 0; i < 4; i++)
		for (size_t k = 0; k < 4; k++, idx++)
			ciphertext[idx] = mixState[k][i];

	// MR출력
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
	// 현재 Key와 plaintext를 xor연산 수행한다.
	for (size_t i = 0; i < KeySize; i++)
		ciphertext[i] = plaintext[i] ^ curKey[i];

	// AR출력
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