/*
	Author: Tobias Wallner
	tobias.wallner1@gmx.net
	
*/


#include <iostream>
#include <sstream>
#include "fixmath.hpp"

#define TEST_CASE(function)										\
	if(function()){ 											\
		std::cout << "[  Ok  ] - " << #function << std::endl;	\
	}else{ 														\
		std::cout << "[Failed] - " << #function << std::endl;	\
	}

bool test_abs(){
	fix32<16> a(5);
	fix32<16> b(-5);
	return a != b && a == abs(b) && abs(a) == abs(b);
}


int main(){
	
	std::cout << "fixmath tests:" << std::endl;
	std::cout << "---------------" << std::endl;
	
	TEST_CASE(test_abs);
	
	return 0;
}