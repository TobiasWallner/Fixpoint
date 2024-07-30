/*
	Author: Tobias Wallner
	tobias.wallner1@gmx.net
	
*/


#include <iostream>
#include <sstream>
#include <cmath>
#include "fixmath.hpp"

#define print_result(value) std::cout << #value << " = " << (value) << std::endl;

#define TEST_CASE(function)										\
	if(function()){ 											\
		std::cout << "[  Ok  ] - " << #function << std::endl;	\
	}else{ 														\
		std::cout << "[Failed] - " << #function << std::endl;	\
	}

// ------------- abs -------------

bool test32_abs(){
	const fix32<16> a(5);
	const fix32<16> b(-5);
	return a != b && a == abs(b) && abs(a) == abs(b);
}

bool test64_abs(){
	const fix64<32> a(5);
	const fix64<32> b(-5);
	return a != b && a == abs(b) && abs(a) == abs(b);
}

// ================ Rounding ================
// ---------------- round_down ----------------

bool test32_round_dow_p(){
	const fix32<17> a(19.5);
	const fix32<17> b(19.9);
	const fix32<17> c(20.0);
	const fix32<17> d(20.1);
	const fix32<17> e(20.2);
	
	const bool test_a = round_down(a) == 19;
	const bool test_b = round_down(b) == 19;
	const bool test_c = round_down(c) == 20;
	const bool test_d = round_down(d) == 20;
	const bool test_e = round_down(e) == 20;
	
	return test_a && test_b && test_c && test_d && test_e;
}

bool test32_round_dow_m(){
	const fix32<17> a(-19.5);
	const fix32<17> b(-19.9);
	const fix32<17> c(-20.0);
	const fix32<17> d(-20.1);
	const fix32<17> e(-20.2);
	
	const bool test_a = round_down(a) == -20;
	const bool test_b = round_down(b) == -20;
	const bool test_c = round_down(c) == -20;
	const bool test_d = round_down(d) == -21;
	const bool test_e = round_down(e) == -21;
	
	return test_a && test_b && test_c && test_d && test_e;
}

bool test64_round_dow_p(){
	const fix32<17> a(19.5);
	const fix32<17> b(19.9);
	const fix32<17> c(20.0);
	const fix32<17> d(20.1);
	const fix32<17> e(20.2);
	
	const bool test_a = round_down(a) == 19;
	const bool test_b = round_down(b) == 19;
	const bool test_c = round_down(c) == 20;
	const bool test_d = round_down(d) == 20;
	const bool test_e = round_down(e) == 20;
	
	return test_a && test_b && test_c && test_d && test_e;
}

bool test64_round_dow_m(){
	const fix64<39> a(-19.5);
	const fix64<39> b(-19.9);
	const fix64<39> c(-20.0);
	const fix64<39> d(-20.1);
	const fix64<39> e(-20.2);
	
	const bool test_a = round_down(a) == -20;
	const bool test_b = round_down(b) == -20;
	const bool test_c = round_down(c) == -20;
	const bool test_d = round_down(d) == -21;
	const bool test_e = round_down(e) == -21;
	
	return test_a && test_b && test_c && test_d && test_e;
}

// ---------------- floor ----------------

bool test32_floor_p(){
	const fix32<17> a(19.5);
	const fix32<17> b(19.9);
	const fix32<17> c(20.0);
	const fix32<17> d(20.1);
	const fix32<17> e(20.2);
	
	const bool test_a = round_down(a) == std::floor(19.5);
	const bool test_b = round_down(b) == std::floor(19.9);
	const bool test_c = round_down(c) == std::floor(20.0);
	const bool test_d = round_down(d) == std::floor(20.1);
	const bool test_e = round_down(e) == std::floor(20.2);
	
	return test_a && test_b && test_c && test_d && test_e;
}

bool test32_floor_m(){
	const fix32<17> a(-19.5);
	const fix32<17> b(-19.9);
	const fix32<17> c(-20.0);
	const fix32<17> d(-20.1);
	const fix32<17> e(-20.2);
	
	const bool test_a = round_down(a) == std::floor(-19.5);
	const bool test_b = round_down(b) == std::floor(-19.9);
	const bool test_c = round_down(c) == std::floor(-20.0);
	const bool test_d = round_down(d) == std::floor(-20.1);
	const bool test_e = round_down(e) == std::floor(-20.2);
	
	return test_a && test_b && test_c && test_d && test_e;
}

