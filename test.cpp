#include <iostream>
#include "fix32.hpp"

#define TEST_CASE(function)												\
	if(function()){ 											\
		std::cout << "[  Ok  ] - " << #function << std::endl;	\
	}else{ 																\
		std::cout << "[Failed] - " << #function << std::endl;	\
	}

bool construct_fixpoint(){
	volatile fix32<16> a; //default construct
	volatile auto b = fix32<23>::from(1);
	volatile auto c = fix32<28>::reinterpret_from(2315);
	return true;
}

int main(){
	std::cout << "Fixpoint Tests:" << std::endl;
	std::cout << "---------------" << std::endl;
	
	
	TEST_CASE(construct_fixpoint);
	
	return 0;
}