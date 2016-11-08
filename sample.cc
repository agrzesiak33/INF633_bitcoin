// Sample program demonstrating the use of the Big Integer Library.

// Standard libraries
#include <string>
#include <iostream>
#include <vector>

// `BigIntegerLibrary.hh' includes all of the library headers.
#include "BigIntegerLibrary.hh"
using namespace std;

std::vector<bool> reducePointOps(BigInteger privateKey){
	std::vector<bool> sequence;
	BigInteger temp(privateKey);
	while(temp>2){
		if(temp%2==0){
			sequence.push_back(true);
			cout<<"1";
			temp/=2;
		}

		else{
			sequence.push_back(false);
			temp-=1;
			cout<<"0";
		}
	}
	sequence.push_back(true);
	cout<<"1";
	cout<<endl;
	return sequence;
}

int main() {
	/* The library throws `const char *' error messages when things go
	 * wrong.  It's a good idea to catch them using a `try' block like this
	 * one.  Your C++ compiler might need a command-line option to compile
	 * code that uses exceptions. */
	try {
		// Add your own code here to experiment with the library.
		std::vector<bool> sequence;
		BigUnsigned pk = stringToBigUnsigned("89289789787928973487945389705348790438970534897034");
		
		sequence = reducePointOps(pk);
		for(std::vector<bool>::iterator it = sequence.begin(); it != sequence.end(); ++it){
			if(*it == true)
				std::cout<<"1";
			else
				std::cout<<"0";
		}
		std::cout<<std::endl;

	} catch(char const* err) {
		std::cout << "The library threw an exception:\n"
			<< err << std::endl;
	}

	return 0;
}

/*
The original sample program produces this output:

3141592653589793238462643383279
314424
313894
83252135
1185
134
0xFF
0xFF00FFFF
0xFF00FF00
0x1FFFE00000
0x3F
314^0 = 1
314^1 = 314
314^2 = 98596
314^3 = 30959144
314^4 = 9721171216
314^5 = 3052447761824
314^6 = 958468597212736
314^7 = 300959139524799104
314^8 = 94501169810786918656
314^9 = 29673367320587092457984
314^10 = 9317437338664347031806976
12
8
1931

*/
