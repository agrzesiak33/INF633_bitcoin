#ifndef publicKey_h
#define publicKey_h

#include "BigIntegerLibrary.hh"
#include <vector>


typedef unsigned int uint_32;
typedef unsigned char uchar_8;

BigInteger genPrivateKey();
std::string inverse(std::string a, std::string, bool printing);
BigInteger inverse(BigInteger a, BigInteger m, bool printing);
BigInteger eea(BigInteger a, BigInteger b, BigInteger *s, BigInteger *t, bool printing);
std::vector<bool> reducePointOps(std::string privateKey);
std::vector<bool> reducePointOps(BigInteger privateKey);
void genPublicKey(std::string privateKey, std::string a, std::string x, std::string y, std::string modulo, std::string output[], bool printing);
void genPublicKey(BigInteger privateKey, BigInteger a, BigInteger x, BigInteger y, BigInteger modulo,std::string output[], bool printing);

#endif
