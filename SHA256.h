#ifndef SHA256_H
#define SHA256_H
#include <stddef.h>
#include <string>

typedef unsigned int uint32;
typedef unsigned char uchar8;
typedef unsigned long long int ullint64;


void hashData(std::string mess, uint32 length, uchar8 hashed[]);
void hashData(uchar8 mess[], uint32 length, uchar8 hashed[]);
uint32 ROTR_32(uint32 w, int n);
uint32 SHR(uint32 w, int n);
void computeHashValues(const uchar8 data[]);

#endif