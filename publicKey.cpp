#include "publicKey.h"
#include <string>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include "BigIntegerLibrary.hh"

using namespace std;
/*
Order= 0xFFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFE BAAEDCE6 AF48A03B BFD25E8C D0364141    256 bits
Prime Modulo = 0xFFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFE FFFFFC2F
Base Point = 04 79BE667E F9DCBBAC 55A06295 CE870B07 029BFCDB 2DCE28D9 59F2815B 16F81798
			      483ADA77 26A3C465 5DA4FBFC 0E1108A8 FD17B448 A6855419 9C47D08F FB10D4B8
y^2 = x^3 + ax + b
Bitcoin uses values a=0 b=7
*/
BigInteger order = stringToBigUnsigned16("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141");
//BigInteger primeModulo  = stringToBigUnsigned16("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F");
//BigInteger basePoint_X  = stringToBigUnsigned16("79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798");
//BigInteger basePoint_Y  = stringToBigUnsigned16("483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8");

//got this code from stackoverflow since there is a bug with std::to_string()
namespace patch{
	template <typename T> std::string to_string (const T& n){
		std::ostringstream stm;
		stm<<n;
		return stm.str();
	}
}


//Private Key is a number from 1-Order
BigInteger genPrivateKey(){
	BigInteger privateKey(0);
	std::string privateKey_string;

	privateKey_string += patch::to_string(rand());
	privateKey_string += patch::to_string(rand());
	privateKey_string += patch::to_string(rand());
	privateKey_string += patch::to_string(rand() % 0xfffffffe);
	privateKey_string += patch::to_string(rand() % 0xBAAEDCE6);
	privateKey_string += patch::to_string(rand() % 0xAF48A03B);
	privateKey_string += patch::to_string(rand() % 0xBFD25E8C);
	privateKey_string += patch::to_string(rand() % 0xD0364141);

	return stringToBigInteger(privateKey_string);
}
std::string inverse(std::string a, std::string modulo, bool printing){
	return bigIntegerToString(inverse(stringToBigInteger(a), stringToBigInteger(modulo), printing));
}

BigInteger inverse(BigInteger a, BigInteger m, bool printing){
	BigInteger s;
	BigInteger t;
	eea(m, a, &s, &t, printing);
	if(t<0)
		return (t%m);
	else
		return t;
}

BigInteger eea(BigInteger a, BigInteger b, BigInteger *s, BigInteger *t, bool printing){
	if(a==0){
		*s=0;
		*t=1;
		return b;
	}

	BigInteger s1;
	BigInteger t1;
	BigInteger gcd = eea(b%a, a, &s1, &t1, printing);
	if(printing){
		cout<<"a: "<<a<<", b: "<<b<<endl;
	}
	*s = t1 - (b/a) * s1;
	*t = s1;

	return gcd;
}

/*
reducePointOps takes in the private key and vinds out the various point additions and multiplications
we need to do in order to recuce the amount of operations we need to perform.
The output string format is as follows:	If temp % 2 = 0 we push a true in the vector meaning we are 
going to do a point doubling.  If temp % 2 =1 we push a false meaning we will do a point addition.
Once we get down to when temp is equal to 2 (the last step will ALWAYS have temp = 2) we will simply
push a true onto the vector. 
*/
std::vector<bool> reducePointOps(std::string privateKey){
	return(reducePointOps(stringToBigInteger(privateKey)));
}

std::vector<bool> reducePointOps(BigInteger privateKey){
	std::vector<bool> sequence;
	BigInteger temp(privateKey);
	while(temp>2){
		if(temp%2==0){
			sequence.push_back(true);
			temp/=2;
		}

		else{
			sequence.push_back(false);
			temp-=1;
		}
	}
	sequence.push_back(true);
	return sequence;
}

void genPublicKey(std::string privateKey, std::string a, std::string x, 
						 std::string y, std::string modulo, std::string output[], bool printing){

	return(genPublicKey(stringToBigUnsigned16(privateKey), 
						stringToBigUnsigned16(a), 
						stringToBigUnsigned16(x),
						stringToBigUnsigned16(y),
						stringToBigUnsigned16(modulo),
						output,
						printing));
}

