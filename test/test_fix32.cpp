/*
	Author: Tobias Wallner
	tobias.wallner1@gmx.net
	
*/


#include <iostream>
#include <sstream>
#include "fix32.hpp"
#include "fix64.hpp"
//#include "fixmath.hpp"

#define TEST_CASE(function)										\
	if(function()){ 											\
		std::cout << "[  Ok  ] - " << #function << std::endl;	\
	}else{ 														\
		std::cout << "[Failed] - " << #function << std::endl;	\
	}

/*
	Author: Tobias Wallner
	tobias.wallner1@gmx.net
	
*/


#include <iostream>
#include <sstream>
#include "fix32.hpp"

#define TEST_CASE(function)										\
	if(function()){ 											\
		std::cout << "[  Ok  ] - " << #function << std::endl;	\
	}else{ 														\
		std::cout << "[Failed] - " << #function << std::endl;	\
	}


bool construct_fixpoint(){
	volatile fix32<16> a; //default construct
	volatile auto b = fix32<23>(1);
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
	
	result &= a == fix32<23>(5);
	result &= a != fix32<23>::reinterpret(5);
	result &= a == fix32<23>::reinterpret(5<<23);
	
	return result;
}

bool addition(){
	fix32<23> a = 5;
	fix32<23> b = 11;
	int32_t expected_result = (5 + 11) << 23;
	fix32<23> result = a + b;
	int32_t int_result = reinterpret_as_int32(result);
	return expected_result == int_result;
}

bool subtraction(){
	fix32<23> a = 5;
	fix32<23> b = 11;
	int32_t expected_result = (5 - 11) << 23;
	fix32<23> result = a - b;
	int32_t int_result = reinterpret_as_int32(result);
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
	int32_t int_result = reinterpret_as_int32(a);
	return expected_result == int_result;
}

bool self_subtraction(){
	fix32<23> a = 5;
	fix32<23> b = 11;
	int32_t expected_result = (5 - 11) << 23;
	a -= b;
	int32_t int_result = reinterpret_as_int32(a);
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

bool construct_from_string(){
	constexpr fix32<10> a("3.1415");
	fix32<10> a_lower(3.140);
	fix32<10> a_upper(3.142);
	return a_lower < a && a < a_upper;
}

bool construct_from_signed_string() {
	constexpr fix32<20> b("-3.1415");
	fix32<20> b_lower(-3.142);
	fix32<20> b_upper(-3.140);
	return b_lower < b && b < b_upper;
}

bool construct_from_binary_string() {
	constexpr int32_t iexpected = (0b10101010) << (16-4);
	constexpr fix32<16> expected = fix32<16>::reinterpret(iexpected);
	
	constexpr fix32<16> value1("0b1010.1010");
	constexpr fix32<16> value2("1010.1010", 2);
	constexpr fix32<16> value3("0b1010.1010", 2);
	
	bool test1 = value1 == expected;
	bool test2 = value2 == expected;
	bool test3 = value3 == expected;
	
	return test1 && test2 && test3;
}

bool construct_from_hex_string(){
	constexpr int32_t iexpected = (0x12AB56FE);
	
	constexpr fix32<16> value1("0x12AB.56FE");
	constexpr fix32<16> value2("12AB.56FE", 16);

	constexpr fix32<20> value3("0x12a.b56fe");
	constexpr fix32<20> value4("12a.b56fe", 16);
	
	bool test1 = value1 == fix32<16>::reinterpret(iexpected);
	bool test2 = value2 == fix32<16>::reinterpret(iexpected);
	bool test3 = value3 == fix32<20>::reinterpret(iexpected);
	bool test4 = value4 == fix32<20>::reinterpret(iexpected);
	
	return test1 && test2 && test3 && test4;
}

bool construct_from_stringstream() {
	std::stringstream str("3.1415");
	fix32<10> a;
	str >> a;
	fix32<10> a_lower(3.140);
	fix32<10> a_upper(3.142);
	return a_lower < a && a < a_upper;
}

bool construct_from_signed_stringstream() {
	std::stringstream str("-3.1415");
	fix32<20> b;
	str >> b;

	fix32<20> b_lower(-3.142);
	fix32<20> b_upper(-3.140);

	return b_lower < b && b < b_upper;
}

bool construct_from_binary_stringstream() {
	constexpr int32_t iexpected = (0b10101010) << (16 - 4);
	constexpr fix32<16> expected = fix32<16>::reinterpret(iexpected);

	std::stringstream str("0b1010.1010");

	fix32<16> value;
	str >> value;

	return value == expected;
}

bool construct_from_hex_stringstream() {
	constexpr int32_t iexpected = (0x12AB56FE);

	std::stringstream str1("0x12AB.56FE");
	std::stringstream str2("0x12ab.56fe");


	fix32<16> value1;
	fix32<16> value2;
	
	str1 >> value1;
	str2 >> value2;

	bool test1 = value1 == fix32<16>::reinterpret(iexpected);
	bool test2 = value2 == fix32<16>::reinterpret(iexpected);

	return test1 && test2;
}

int main(){
	std::cout << "fix32 Tests:" << std::endl;
	std::cout << "---------------" << std::endl;
	
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
	
	TEST_CASE(construct_from_string);
	TEST_CASE(construct_from_signed_string);
	TEST_CASE(construct_from_binary_string);
	TEST_CASE(construct_from_hex_string);

	TEST_CASE(construct_from_stringstream);
	TEST_CASE(construct_from_signed_stringstream);
	TEST_CASE(construct_from_binary_stringstream);
	TEST_CASE(construct_from_hex_stringstream);
	
	return 0;
}
