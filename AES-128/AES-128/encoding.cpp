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

	// plain.bin을 읽고 plain[0] ~ plain[15] 까지 byte단위로 저장
	plain.seekg(0, plain.beg);
	plain.read(plaintext, PlainSize);
	plain.close();
	// end

	getCurKey(0);
	AddRoundKey();

	for (size_t round = 1; round <= Round; round++) {
		getCurKey(round);

		// 암호화
		Substitute();
		ShiftRows();
		(round == (Round)) ? true : MixColumns();
		AddRoundKey();
	}

	// cipher.bin에 ciphertext 쓰기
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
	// sbox 사용
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
	// GF 사용
	char temp[4] = { 0 };
	for (size_t i = 0; i < 4; i++) {
		for (size_t k = 0; k < 4; k++) 
			 temp[k] = plaintext[i + k * 4];

		// 스칼라곱
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
	// 참고 사이트 https://www.angelfire.com/biz7/atleast/mix_columns.pdf
	char c[4];
	for (size_t i = 0; i < 4; i++){
		bool flag1, flag2 = 1;
		if (mix_col_y[k][i] == 1) {
			// 그대로 흘려보냄
			c[i] = temp[i];
			continue;
		}
		else if (mix_col_y[k][i] == 2) {
			// 왼쪽 shift
			flag1 = 0;
		}
		else {
			// 3일 경우 1 + 2로 나누어서 계산
		}

		c[i] = temp[i];
		if ((c[i] * 2) < 256)
			flag2 = 0;

		c[i] << 1;
		c[i] = c[i] ^ (flag2 * polynomial[ver]) ^ (temp[i] * flag1);
		// 3이면 2 + 1의 연산 중 1일 때 xor연산을 한번더 해주어야 한다.
		// 따라서, flag1을 1로 나두어 기존 값을 XOR 수행
		// c[i] * 2에서 carry가 발생했을 경우 polynomial mod 연산을 수행해주어야 한다.
		// 따라서, flag2를 1로 나두어 XOR연산을 수행
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