void genPublicKey(BigInteger privateKey, BigInteger a, BigInteger x, BigInteger y, BigInteger modulo, std::string output[], bool printing){
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

	//cout<<privateKey<< a<< x<<y<<modulo<<endl;
	BigInteger s;
	BigInteger x3 = x;
	BigInteger y3 = y;
	BigInteger six(6); 
	BigInteger rx, ry;
	BigInteger three(3);
	BigInteger two(2);

	for(std::vector<bool>::reverse_iterator it = sequence.rbegin(); it != sequence.rend(); ++it){
		//meaning point doubling
		if(*it == true){
			cout<<privateKey<<a<<x<<y<<modulo<<endl;
			s = (((three * (x3 * x3))+a) * inverse((two*y3), modulo, false))%modulo;

			rx = (((s*s)%modulo)-(two * x3))%modulo;

			ry = (s * (x3 - rx) - y3) % modulo;

			if(printing)
				cout<<"Doubling: s="<<s<<", x="<<rx<<", y="<<ry<<endl;
		}
		//meaning point addition
		else{
			//(x2,y2) -> base point 		(x1,y1)->(x3, y3) aka previously calculated point
			s = ((y - y3) * inverse((x - x3), modulo,false)) % modulo;

			rx = (((s*s) % modulo) - x3 - x) % modulo;

			ry = ((s * (x3 - rx)) - y3)%modulo;
			if(printing)
				cout<<"Addition: s="<<s<<", x="<<rx<<", y="<<ry<<endl;		
		}

		if(rx==x && ry==(y-1)){
			x3=x;
			y3=y;
		}
		else{
			x3=rx;
			y3=ry;
		}
	}

	output[0]=bigIntegerToString(x3);
	output[1]=bigIntegerToString(y3);
}


//int main(){
	
	//test inverse function
	//BigInteger temp_prime_modulo = primeModulo;
	//BigInteger temp_base_X = basePoint_X;
	//BigInteger temp_base_Y = basePoint_Y;

/*
	BigInteger a(2);
	BigInteger  modulo(17);
	BigInteger a_inverse = inverse(a, modulo);
	std::cout<<a_inverse<<endl;
*/
	//primeModulo=temp_prime_modulo;
	//a=stringToBigInteger("798465132654987465132654849516651354357");
	//a_inverse = inverse(a);
	//std::cout<<a_inverse<<endl;
	//end inverse test

	//test reducePointOps
	/*
	std::vector<bool> sequence1 = reducePointOps(stringToBigInteger("10"));
	for(std::vector<bool>::reverse_iterator it = sequence1.rbegin(); it != sequence1.rend(); ++it){
		if(*it == true){
			cout<<"1";
		}
		else{
			cout<<"0";		
		}
	}
	cout<<endl;
	/*
	cout<<endl;
	std::vector <bool> sequence1 = reducePointOps(stringToBigInteger("15"));
	for(std::vector<bool>::reverse_iterator it = sequence1.rbegin(); it != sequence1.rend(); ++it){
		if(*it == true){
			cout<<"1";
		}
		else{
			cout<<"0";		
		}
	}
	cout<<endl;*/
	//end reduceTestOps test
	
	/*
	//Test genPublic with in class example
	BigInteger five(5);
	BigInteger one(1);
	BigInteger two(2);
	BigInteger seventeen(17); 

	BigInteger privateKey;
	privateKey = stringToBigInteger("4");
	std::string publicKey_str;
	publicKey_str = genPublicKey(privateKey, two, five, one, seventeen, true);
	cout << publicKey_str<<endl;
	


	//test genPublicKey with a private key whose public key is known
	BigInteger privateKey = stringToBigUnsigned16("18E14A7B6A307F426A94F8114701E7C8E774E7F9A47E2C2035DB29A206321725");
	std::string publicKey_str;
	publicKey_str = genPublicKey(privateKey);

	BigUnsigned publicKey_int;
	publicKey_int=stringToBigUnsigned(publicKey_str);
	std::string publicKey16_str;
	publicKey16_str = bigUnsignedToString16(publicKey_int);
	cout << publicKey16_str<<endl;

	return 1;
}
*/
//458ACBDD5DE95281B9ACAB95D0F8DAC011C895EBE95E567BE9E308C12936CCAE3B81CE567B126A604A938D29ED15B5A96779AF27B5DC9 ED6F2EE187E9C582BA6