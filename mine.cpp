#include "mine.h"
#include "SHA256.h"
#include <stdio.h>
#include <sstream>
#include <iostream>

using namespace std;

void mine(){
	//Initialize bitcoin block data
	//int = 32 bits
	uint32 version    = 3551263261;
	uint32 time       = 0x53058b35;
	uint32 bits       = 0x180455d2;
	std::string prev_block 	= "000000000000000001cc7e0e320fc5dca83376de6de83245db2958bee2d87f03";//64 hex chars long
	std::string merkl_root 	= "ad4e3a15d28de232a61106b2fefaba855bf107a933e2b7d890e6644b6d430b3c";//
	//cout<<prev_block.length();

	uchar8 blockToHash[82]={};
	uint32 currentPos=0;
	std::stringstream ss;
	
	//Now we add all the information that isn't going to change into our array that we will eventually
	//o send to the hash function.  

	//add the version to the front of the processing block
	blockToHash[currentPos++] = version >> 24 ;
	blockToHash[currentPos++] = version >> 16;
	blockToHash[currentPos++] = version >> 8;
	blockToHash[currentPos++] = version;

 	//convert prev_block from hexstring to char[] and add it to our processing block
	for(int i=63;i>=0;i-=2){
		ss<<std::hex<<prev_block.substr(i,i-1);
		ss>>blockToHash[currentPos++];
	}

	//convert the merkel root from hexstring to char[] and add it to pur processing block
	for(int i=63;i>=0;i-=2){
		ss<<std::hex<<merkl_root.substr(i,i-1);
		ss>>blockToHash[currentPos++];
	}

	//add the time to the processing block
	blockToHash[currentPos++] = time >> 24 ;
	blockToHash[currentPos++] = time >> 16;
	blockToHash[currentPos++] = time >> 8;
	blockToHash[currentPos++] = time;

	//add the bits to the processing block
	blockToHash[currentPos++] = bits >> 24 ;
	blockToHash[currentPos++] = bits >> 16;
	blockToHash[currentPos++] = bits >> 8;
	blockToHash[currentPos++] = bits;

	//Now we will start trying possible nonce values attempting to get one that gives us a certain amount of leading 0's
	//this temp array will hold the result that we get back from the hash function
	uchar8 temp[32];
	uint32 nonce = 0;

	while(nonce < 0xffffffff){
		//add nonce to the end of the array
		blockToHash[78] = nonce >> 24 ;
		blockToHash[79] = nonce >> 16;
		blockToHash[80] = nonce >> 8;
		blockToHash[81] = nonce;

		hashData(blockToHash, 82, temp);
		for(int p=0;p<32;p++){
			printf("%02x", (int)temp[p]);
		}
		cout<<", "<<nonce<<endl;
		if(temp[0]==0 && temp[1]==0 &&temp[2]==0 && temp[3]==0 &&
			temp[4]==0 && temp[5]==0 && temp[6]==0 && temp[7]==0 && (temp[8]>>4) == 0 ){
			return;
		}
		nonce++;
	}
}