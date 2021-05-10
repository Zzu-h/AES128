#include "encoding.h"

encoding::encoding(const char* _plain, const char* _cipher, const char* _key)
	:keys(_key) {
	this->plain_Path = _plain;
	this->cipher_Path = _cipher;
}

errno_t encoding::doEncoding() {
	return 0;
}

errno_t encoding::binCopy() {
	return 0;
}
void encoding::Substitute() {

}
void encoding::ShiftRows() {

}
void encoding::MixColumns() {

}
void encoding::AddRoundKey() {

}