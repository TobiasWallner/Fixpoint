/*
	Author: Tobias Wallner
	tobias.wallner1@gmx.net
	
*/


#include <iostream>
#include <sstream>
#include "fix64.hpp"

#define TEST_CASE(function)										\
	if(function()){ 											\
		std::cout << "[  Ok  ] - " << #function << std::endl;	\
	}else{ 														\
		std::cout << "[Failed] - " << #function << std::endl;	\
	}

bool multiplication(){
	uint32_t a = 40522;
	uint32_t b = 30789;
	
	fix64<32> fix_a(a);
	fix64<32> fix_b(b);
	
	const auto expected = a*b;
	const auto result = fix_a * fix_b;

	return result == expected;
}

bool signed_multiplication(){
	int64_t a = -40522;
	int64_t b = 30789;
	
	fix64<32> fix_a(a);
	fix64<32> fix_b(b);
	
	const auto expected = a*b;
	const auto result = fix_a * fix_b;
	
	return result == expected;
}

bool signed_multiplication2(){
	int64_t a = 40522;
	int64_t b = -30789;
	
	fix64<32> fix_a(a);
	fix64<32> fix_b(b);
	
	const auto expected = a*b;
	const auto result = fix_a * fix_b;
	
	return result == expected;
}

bool negative_multiplication(){
	int64_t a = -40522;
	int64_t b = -30789;
	
	fix64<32> fix_a(a);
	fix64<32> fix_b(b);
	
	const auto expected = a*b;
	const auto result = fix_a * fix_b;
	
	return result == expected;
}

bool division(){
	double a = 40522.562;
	double b = 20209.48;
	
	fix64<32> fix_a(a);
	fix64<32> fix_b(b);
	
	const auto expected = a / b;
	const auto result = fix_a / fix_b;

	return (expected - 0.1) <= result && result <= (expected + 0.1);
}

bool signed_division(){
	double a = 40522.562;
	double b = -20209.48;
	
	fix64<32> fix_a(a);
	fix64<32> fix_b(b);
	
	const auto expected = a / b;
	const auto result = fix_a / fix_b;
	
	return (expected - 0.1) <= result && result <= (expected + 0.1);
}

bool signed_division2(){
	double a = 40522.562;
	double b = -20209.48;
	
	fix64<32> fix_a(a);
	fix64<32> fix_b(b);
	
	const auto expected = a / b;
	const auto result = fix_a / fix_b;
	
	return (expected - 0.1) <= result && result <= (expected + 0.1);
}

bool negative_division(){
	double a = -40522.562;
	double b = -20209.48;
	
	fix64<32> fix_a(a);
	fix64<32> fix_b(b);
	
	const auto expected = a / b;
	const auto result = fix_a / fix_b;
	
	return (expected - 0.1) <= result && result <= (expected + 0.1);
}

bool construct_from_string() {
	fix64<20> a("3.14159265");
	fix64<20> a_lower(3.140);
	fix64<20> a_upper(3.142);
	return a_lower < a && a < a_upper;
}

bool construct_from_signed_string() {
	constexpr fix64<40> b("-3.14159265");
	fix64<40> b_lower(-3.142);
	fix64<40> b_upper(-3.140);
	return b_lower < b && b < b_upper;
}

bool construct_from_binary_string() {
	constexpr int32_t iexpected = (0b10101010) << (16 - 4);
	constexpr fix64<16> expected = fix64<16>::reinterpret(iexpected);

	constexpr fix64<16> value1("0b1010.1010");
	constexpr fix64<16> value2("1010.1010", 2);
	constexpr fix64<16> value3("0b1010.1010", 2);

	bool test1 = value1 == expected;
	bool test2 = value2 == expected;
	bool test3 = value3 == expected;

	return test1 && test2 && test3;
}

bool construct_from_hex_string() {
	constexpr int32_t iexpected = (0x12AB56FE);

	constexpr fix64<16> value1("0x12AB.56FE");
	constexpr fix64<16> value2("12AB.56FE", 16);

	constexpr fix64<20> value3("0x12a.b56fe");
	constexpr fix64<20> value4("12a.b56fe", 16);

	bool test1 = value1 == fix64<16>::reinterpret(iexpected);
	bool test2 = value2 == fix64<16>::reinterpret(iexpected);
	bool test3 = value3 == fix64<20>::reinterpret(iexpected);
	bool test4 = value4 == fix64<20>::reinterpret(iexpected);

	return test1 && test2 && test3 && test4;
}

bool construct_from_stringstream() {
	std::stringstream str("3.1415");
	fix64<10> a;
	str >> a;
	fix64<10> a_lower(3.140);
	fix64<10> a_upper(3.142);
	return a_lower < a && a < a_upper;
}

bool construct_from_signed_stringstream() {
	std::stringstream str("-3.1415");
	fix64<20> b;
	str >> b;

	fix64<20> b_lower(-3.142);
	fix64<20> b_upper(-3.140);

	return b_lower < b && b < b_upper;
}

bool construct_from_binary_stringstream() {
	constexpr int32_t iexpected = (0b10101010) << (16 - 4);
	constexpr fix64<16> expected = fix64<16>::reinterpret(iexpected);

	std::stringstream str("0b1010.1010");

	fix64<16> value;
	str >> value;

	return value == expected;
}

bool construct_from_hex_stringstream() {
	constexpr int64_t iexpected = (0x12AB56FE0000ULL);

	std::stringstream str1("0x12AB.56FE");
	std::stringstream str2("0x12ab.56fe");


	fix64<32> value1;
	fix64<32> value2;

	str1 >> value1;
	str2 >> value2;

	bool test1 = value1 == fix64<32>::reinterpret(iexpected);
	bool test2 = value2 == fix64<32>::reinterpret(iexpected);

	return test1 && test2;
}

int main(){
	
	std::cout << "fix64 tests:" << std::endl;
	std::cout << "---------------" << std::endl;
	
	TEST_CASE(multiplication);
	TEST_CASE(signed_multiplication);
	TEST_CASE(negative_multiplication);
	TEST_CASE(signed_multiplication2);
	
	TEST_CASE(division);
	TEST_CASE(signed_division);
	TEST_CASE(signed_division2);
	TEST_CASE(negative_division);

	TEST_CASE(construct_from_string);
	TEST_CASE(construct_from_signed_string) 
	TEST_CASE(construct_from_binary_string);
	TEST_CASE(construct_from_hex_string);

	TEST_CASE(construct_from_stringstream);
	TEST_CASE(construct_from_signed_stringstream);
	TEST_CASE(construct_from_binary_stringstream);
	TEST_CASE(construct_from_hex_stringstream);
	
	return 0;
}