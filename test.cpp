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

// ------- fix32 -------

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

// ------------------- fix64 -----------------------

bool fix64_multiplication(){
	uint32_t a = 40522;
	uint32_t b = 30789;
	
	fix64<32> fix_a(a);
	fix64<32> fix_b(b);
	
	const auto expected = a*b;
	const auto result = fix_a * fix_b;

	return result == expected;
}

bool fix64_signed_multiplication(){
	int64_t a = -40522;
	int64_t b = 30789;
	
	fix64<32> fix_a(a);
	fix64<32> fix_b(b);
	
	const auto expected = a*b;
	const auto result = fix_a * fix_b;
	
	return result == expected;
}

bool fix64_signed_multiplication2(){
	int64_t a = 40522;
	int64_t b = -30789;
	
	fix64<32> fix_a(a);
	fix64<32> fix_b(b);
	
	const auto expected = a*b;
	const auto result = fix_a * fix_b;
	
	return result == expected;
}

bool fix64_negative_multiplication(){
	int64_t a = -40522;
	int64_t b = -30789;
	
	fix64<32> fix_a(a);
	fix64<32> fix_b(b);
	
	const auto expected = a*b;
	const auto result = fix_a * fix_b;
	
	return result == expected;
}

bool fix64_division(){
	double a = 40522.562;
	double b = 20209.48;
	
	fix64<32> fix_a(a);
	fix64<32> fix_b(b);
	
	const auto expected = a / b;
	const auto result = fix_a / fix_b;

	return (expected - 0.1) <= result && result <= (expected + 0.1);
}

bool fix64_signed_division(){
	double a = 40522.562;
	double b = -20209.48;
	
	fix64<32> fix_a(a);
	fix64<32> fix_b(b);
	
	const auto expected = a / b;
	const auto result = fix_a / fix_b;
	
	return (expected - 0.1) <= result && result <= (expected + 0.1);
}

bool fix64_signed_division2(){
	double a = 40522.562;
	double b = -20209.48;
	
	fix64<32> fix_a(a);
	fix64<32> fix_b(b);
	
	const auto expected = a / b;
	const auto result = fix_a / fix_b;
	
	return (expected - 0.1) <= result && result <= (expected + 0.1);
}

bool fix64_negative_division(){
	double a = -40522.562;
	double b = -20209.48;
	
	fix64<32> fix_a(a);
	fix64<32> fix_b(b);
	
	const auto expected = a / b;
	const auto result = fix_a / fix_b;
	
	return (expected - 0.1) <= result && result <= (expected + 0.1);
}

bool fix64_construct_from_string() {
	fix64<20> a("3.14159265");
	fix64<20> a_lower(3.140);
	fix64<20> a_upper(3.142);
	return a_lower < a && a < a_upper;
}

bool fix64_construct_from_signed_string() {
	constexpr fix64<40> b("-3.14159265");
	fix64<40> b_lower(-3.142);
	fix64<40> b_upper(-3.140);
	return b_lower < b && b < b_upper;
}

bool fix64_construct_from_binary_string() {
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

bool fix64_construct_from_hex_string() {
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

bool fix64_construct_from_stringstream() {
	std::stringstream str("3.1415");
	fix64<10> a;
	str >> a;
	fix64<10> a_lower(3.140);
	fix64<10> a_upper(3.142);
	return a_lower < a && a < a_upper;
}

bool fix64_construct_from_signed_stringstream() {
	std::stringstream str("-3.1415");
	fix64<20> b;
	str >> b;

	fix64<20> b_lower(-3.142);
	fix64<20> b_upper(-3.140);

	return b_lower < b && b < b_upper;
}

bool fix64_construct_from_binary_stringstream() {
	constexpr int32_t iexpected = (0b10101010) << (16 - 4);
	constexpr fix64<16> expected = fix64<16>::reinterpret(iexpected);

	std::stringstream str("0b1010.1010");

	fix64<16> value;
	str >> value;

	return value == expected;
}

bool fix64_construct_from_hex_stringstream() {
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

// ------- examples -------

void example1(){
	std::cout << "example1:" << std::endl;
	fix32<8> a = 3.1415926535897932f;          // 24 integer bits, 8 fractional bits
	fix32<8> b = 1.5f;
	fix32<8> c = a + b;
	std::cout << "Result: " << c << std::endl; // Output> Result: 4.640
}

void example2(){
	std::cout << "example2:" << std::endl;
	fix64<42> a = 3.1415926535897932;          // 16 integer bits, 48 fractional bits 
	fix64<42> b = 1.5;
	fix64<42> c = a * b;
	std::cout << "Result: " << c << std::endl; // Output> Result: 4.742 
	std::cout << "Result: "; 
	print(std::cout, c, 8) << std::endl;       // Output> Result: 4.74218380
}

// ------- fixmath -------
/*
bool test_abs(){
	fix32<16> a(5);
	fix32<16> b(-5);
	return a != b && a == abs(b) && abs(a) == abs(b);
}
*/

int main(){
	
	example1();
	example2();
	
	std::cout << "Fixpoint Tests:" << std::endl;
	std::cout << "---------------" << std::endl;
	
	std::cout << "float test: " << (fix32<20>(420)/fix32<20>(507)) << std::endl;
	
	// ------- fix32 -------
	
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
	
	// ------- fix64 -------
	
	TEST_CASE(fix64_multiplication);
	TEST_CASE(fix64_signed_multiplication);
	TEST_CASE(fix64_negative_multiplication);
	TEST_CASE(fix64_signed_multiplication2);
	
	TEST_CASE(fix64_division);
	TEST_CASE(fix64_signed_division);
	TEST_CASE(fix64_signed_division2);
	TEST_CASE(fix64_negative_division);

	TEST_CASE(fix64_construct_from_string);
	TEST_CASE(fix64_construct_from_signed_string) 
	TEST_CASE(fix64_construct_from_binary_string);
	TEST_CASE(fix64_construct_from_hex_string);

	TEST_CASE(fix64_construct_from_stringstream);
	TEST_CASE(fix64_construct_from_signed_stringstream);
	TEST_CASE(fix64_construct_from_binary_stringstream);
	TEST_CASE(fix64_construct_from_hex_stringstream);
	
	// ------- fixmath -------
	
	//TEST_CASE(test_abs)
	
	return 0;
}