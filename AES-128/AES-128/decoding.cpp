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


	// cipher.bin을 읽고 cipher[0] ~ cipher[15] 까지 byte단위로 저장
	cipher.seekg(0, cipher.beg);
	cipher.read(ciphertext, CipherSize);
	cipher.close();
	// end

	getCurKey(10);
	//AddRoundKey();
	for (size_t i = 0; i < KeySize; i++)
		cout << hex << short(ciphertext[i]) << " ";
	cout << endl;
	for (int round = Round-1; round >= 0; round--) {
		getCurKey(round);
		
		/*ShiftRows();
		Substitute();
		AddRoundKey();
		if(round != 0)
			MixColumns();*/

		MixColumns();

		for (size_t i = 0; i < KeySize; i++)
			cout << hex << short(ciphertext[i]) << " ";
		cout << endl;
	}

	// plain.bin에 plaintext 쓰기
	plain.seekp(0, plain.beg);
	plain.write(plaintext, PlainSize);
	plain.close();

	return 0;
}

void decoding::Copy() {
	for (size_t i = 0; i < CipherSize; i++)
		ciphertext[i] = plaintext[i];
}
void decoding::Substitute() {
	// sbox 사용
	for (size_t i = 0; i < CipherSize; i++) {
		plaintext[i] = inv_sbox[(unsigned char)ciphertext[i]];
	}

	Copy();
}
void decoding::ShiftRows() {
	/*size_t idx = 0;
	for (size_t i = 0; i < 4; i++)
		for (size_t k = 0; k < 4; k++, idx++)
			plaintext[idx] = ciphertext[((k - i + 4) % 4) + (4 * i)];*/

	/* row 1 */
	 plaintext[0] = ciphertext[0];
	 plaintext[1] = ciphertext[1];
	 plaintext[2] = ciphertext[2];
	 plaintext[3] = ciphertext[3];

	/* row 2 */
	 plaintext[4] = ciphertext[7];
	 plaintext[5] = ciphertext[4];
	 plaintext[6] = ciphertext[5];
	 plaintext[7] = ciphertext[6];

	/* row 3 */
	 plaintext[8] = ciphertext[10];
	 plaintext[9] = ciphertext[11];
	 plaintext[10] = ciphertext[8];
	 plaintext[11] = ciphertext[9];

	/* row 4 */
	 plaintext[12] = ciphertext[13];
	 plaintext[13] = ciphertext[14];
	 plaintext[14] = ciphertext[15];
	 plaintext[15] = ciphertext[12];

	Copy();
}
errno_t decoding::MixColumns() {
	// GF 사용

	char A, B, C, D;
	for (size_t i = 0; i < 4; i++) {
		for (size_t k = 0; k < 4; k++)
			plaintext[i + k * 4] = Multiply(ciphertext[i + (0 * 4)], mix_col_inv_y[k][0])
			^ Multiply(ciphertext[i + (1 * 4)], mix_col_inv_y[k][1])
			^ Multiply(ciphertext[i + (2 * 4)], mix_col_inv_y[k][2])
			^ Multiply(ciphertext[i + (3 * 4)], mix_col_inv_y[k][3]);
		/*cout << "\n\n";
		for (size_t k = 0; k < 4; k++)
			cout  << hex << (short)ciphertext[i + (k * 4)] << " ";
		cout << "\n\n"<< hex << (short)plaintext[i]<<endl;*/

		
		/*A = ciphertext[i + 0 * 4];
		B = ciphertext[i + 1 * 4];
		C = ciphertext[i + 2 * 4];
		D = ciphertext[i + 3 * 4];

		plaintext[i + 0 * 4] = Multiply(A, mix_col_inv_y[0][0])
			^ Multiply(B, mix_col_inv_y[0][1])
			^ Multiply(C, mix_col_inv_y[0][2])
			^ Multiply(D, mix_col_inv_y[0][3]);

		plaintext[i + 1 * 4] = Multiply(A, mix_col_inv_y[1][0])
			^ Multiply(B, mix_col_inv_y[1][1])
			^ Multiply(C, mix_col_inv_y[1][2])
			^ Multiply(D, mix_col_inv_y[1][3]);

		plaintext[i + 2 * 4] = Multiply(A, mix_col_inv_y[2][0])
			^ Multiply(B, mix_col_inv_y[2][1])
			^ Multiply(C, mix_col_inv_y[2][2])
			^ Multiply(D, mix_col_inv_y[2][3]);

		plaintext[i + 3 * 4] = Multiply(A, mix_col_inv_y[3][0])
			^ Multiply(B, mix_col_inv_y[3][1])
			^ Multiply(C, mix_col_inv_y[3][2])
			^ Multiply(D, mix_col_inv_y[3][3]);*/
	}
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

	/*cout << round << ": ";
	for (size_t i = 0; i < KeySize; i++)
		cout << hex << short(curKey[i]) << " ";
	cout << endl;*/
}