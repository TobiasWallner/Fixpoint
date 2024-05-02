/*
	Author: Tobias Wallner
	tobias.wallner1@gmx.net
	
*/

#pragma once

#include <iostream>
#include <cinttypes>

#if __cplusplus >= 201703L
	#define FIX32_IF_CONSTEXPR constexpr
#else
	#define FIX32_IF_CONSTEXPR 
#endif

template<size_t fractional_bits>
class fix32{
private:
	int32_t value;
	
public:
	constexpr fix32() = default;
	constexpr fix32(const fix32&) = default;
	constexpr fix32(int32_t number){
		this->value = number << fractional_bits;
	}
	
	template<size_t other_frac_bits>
	constexpr fix32(const fix32<other_frac_bits>& other){
		if FIX32_IF_CONSTEXPR (fractional_bits >= other_frac_bits)
			this->value = other.reinterpret_as_int32_t() << static_cast<size_t>(fractional_bits - other_frac_bits);
		else
			this->value = other.reinterpret_as_int32_t() >> static_cast<size_t>(other_frac_bits - fractional_bits);
	}
	
	constexpr fix32& operator= (const fix32&) = default;
	
	// Arithmetic operators
	
	constexpr friend fix32 operator+ (fix32 lhs, fix32 rhs){
		fix32 result;
		result.value = lhs.value + rhs.value;
		return result;
	}
	
	constexpr friend fix32 operator- (fix32 lhs, fix32 rhs){
		fix32 result;
		result.value = lhs.value - rhs.value;
		return result;
	}
	
	constexpr friend fix32 operator* (fix32 lhs, fix32 rhs){
		int64_t temp = static_cast<int64_t>(lhs.value) * static_cast<int64_t>(rhs.value);
		return fix32::reinterpret(static_cast<int32_t>(temp >> fractional_bits));
	}
	
	constexpr friend fix32 operator/ (fix32 lhs, fix32 rhs){
		int64_t temp = (static_cast<int64_t>(lhs.value) << fractional_bits) / static_cast<int64_t>(rhs.value);
		return fix32::reinterpret(static_cast<int32_t>(temp));
	}
	
	constexpr fix32& operator+= (fix32 rhs){return *this = *this + rhs;}
	constexpr fix32& operator-= (fix32 rhs){return *this = *this - rhs;}
	constexpr fix32& operator*= (fix32 rhs){return *this = *this * rhs;}
	constexpr fix32& operator/= (fix32 rhs){return *this = *this / rhs;}
	
	// Comparison operators
	constexpr friend bool operator== (fix32 lhs, fix32 rhs){return lhs.value == rhs.value;}
	constexpr friend bool operator!= (fix32 lhs, fix32 rhs){return lhs.value != rhs.value;}
	constexpr friend bool operator< (fix32 lhs, fix32 rhs){return lhs.value < rhs.value;}
	constexpr friend bool operator> (fix32 lhs, fix32 rhs){return lhs.value > rhs.value;}
	constexpr friend bool operator<= (fix32 lhs, fix32 rhs){return lhs.value <= rhs.value;}
	constexpr friend bool operator>= (fix32 lhs, fix32 rhs){return lhs.value >= rhs.value;}
	
	// conversion
	static constexpr fix32 from(int32_t number){
		fix32 result;
		result.value = number << fractional_bits;
		return result;
	}
	
	static constexpr fix32 reinterpret(int32_t number){
		fix32 result;
		result.value = number;
		return result;
	}
	
	template<size_t other_frac_bits>
	explicit constexpr operator fix32<other_frac_bits> () {return fix32<other_frac_bits>(*this);}
	
	explicit constexpr operator int32_t (){return this->value >> fractional_bits;}
	
	explicit constexpr operator float (){
		constexpr static float fnorm = 1.f / static_cast<float>(1 << fractional_bits);
		float fval = static_cast<float>(this->value);
		float result = fval * fnorm;
		return result;
	}
	
	explicit constexpr operator double (){
		constexpr static double fnorm = 1.f / static_cast<double>(1 << fractional_bits);
		double fval = static_cast<double>(this->value);
		double result = fval * fnorm;
		return result;
	}
	
	constexpr int32_t static_cast_to_int32_t() const {return this->value >> fractional_bits;}
	constexpr int32_t reinterpret_as_int32_t() const {return this->value;}
	
	constexpr friend int32_t static_cast_to_int32_t(fix32 f){return f.value >> fractional_bits;}
	constexpr friend int32_t reinterpret_as_int32_t(fix32 f){return f.value;}
	
	inline friend std::ostream& operator<< (std::ostream& stream, fix32 value){
		return stream << static_cast<double>(value);
	}
};

#undef FIX32_IF_CONSTEXPR