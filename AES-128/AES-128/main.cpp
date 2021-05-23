#include "encoding.h"
#include "decoding.h"

int main(int argc, char* argv[]) {

	if (argc != 2) {
		cout << "Please select 'd' or 'e'." << endl;
		exit(1);
	}
	bool printFlag = false;

	if (argv[1][0] == 'e') {
		// 각 plaintext 경로, ciphertext 경로, key 경로, 암호화 과정 출력 여부를 받는다.
		encoding encode("plain.bin", "cipher.bin", "key.bin", printFlag);

		// doEncoding 함수 호출. 
		if (encode.doEncoding())
			cout << "Encoding Failed." << endl;
		else
			cout << "Encoding Successed." << endl;
	}
	else if (argv[1][0] == 'd') {
		// 각 ciphertext경로, 복호화된 plaintext  경로, key 경로, 복호화 과정 출력 여부를 받는다.
		decoding decode("cipher.bin", "plain2.bin", "key.bin", printFlag);

		// doDecoding 함수 호출
		if (decode.doDecoding())
			cout << "Decoding Failed" << endl;
		else
			cout << "Decoding Successed." << endl;
	}
	else {
		cout << "Please select 'd' or 'e'." << endl;
	}

	return 0;
}