bool test64_floor_p(){
	const fix32<17> a(19.5);
	const fix32<17> b(19.9);
	const fix32<17> c(20.0);
	const fix32<17> d(20.1);
	const fix32<17> e(20.2);
	
	const bool test_a = round_down(a) == std::floor(19.5);
	const bool test_b = round_down(b) == std::floor(19.9);
	const bool test_c = round_down(c) == std::floor(20.0);
	const bool test_d = round_down(d) == std::floor(20.1);
	const bool test_e = round_down(e) == std::floor(20.2);
	
	return test_a && test_b && test_c && test_d && test_e;
}

bool test64_floor_m(){
	const fix64<39> a(-19.5);
	const fix64<39> b(-19.9);
	const fix64<39> c(-20.0);
	const fix64<39> d(-20.1);
	const fix64<39> e(-20.2);
	
	const bool test_a = round_down(a) == std::floor(-19.5);
	const bool test_b = round_down(b) == std::floor(-19.9);
	const bool test_c = round_down(c) == std::floor(-20.0);
	const bool test_d = round_down(d) == std::floor(-20.1);
	const bool test_e = round_down(e) == std::floor(-20.2);
	
	return test_a && test_b && test_c && test_d && test_e;
}

// ---------------- round_up ----------------

bool test32_round_up_p(){
	const fix32<17> a(19.5);
	const fix32<17> b(19.9);
	const fix32<17> c(20.0);
	const fix32<17> d(20.1);
	const fix32<17> e(20.2);
	
	const bool test_a = round_up(a) == 20;
	const bool test_b = round_up(b) == 20;
	const bool test_c = round_up(c) == 20;
	const bool test_d = round_up(d) == 21;
	const bool test_e = round_up(e) == 21;
	
	return test_a && test_b && test_c && test_d && test_e;
}

bool test32_round_up_m(){
	const fix32<17> a(-19.5);
	const fix32<17> b(-19.9);
	const fix32<17> c(-20.0);
	const fix32<17> d(-20.1);
	const fix32<17> e(-20.2);
	
	const bool test_a = round_up(a) == -19;
	const bool test_b = round_up(b) == -19;
	const bool test_c = round_up(c) == -20;
	const bool test_d = round_up(d) == -20;
	const bool test_e = round_up(e) == -20;
	
	return test_a && test_b && test_c && test_d && test_e;
}

bool test64_round_up_p(){
	const fix32<17> a(19.5);
	const fix32<17> b(19.9);
	const fix32<17> c(20.0);
	const fix32<17> d(20.1);
	const fix32<17> e(20.2);
	
	const bool test_a = round_up(a) == 20;
	const bool test_b = round_up(b) == 20;
	const bool test_c = round_up(c) == 20;
	const bool test_d = round_up(d) == 21;
	const bool test_e = round_up(e) == 21;
	
	return test_a && test_b && test_c && test_d && test_e;
}

bool test64_round_up_m(){
	const fix64<39> a(-19.5);
	const fix64<39> b(-19.9);
	const fix64<39> c(-20.0);
	const fix64<39> d(-20.1);
	const fix64<39> e(-20.2);
	
	const bool test_a = round_up(a) == -19;
	const bool test_b = round_up(b) == -19;
	const bool test_c = round_up(c) == -20;
	const bool test_d = round_up(d) == -20;
	const bool test_e = round_up(e) == -20;
	
	return test_a && test_b && test_c && test_d && test_e;
}

// ---------------- ceil ----------------

bool test32_ceil_p(){
	const fix32<17> a(19.5);
	const fix32<17> b(19.9);
	const fix32<17> c(20.0);
	const fix32<17> d(20.1);
	const fix32<17> e(20.2);
	
	const bool test_a = round_up(a) == std::ceil(19.5);
	const bool test_b = round_up(b) == std::ceil(19.9);
	const bool test_c = round_up(c) == std::ceil(20.0);
	const bool test_d = round_up(d) == std::ceil(20.1);
	const bool test_e = round_up(e) == std::ceil(20.2);
	
	return test_a && test_b && test_c && test_d && test_e;
}

bool test32_ceil_m(){
	const fix32<17> a(-19.5);
	const fix32<17> b(-19.9);
	const fix32<17> c(-20.0);
	const fix32<17> d(-20.1);
	const fix32<17> e(-20.2);
	
	const bool test_a = round_up(a) == std::ceil(-19.5);
	const bool test_b = round_up(b) == std::ceil(-19.9);
	const bool test_c = round_up(c) == std::ceil(-20.0);
	const bool test_d = round_up(d) == std::ceil(-20.1);
	const bool test_e = round_up(e) == std::ceil(-20.2);
	
	return test_a && test_b && test_c && test_d && test_e;
}

