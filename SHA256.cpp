#include "SHA256.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <sstream>
using namespace std;


const uint32 constants[64] = 
            {0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
             0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
             0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
             0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
             0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
             0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
             0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
             0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
             0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
             0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
             0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
             0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
             0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
             0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
             0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
             0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

//Initial hash values
uint32 hashConstants[] = 
			{0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 
			 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};


uint32 ROTR_32(uint32 w,int n){
	return (((w)>>(n)) | ((w)<<(32-(n))));//assumes we are getting 32 bits
}
uint32 SHR(uint32 w,int n){
	return ((w)>>(n));
}

void computeHashValues(const uchar8 data[]){

	uint32 a,b,c,d,e,f,g,h,s0,s1,S0,S1,ch, maj,temp1,temp2, w[64]={};

	//First copy the message into out w array.  Since data is made of type char
	//we haev to convert 4 chars to 1 int  8 to 32 bits.
	for(int i=0, j=0;i<16;i++, j+=4){
		w[i]= (data[j]<<24) | (data[j+1]<<16) | (data[j+2]<<8) | (data[j+3]);
	}
	//Next apply some matematical functions to the first 16 entries, ie part of the message,
	//to get the rest of teh 64 entries. 
	for(int i=16;i<64;i++){
			s0 = ROTR_32(w[i-15],7) ^ ROTR_32(w[i-15], 18) ^ SHR(w[i-15], 3);//sig0
			s1 = ROTR_32(w[i-2],17) ^ ROTR_32(w[i-2], 19) ^ SHR(w[i-2], 10);//sig1
			w[i] = w[i-16] + s0 + w[i-7] + s1;
	}
	//Initialize our variables to the current hash values
	a=hashConstants[0];
	b=hashConstants[1];
	c=hashConstants[2];
	d=hashConstants[3];
	e=hashConstants[4];
	f=hashConstants[5];
	g=hashConstants[6];
	h=hashConstants[7];

	//Where the compression happens.  Apply some more math to the data and mix up the outputs
	//in order to to get data that is not able to be reverse engineered.  
	for(int i=0;i<64;i++){
		S1 = ROTR_32(e,6) ^ ROTR_32(e,11) ^ ROTR_32(e, 25);//ep1
		ch = ((e) & (f)) ^ ((~e) & (g));
		temp1 = h + S1 + ch + constants[i] + w[i];
		S0 = ROTR_32(a,2) ^ ROTR_32(a, 13) ^ ROTR_32(a, 22);//ep0
		maj = (((a) & (b)) ^ ((a) & (c)) ^ ((b) & (c)));
		temp2 = S0 + maj;

		h = g;
		g = f;
		f = e;
		e = d + temp1;
		d = c;
		c = b;
		b = a;
		a = temp1 + temp2;
	}
	//add the compressed chunk to the current hash
	hashConstants[0] += a;
	hashConstants[1] += b;
	hashConstants[2] += c;
	hashConstants[3] += d;
	hashConstants[4] += e;
	hashConstants[5] += f;
	hashConstants[6] += g;
	hashConstants[7] += h;
}

void hashData(std::string mess, uint32 length, uchar8 hashed[]){
	uchar8 temp[length]={};
	for(int i=0; i<length;i++){
		temp[i]=mess[i];
	}
	hashData(temp, length, hashed);
}

void hashData(uchar8 mess[], uint32 length, uchar8 hashed[]){
	uint32 i=0;
	uint32 dataLength=0;
	ullint64 bitLength=0;
	uchar8 data[64]={};
	
	//Iterates through each character and adds it to a temporary data array and once it reaches
	//a certain length it sends it to get computed and added to the hash values. 
	for(i=0;i<length;i++){
		data[dataLength]=mess[i];
		dataLength++;
		if(dataLength==64){
			computeHashValues(data);
			bitLength+=512;
			dataLength=0;
		}
	}


	//takes the rest of the data that is just sitting in the data array and appends a 1 bit to the end.  
	//Nest 0's are appended onto the end in order to make sure the chunk has 512 bits.  Once it has the 
	//correctnumber of bits, the length in big endian is appended and sent through to get hashed.  
	i=dataLength;
	if(dataLength<56){
		data[i]=0x80;
		i++;
		while(i<56){
			data[i]=0x00;
			i++;
		}

	}
	else{
		data[i]=0x80;
		i++;
		while(i<64){
			data[i]=0x00;
			i++;
		}
		computeHashValues(data);
		memset(data, 0,56);
	}
	
	//Appending the length to the end
	bitLength += dataLength*8;
	data[63] = bitLength ;
	data[62] = bitLength >> 8;
	data[61] = bitLength >> 16;
	data[60] = bitLength >> 24;
	data[59] = bitLength >> 32;
	data[58] = bitLength >> 40;
	data[57] = bitLength >> 48;
	data[56] = bitLength >> 56;
	computeHashValues(data);

	//Once the hash constants that we have been changing every time we hash a block we can
	//now output the data.  Since my machine stores data in little endian and SHA family of
	//hash functions use big endian we have to convert it to big endian.
	for(int h=0; h<4;h++){
		hashed[h]    = (hashConstants[0] >> (24 - h * 8)) & 0x000000ff;
		hashed[h+4]  = (hashConstants[1] >> (24 - h * 8)) & 0x000000ff;
		hashed[h+8]  = (hashConstants[2] >> (24 - h * 8)) & 0x000000ff;
		hashed[h+12] = (hashConstants[3] >> (24 - h * 8)) & 0x000000ff;
		hashed[h+16] = (hashConstants[4] >> (24 - h * 8)) & 0x000000ff;
		hashed[h+20] = (hashConstants[5] >> (24 - h * 8)) & 0x000000ff;
		hashed[h+24] = (hashConstants[6] >> (24 - h * 8)) & 0x000000ff;
		hashed[h+28] = (hashConstants[7] >> (24 - h * 8)) & 0x000000ff;
	}

	//for(int p=0;p<32;p++){
	//	printf("%02x",(int)hashed[p]);
	//}
}