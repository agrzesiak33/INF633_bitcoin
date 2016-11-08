#include "SHA256.h"
#include "mine.h"
#include "publicKey.h"
#include <cstring>
#include <string.h>
#include <stdio.h>
#include <fstream>

using namespace std;

int main(int argc, char** argv){
	if(argc ==1){
		std::cout<<"Enter a command 1 to test SHA256, 2 to test mining, and 3 to test the publicKey generator."<<endl;
		return -1;
	}
	std::string temp(argv[1]);

	//testing SHA256
	if(temp=="1" && argc==3){
		unsigned char output[32];
		std::string input(argv[2]);
		unsigned int length = input.length();
		hashData(input, length, output);
		for(int p=0;p<32;p++)
			printf("%02x", (int)output[p]);
		cout<<endl;
		 
	}
	//testing the mining of a bitcoin
	else if(temp=="2"){
		mine();
	}
	//testing the faulty public key generator
	else if (temp=="3" && argc >6 ){
		//bitcoin modulo:	FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F
		//bitcoin start x: 	79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798
		//bitcoin start y: 	483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8
		//bitcoin a:		0
		std::string privateKey(	argv[2]);
		std::string a(		   	argv[3]);
		std::string x(			argv[4]);
		std::string y(			argv[5]);
		std::string modulo(		argv[6]);
		bool printing =(argc==8)?true:false;
		std::string output[2];
		genPublicKey(privateKey, a, x, y, modulo, output, printing);
		cout<<"Public key is: " << output[0]<<" " <<output[1]<<endl;
	}

	//testing inverse
	else if(temp=="4" && argc>3){
		std::string input(argv[2]);
		std::string modulo(argv[3]);
		bool printing =false;
		if(argc==5)
			printing =true;
		std::string inverse_(inverse(input, modulo, printing));
		cout<<"Inverse of "<< input << " = " <<inverse_<<endl;
	}
	
	//testing reducePointOps
	else if(temp=="5" && argc==3){
		std::string privateKey(argv[2]);
		std::vector<bool> sequence = reducePointOps(privateKey);

		for(std::vector<bool>::reverse_iterator it = sequence.rbegin(); it != sequence.rend(); ++it){
			if(*it == true){
				cout<<"1";
			}
			else{
				cout<<"0";		
			}
		}
		cout<<endl;
	}


	

	return 0;
}