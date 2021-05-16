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
	AddRoundKey();

	for (size_t round = 1; round <= Round; round++) {
		getCurKey(round);

		// ��ȣȭ
		Substitute();
		ShiftRows();
		(round == (Round)) ? true : MixColumns();
		AddRoundKey();
	}

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
		short back = plaintext[i] % 0x10;
		short front = plaintext[i] / 0x10;
		ciphertext[i] = sbox.my_sbox[front][back];
	}

	Copy();
}
void encoding::ShiftRows() {
	size_t idx = 0;
	for (size_t i = 0; i < 4; i++)
		for (size_t k = 0; k < 4; k++, idx++)
			ciphertext[idx] = plaintext[((i + k) % 4) + (4 * i)];
	
	Copy();
}
errno_t encoding::MixColumns() {
	// GF ���
	char temp[4] = { 0 };
	for (size_t i = 0; i < 4; i++) {
		for (size_t k = 0; k < 4; k++) 
			 temp[k] = plaintext[i + k * 4];

		// ��Į���
		char out[4] = { 0 };
		for (size_t k = 0; k < 4; k++)
			out[k] = Scalar(k, temp);

		for (size_t k = 0; k < 4; k++)
			ciphertext[i + k * 4] = out[k];
	}

	Copy();
	return 0;
}
char encoding::Scalar(int k, char *temp) {
	// ���� ����Ʈ https://www.angelfire.com/biz7/atleast/mix_columns.pdf
	char c[4];
	for (size_t i = 0; i < 4; i++){
		bool flag1, flag2 = 1;
		if (mix_col_y[k][i] == 1) {
			// �״�� �������
			c[i] = temp[i];
			continue;
		}
		else if (mix_col_y[k][i] == 2) {
			// ���� shift
			flag1 = 0;
		}
		else {
			// 3�� ��� 1 + 2�� ����� ���
		}

		c[i] = temp[i];
		if ((c[i] * 2) < 256)
			flag2 = 0;

		c[i] << 1;
		c[i] = c[i] ^ (flag2 * polynomial[ver]) ^ (temp[i] * flag1);
		// 3�̸� 2 + 1�� ���� �� 1�� �� xor������ �ѹ��� ���־�� �Ѵ�.
		// ����, flag1�� 1�� ���ξ� ���� ���� XOR ����
		// c[i] * 2���� carry�� �߻����� ��� polynomial mod ������ �������־�� �Ѵ�.
		// ����, flag2�� 1�� ���ξ� XOR������ ����
	}

	char s = 0;
	for (size_t i = 0; i < 4; i++)
		s ^= c[i];
	return s;
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