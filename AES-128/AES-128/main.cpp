#include "encoding.h"
#include "decoding.h"

int main(int argc, char* argv[]) {

	if (argc != 2) {
		cout << "Please select 'd' or 'e'." << endl;
		exit(1);
	}
	bool printFlag = false;

	if (argv[1][0] == 'e') {
		// �� plaintext ���, ciphertext ���, key ���, ��ȣȭ ���� ��� ���θ� �޴´�.
		encoding encode("plain.bin", "cipher.bin", "key.bin", printFlag);

		// doEncoding �Լ� ȣ��. 
		if (encode.doEncoding())
			cout << "Encoding Failed." << endl;
		else
			cout << "Encoding Successed." << endl;
	}
	else if (argv[1][0] == 'd') {
		// �� ciphertext���, ��ȣȭ�� plaintext  ���, key ���, ��ȣȭ ���� ��� ���θ� �޴´�.
		decoding decode("cipher.bin", "plain2.bin", "key.bin", printFlag);

		// doDecoding �Լ� ȣ��
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