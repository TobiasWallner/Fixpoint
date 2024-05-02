/*
	Author: Tobias Wallner
	tobias.wallner1@gmx.net
	
*/


#include <iostream>
#include "fix32.hpp"

#define TEST_CASE(function)										\
	if(function()){ 											\
		std::cout << "[  Ok  ] - " << #function << std::endl;	\
	}else{ 														\
		std::cout << "[Failed] - " << #function << std::endl;	\
	}

bool construct_fixpoint(){
	volatile fix32<16> a; //default construct
	volatile auto b = fix32<23>::from(1);
	volatile auto c = fix32<28>::reinterpret(2315);
	volatile fix32<10> d = 5;
	return true;
}

bool comparison(){
	fix32<23> a = 5;
	fix32<23> b = 11;
	
	bool result = true;
	
	result &= (a == a) == true;
	result &= (b == b) == true;
	result &= (a == b) == false;
	result &= (b == a) == false;
	
	result &= (a != a) == false;
	result &= (b != b) == false;
	result &= (a != b) == true;
	result &= (b != a) == true;
	
	result &= (a < a) == false;
	result &= (a < b) == true;
	result &= (b < a) == false;
	
	result &= (a <= a) == true;
	result &= (a <= b) == true;
	result &= (b <= a) == false;
	
	result &= (a > a) == false;
	result &= (a > b) == false;
	result &= (b > a) == true;
	
	result &= (a >= a) == true;
	result &= (a >= b) == false;
	result &= (b >= a) == true;
	
	return result;
}

bool conversion(){
	fix32<23> a = 5;
	
	bool result = true;
	
	result &= a == fix32<23>::from(5);
	result &= a != fix32<23>::reinterpret(5);
	result &= a == fix32<23>::reinterpret(5<<23);
	
	return result;
}

bool addition(){
	fix32<23> a = 5;
	fix32<23> b = 11;
	int32_t expected_result = (5 + 11) << 23;
	fix32<23> result = a + b;
	int32_t int_result = reinterpret_as_int32_t(result);
	return expected_result == int_result;
}

bool subtraction(){
	fix32<23> a = 5;
	fix32<23> b = 11;
	int32_t expected_result = (5 - 11) << 23;
	fix32<23> result = a - b;
	int32_t int_result = reinterpret_as_int32_t(result);
	return expected_result == int_result;
}

bool multiplication(){
	fix32<20> a = 5;
	fix32<20> b = 11;
	fix32<20> expected_result = fix32<20>::reinterpret((5 * 11) << 20);
	fix32<20> result = a * b;
	return expected_result == result;
}

bool division(){
	fix32<20> a = 5;
	fix32<20> b = 11;
	fix32<20> expected_result = fix32<20>::reinterpret((5<<20) / 11);
	fix32<20> result = a / b;
	return expected_result == result;
}

bool self_addition(){
	fix32<23> a = 5;
	fix32<23> b = 11;
	int32_t expected_result = (5 + 11) << 23;
	a += b;
	int32_t int_result = reinterpret_as_int32_t(a);
	return expected_result == int_result;
}

bool self_subtraction(){
	fix32<23> a = 5;
	fix32<23> b = 11;
	int32_t expected_result = (5 - 11) << 23;
	a -= b;
	int32_t int_result = reinterpret_as_int32_t(a);
	return expected_result == int_result;
}

bool self_multiplication(){
	fix32<20> a = 5;
	fix32<20> b = 11;
	fix32<20> expected_result = fix32<20>::reinterpret((5 * 11) << 20);
	a *= b;
	return expected_result == a;
}

bool self_division(){
	fix32<20> a = 5;
	fix32<20> b = 11;
	fix32<20> expected_result = fix32<20>::reinterpret((5<<20) / 11);
	a /= b;
	return expected_result == a;
}

bool conversion_between_formatats(){
	bool first;
	{
		fix32<20> a = 5;
		fix32<10> b(a);
		fix32<10> expected(5);
		first = b == expected;
	}
	bool second;
	{
		fix32<10> a = 5;
		fix32<20> b(a);
		fix32<20> expected(5);
		second = b == expected;	
	}
	return first && second;
}

bool casting_between_formatats(){
	bool first;
	{
		fix32<20> a = 5;
		fix32<10> b = static_cast<fix32<10>>(a);
		fix32<10> expected(5);
		first = b == expected;
	}
	bool second;
	{
		fix32<10> a = 5;
		fix32<20> b = static_cast<fix32<20>>(a);
		fix32<20> expected(5);
		second = b == expected;	
	}
	return first && second;
}

int main(){
	std::cout << "Fixpoint Tests:" << std::endl;
	std::cout << "---------------" << std::endl;
	
	std::cout << "float test: " << (fix32<20>(420)/fix32<20>(507)) << std::endl;
	
	TEST_CASE(construct_fixpoint);
	
	TEST_CASE(comparison);
	TEST_CASE(conversion);
	
	TEST_CASE(addition);
	TEST_CASE(subtraction);
	TEST_CASE(multiplication);	
	TEST_CASE(division);
	
	TEST_CASE(self_addition);
	TEST_CASE(self_subtraction);
	TEST_CASE(self_multiplication);
	TEST_CASE(self_division);
	
	TEST_CASE(conversion_between_formatats);
	TEST_CASE(casting_between_formatats);
	
	
	return 0;
}