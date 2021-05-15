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
#define polynomial 0b100011011; // 𝑥^8 + 𝑥^4 + 𝑥^3 + 𝑥 + 1
#define polynomial_transformed 0b111100111; // x^8 + 𝑥^7 + 𝑥^6 + 𝑥^5 + 𝑥^2 + 𝑥 + 1

typedef char Key;