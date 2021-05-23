#include "encoding.h"
#include "decoding.h"

int main(int argc, char* argv[]) {

	if (argc != 2) {
		cout << "Please select 'd' or 'e'." << endl;
		exit(1);
	}
	bool printFlag = false;

	if (argv[1][0] == 'e') {
		encoding encode("plain.bin", "cipher.bin", "key.bin", printFlag);

		// doEncoding 함수 호출. 반드시 입력할 문자열을 함께 넘길 것. 크기는 MAX_LEN으로
		if (encode.doEncoding())
			cout << "Encoding Failed." << endl;
		else
			cout << "Encoding Successed." << endl;
	}
	else if (argv[1][0] == 'd') {
		decoding decode("cipher.bin", "plain2.bin", "key.bin", printFlag);

		// doDecoding 함수 호출. 반드시 출력할 문자열을 함께 넘길 것. 크기는 MAX_LEN으로
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