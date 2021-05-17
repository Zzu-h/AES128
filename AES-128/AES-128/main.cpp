#include "encoding.h"
#include "decoding.h"

int main(int argc, char* argv[]) {
	//uint8_t t1[4] = { 0x0E,0x09,0x0D,0x0B };
	/*uint8_t t1[4] = { mix_col_inv_y[0][0],mix_col_inv_y[0][1],mix_col_inv_y[0][2],mix_col_inv_y[0][3] };
	uint8_t t2[4] = { 0x16,0x52,0x9E,0xDA };
	uint8_t out_ = Multiply(t1[0], t2[0]) ^ Multiply(t1[1], t2[1]) ^ Multiply(t1[2], t2[2]) ^ Multiply(t1[3], t2[3]);
	cout << hex << (short)out_ << endl;*/

	/*uint8_t t1[4] = { mix_col_y[1][0],mix_col_y[1][1],mix_col_y[1][2],mix_col_y[1][3] };
	uint8_t t2[4] = { 0x93,0x33,0xfc,0x82 };
	uint8_t out_ = Multiply(t1[0], t2[0])
		^ Multiply(t1[1], t2[1])
		^ Multiply(t1[2], t2[2])
		^ Multiply(t1[3], t2[3]);
	cout << hex << (unsigned)out_ << endl;*/

	if (argc != 2) {
		cout << "Please select 'd' or 'e'." << endl;
		exit(1);
	}

	if (argv[1][0] == 'e') {
		encoding encode("plain.bin", "cipher.bin", "key.bin");

		// doEncoding 함수 호출. 반드시 입력할 문자열을 함께 넘길 것. 크기는 MAX_LEN으로
		if (encode.doEncoding())
			cout << "Encoding Failed." << endl;
		else
			cout << "Encoding Successed." << endl;
	}
	else if (argv[1][0] == 'd') {

		decoding decode("cipher.bin", "plain2.bin", "key.bin");

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