bool test64_ceil_p(){
	const fix32<17> a(19.5);
	const fix32<17> b(19.9);
	const fix32<17> c(20.0);
	const fix32<17> d(20.1);
	const fix32<17> e(20.2);
	
	const bool test_a = round_up(a) == std::ceil(19.5);
	const bool test_b = round_up(b) == std::ceil(19.9);
	const bool test_c = round_up(c) == std::ceil(20.0);
	const bool test_d = round_up(d) == std::ceil(20.1);
	const bool test_e = round_up(e) == std::ceil(20.2);
	
	return test_a && test_b && test_c && test_d && test_e;
}

bool test64_ceil_m(){
	const fix64<39> a(-19.5);
	const fix64<39> b(-19.9);
	const fix64<39> c(-20.0);
	const fix64<39> d(-20.1);
	const fix64<39> e(-20.2);
	
	const bool test_a = round_up(a) == std::ceil(-19.5);
	const bool test_b = round_up(b) == std::ceil(-19.9);
	const bool test_c = round_up(c) == std::ceil(-20.0);
	const bool test_d = round_up(d) == std::ceil(-20.1);
	const bool test_e = round_up(e) == std::ceil(-20.2);
	
	return test_a && test_b && test_c && test_d && test_e;
}

// ------------- mod -------------

bool test32_mod_pp(){
	const fix32<16> a(30584.5);
	const fix32<16> b(13);
	const fix32<16> m = mod(a, b);
	
	const fix32<16> expected = a - round_down(a / b) * b;
	
	return m == expected;
}

bool test32_mod_mp(){
	const fix32<16> a(-30584.5);
	const fix32<16> b(13);
	const fix32<16> m = mod(a, b);
	
	const fix32<16> expected = a - round_down(a / b) * b;
	
	return m == expected;
}

bool test32_mod_pm(){
	const fix32<16> a(30584.5);
	const fix32<16> b(-13);
	const fix32<16> m = mod(a, b);
	
	const fix32<16> expected = a - round_down(a / b) * b;
	
	return m == expected;
}

bool test32_mod_mm(){
	const fix32<16> a(-30584.5);
	const fix32<16> b(-13);
	const fix32<16> m = mod(a, b);
	
	const fix32<16> expected = a - round_down(a / b) * b;
	
	return m == expected;
}


bool test64_mod_pp(){
	const fix64<41> a(30584.5);
	const fix64<41> b(13);
	const fix64<41> m = mod(a, b);

	const fix64<41> expected = a - round_down(a / b) * b;
	
	return m == expected;
}

bool test64_mod_mp(){
	const fix64<41> a(-30584.5);
	const fix64<41> b(13);
	const fix64<41> m = mod(a, b);
	
	const fix64<41> expected = a - round_down(a / b) * b;
	
	return m == expected;
}

bool test64_mod_pm(){
	const fix64<41> a(30584.5);
	const fix64<41> b(-13);
	const fix64<41> m = mod(a, b);
	
	const fix64<41> expected = a - round_down(a / b) * b;
	
	return m == expected;
}

bool test64_mod_mm(){
	const fix64<41> a(-30584.5);
	const fix64<41> b(-13);
	const fix64<41> m = mod(a, b);
	
	const fix64<41> expected = a - round_down(a / b) * b;
	
	return m == expected;
}

int main(){
	
	std::cout << "fixmath tests:" << std::endl;
	std::cout << "---------------" << std::endl;
	
	TEST_CASE(test32_abs);
	TEST_CASE(test64_abs);
	
	TEST_CASE(test32_round_dow_p);
	TEST_CASE(test32_round_dow_m);
	TEST_CASE(test64_round_dow_p);
	TEST_CASE(test64_round_dow_m);
	
	TEST_CASE(test32_floor_p);
	TEST_CASE(test32_floor_m);
	TEST_CASE(test64_floor_p);
	TEST_CASE(test64_floor_m);
	
	TEST_CASE(test32_round_up_p);
	TEST_CASE(test32_round_up_m);
	TEST_CASE(test64_round_up_p);
	TEST_CASE(test64_round_up_m);
	
	TEST_CASE(test32_ceil_p);
	TEST_CASE(test32_ceil_m);
	TEST_CASE(test64_ceil_p);
	TEST_CASE(test64_ceil_m);
	
	TEST_CASE(test32_mod_pp);
	TEST_CASE(test32_mod_mp);
	TEST_CASE(test32_mod_pm);
	TEST_CASE(test32_mod_mm);
	
	TEST_CASE(test64_mod_pp);
	TEST_CASE(test64_mod_mp);
	TEST_CASE(test64_mod_pm);
	TEST_CASE(test64_mod_mm);
	
	
	
	return 0;
}