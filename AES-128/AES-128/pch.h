#pragma once
#include <iostream>
#include <istream>
#include <fstream>
using namespace std;

#define ExpansionCount 176

#define KeySize 16 //byte
#define PlainSize 16 //byte
#define CipherSize 16 //byte

#define Round 10

static const uint8_t polynomial[2] = { 0x1B, 0xE7};
#define ver 1

typedef char Key;

static const uint8_t mix_col_y[4][4] = {
    {0x02, 0x03, 0x01, 0x01},
    {0x01, 0x02, 0x03, 0x01},
    {0x01, 0x01, 0x02, 0x03},
    {0x03, 0x01, 0x01, 0x02}
};
static const uint16_t mix_col_inv_y[4][4] = {
   { 0x0E,0x0B,0x0D,0x09 },
   { 0x09,0x0E,0x0B,0x0D },
   { 0x0D,0x09,0x0E,0x0B },
   { 0x0B,0x0D,0x09,0x0E }
};

static int XTime(int X){
    return ((X << 1) & 0xFF) ^ (((X >> 7) & 1) * polynomial[ver]);
}
static int Multiply(int X, int Y){
    return ((Y >> 0 & 1) * X) ^
        ((Y >> 1 & 1) * XTime(X)) ^
        ((Y >> 2 & 1) * XTime(XTime(X))) ^
        ((Y >> 3 & 1) * XTime(XTime(XTime(X)))) ^
        ((Y >> 4 & 1) * XTime(XTime(XTime(XTime(X))))) ^
        ((Y >> 5 & 1) * XTime(XTime(XTime(XTime(XTime(X)))))) ^
        ((Y >> 6 & 1) * XTime(XTime(XTime(XTime(XTime(XTime(X))))))) ^
        ((Y >> 7 & 1) * XTime(XTime(XTime(XTime(XTime(XTime(XTime(X))))))));
}