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

	// plain.bin�� �а� plain[0] ~ plain[15] ���� byte������ ����
	plain.seekg(0, plain.beg);
	plain.read(plaintext, PlainSize);
	plain.close();
	// end

	getCurKey(0);
	//AddRoundKey();
	

	for (size_t round = 1; round <= Round; round++) {
		getCurKey(round);
		for (size_t i = 0; i < KeySize; i++)
			cout << hex << short(plaintext[i]) << " ";
		cout << endl;
		/*Substitute();
		ShiftRows();
		if(round != Round)
			MixColumns();
		AddRoundKey();*/

		MixColumns();
		
	}
	for (size_t i = 0; i < KeySize; i++)
		cout << hex << short(ciphertext[i]) << " ";
	cout << endl;
	// cipher.bin�� ciphertext ����
	cipher.seekp(0, cipher.beg);
	cipher.write(ciphertext, CipherSize);
	cipher.close();

	return 0;
}

void encoding::Copy() {
	for (size_t i = 0; i < PlainSize; i++)
		plaintext[i] = ciphertext[i];
}
void encoding::Substitute() {
	// sbox ���
	for (size_t i = 0; i < PlainSize; i++){
		ciphertext[i] = aes_sbox[(unsigned char)plaintext[i]];
	}

	Copy();
}
void encoding::ShiftRows() {
	/*size_t idx = 0;
	for (size_t i = 0; i < 4; i++)
		for (size_t k = 0; k < 4; k++, idx++)
			ciphertext[idx] = plaintext[((k + i) % 4) + (4 * i)];*/

	/* row 1 */
	ciphertext[0] = plaintext[0];
	ciphertext[1] = plaintext[1];
	ciphertext[2] = plaintext[2];
	ciphertext[3] = plaintext[3];

	/* row 2 */
	ciphertext[4] = plaintext[5];
	ciphertext[5] = plaintext[6];
	ciphertext[6] = plaintext[7];
	ciphertext[7] = plaintext[4];

	/* row 3 */
	ciphertext[8] = plaintext[10];
	ciphertext[9] = plaintext[11];
	ciphertext[10] = plaintext[8];
	ciphertext[11] = plaintext[9];

	/* row 4 */
	ciphertext[12] = plaintext[15];
	ciphertext[13] = plaintext[12];
	ciphertext[14] = plaintext[13];
	ciphertext[15] = plaintext[14];
	
	Copy();
}
errno_t encoding::MixColumns() {
	// GF ���
	char temp[4] = { 0, };
	char A, B, C, D;
	int I, T1, T2;

	//for (size_t i = 0; i < 4; i++)
	//{
	//	for (size_t k = 0; k < 4; k++)
	//		temp[k] = plaintext[i + k * 4];
	//	T1 = temp[0];
	//	T2 = temp[0] ^ temp[1] ^ temp[2] ^ temp[3];
	//	temp[0] ^= XTime(temp[0] ^ temp[1]) ^ T2;
	//	temp[1] ^= XTime(temp[1] ^ temp[2]) ^ T2;
	//	temp[2] ^= XTime(temp[2] ^ temp[3]) ^ T2;
	//	temp[3] ^= XTime(temp[3] ^ T1) ^ T2;

	//	for (size_t k = 0; k < 4; k++)
	//		ciphertext[i + k * 4] = temp[k];
	//}
	for (size_t i = 0; i < 4; i++) {
		for (size_t k = 0; k < 4; k++)
			ciphertext[i + k * 4] = Multiply(plaintext[i + 0 * 4], mix_col_y[k][0])
			^ Multiply(plaintext[i + 1 * 4], mix_col_y[k][1])
			^ Multiply(plaintext[i + 2 * 4], mix_col_y[k][2])
			^ Multiply(plaintext[i + 3 * 4], mix_col_y[k][3]);

		/*A = plaintext[i + 0 * 4];
		B = plaintext[i + 1 * 4];
		C = plaintext[i + 2 * 4];
		D = plaintext[i + 3 * 4];

		ciphertext[i + 0 * 4] = Multiply(A, mix_col_y[0][0])
			^ Multiply(B, mix_col_y[0][1])
			^ Multiply(C, mix_col_y[0][2])
			^ Multiply(D, mix_col_y[0][3]);

		ciphertext[i + 1 * 4] = Multiply(A, mix_col_y[1][0])
			^ Multiply(B, mix_col_y[1][1])
			^ Multiply(C, mix_col_y[1][2])
			^ Multiply(D, mix_col_y[1][3]);

		ciphertext[i + 2 * 4] = Multiply(A, mix_col_y[2][0])
			^ Multiply(B, mix_col_y[2][1])
			^ Multiply(C, mix_col_y[2][2])
			^ Multiply(D, mix_col_y[2][3]);

		ciphertext[i + 3 * 4] = Multiply(A, mix_col_y[3][0])
			^ Multiply(B, mix_col_y[3][1])
			^ Multiply(C, mix_col_y[3][2])
			^ Multiply(D, mix_col_y[3][3]);*/
	}

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

	/*cout << round << ": ";
	for (size_t i = 0; i < KeySize; i++)
		cout  << hex << short(curKey[i]) << " ";
	cout << endl;*/
}


//char encoding::Scalar(int k, char *temp) {
//	// ���� ����Ʈ https://www.angelfire.com/biz7/atleast/mix_columns.pdf
//	char c[4];
//	for (size_t i = 0; i < 4; i++){
//		bool flag1 = 1, flag2 = 1;
//		if (mix_col_y[k][i] == 1) {
//			// �״�� �������
//			c[i] = temp[i];
//			continue;
//		}
//		else if (mix_col_y[k][i] == 2) {
//			// ���� shift
//			flag1 = 0;
//		}
//		else {
//			// 3�� ��� 1 + 2�� ����� ���
//		}
//
//		c[i] = temp[i];
//		if ((c[i] * 2) < 256)
//			flag2 = 0;
//
//		c[i] << 1;
//		c[i] = c[i] ^ (flag2 * polynomial[ver]) ^ (temp[i] * flag1);
//		// 3�̸� 2 + 1�� ���� �� 1�� �� xor������ �ѹ��� ���־�� �Ѵ�.
//		// ����, flag1�� 1�� ���ξ� ���� ���� XOR ����
//		// c[i] * 2���� carry�� �߻����� ��� polynomial mod ������ �������־�� �Ѵ�.
//		// ����, flag2�� 1�� ���ξ� XOR������ ����
//	}
//
//	char s = 0;
//	for (size_t i = 0; i < 4; i++)
//		s ^= c[i];
//	return s;
//}