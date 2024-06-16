#pragma once
/*

	Author: Tobias Wallner
	tobias.wallner1@gmx.net
	
*/


#include <cstddef>
#include <cinttypes>
#include <type_traits>

template<size_t fractional_bits>
class fix64{
private:
	int64_t value;
	
public:
	inline fix64() = default;
	inline fix64(const fix64&) = default;
	inline fix64(int64_t number) : value(number << fractional_bits){}
	inline fix64(uint64_t number) : fix64(static_cast<int64_t>(number)){}
	
	template<typename Integer, std::enable_if_t<std::is_integral<Integer>::value, bool> = true>
	fix64(Integer number) : fix64(static_cast<int64_t>(number)){}

	inline fix64(float num) {
		if (num != 0.f) {
			const int64_t inum = *reinterpret_cast<const uint64_t*>(&num);
			const int64_t float_mantissa = (inum & 0x07FFFFF) | 0x0800000;
			const int64_t float_exponent = ((inum & 0x7F800000) >> 23) - 127;
			const bool float_sign = (*reinterpret_cast<int64_t*>(&num) & 0x80000000) != 0;
			const int64_t shifts = fractional_bits - 23 + float_exponent;
			const int64_t abs_value = (shifts >= 0) ? float_mantissa << shifts : float_mantissa >> -shifts;
			this->value = (float_sign) ? -abs_value : abs_value;
		}
		else {
			this->value = 0;
		}
	}
	
	inline fix64(double num) : fix64(static_cast<float>(num)){}
	
	template<size_t other_frac_bits>
	inline fix64(const fix64<other_frac_bits>& other){
		if (fractional_bits >= other_frac_bits)
			this->value = other.reinterpret_as_int64_t() << static_cast<uint32_t>(fractional_bits - other_frac_bits);
		else
			this->value = other.reinterpret_as_int64_t() >> static_cast<uint32_t>(other_frac_bits - fractional_bits);
	}
	
	inline fix64& operator= (const fix64&) = default;
	
	// Arithmetic operators
	
	inline friend fix64 operator+ (fix64 lhs, fix64 rhs){
		fix64 result;
		result.value = lhs.value + rhs.value;
		return result;
	}
	
	inline friend fix64 operator- (fix64 a){
		fix64 result;
		result.value = -a.value;
		return result;
	}
	
	inline friend fix64 operator- (fix64 lhs, fix64 rhs){
		fix64 result;
		result.value = lhs.value - rhs.value;
		return result;
	}
	
	inline friend fix64 operator* (fix64 lhs, fix64 rhs){
		/*
			Perform the following multiplication:
				(a * 2^32 + b) * (c * 2^32 + d)
		*/
		const uint64_t a = (static_cast<uint64_t>(lhs.value) >> 32) & ((1ULL<<32)-1);
		const uint64_t b = static_cast<uint64_t>(lhs.value) & ((1ULL<<32)-1);
		
		const uint64_t c = (static_cast<uint64_t>(rhs.value) >> 32) & ((1ULL<<32)-1);
		const uint64_t d = static_cast<uint64_t>(rhs.value) & ((1ULL<<32)-1);
		
		const uint64_t ac = a * c;
		const uint64_t bc = b * c;
		const uint64_t ad = a * d;
		const uint64_t bd = b * d;
		
		const uint64_t ad_bc = bc + ad;
		const bool ad_bc_carrie = (ad_bc < bc || ad_bc < ad);
		
		const uint64_t ad_bc_upper = (ad_bc_upper >> 32) & ((1ULL<<32)-1) | (ad_bc_carrie ? (1ULL << 32) : (0ULL));
		const uint64_t ad_bc_lower = ad_bc & ((1ULL<<32)-1);
		
		const uint64_t ad_bc_lower_shifted = (ad_bc_lower << 32);
		const uint64_t result_lower = bd + ad_bc_lower_shifted;
		const bool result_carrie = (result_lower < bd || result_lower < ad_bc_lower_shifted);
		
		const uint64_t result_upper = ac + ad_bc_upper + result_carrie;
		
		const uint64_t result_value = (result_upper << (64 - fractional_bits)) | (result_lower >> fractional_bits);
		
		fix64 result = fix64::reinterpret(result_value);
		return result;
	}
	
	template<typename Integer, std::enable_if_t<std::is_integral<Integer>::value, bool> = true>
	inline friend fix64 operator* (fix64 lhs, Integer rhs){
		return fix64::reinterpret(lhs.value * static_cast<int64_t>(rhs));
	}

	template<typename Integer, std::enable_if_t<std::is_integral<Integer>::value, bool> = true>
	inline friend fix64 operator* (Integer lhs, fix64 rhs){
		return fix64::reinterpret(lhs * rhs.value);
	}

	inline friend fix64 operator/ (fix64 lhs, fix64 rhs){
		bool sign_lhs = lhs < 0;
		bool sign_rhs = rhs < 0;
		
		lhs = sign_lhs ? -lhs : lhs;
		rhs = sign_rhs ? -rhs : rhs;
		
		const uint64_t lower = static_cast<uint64_t>(lhs.value) << fractional_bits;
		const uint64_t upper = static_cast<uint64_t>(lhs.value) >> (64-fractional_bits);  
		
		const uint64_t a = upper >> 32;
		const uint64_t b = upper & ((1ULL<<32)-1);
		const uint64_t c = lower >> 32;
		const uint64_t d = lower & ((1ULL<<32)-1);
		
		const uint64_t div1 = a / rhs.value;
		const uint64_t rest1 = a % rhs.value;
		
		const uint64_t val2 = (rest1 << 32) | b;
		const uint64_t div2 = val2 / rhs.value;
		const uint64_t rest2 = val2 % rhs.value;
		
		const uint64_t val3 = (rest2 << 32) | c;
		const uint64_t div3 = val3 / rhs.value;
		const uint64_t rest3 = val3 % rhs.value;
		
		const uint64_t val4 = (rest3 << 32) | c;
		const uint64_t div4 = val4 / rhs.value;
		
		const uint64_t result_val_ = div4 + (div3 << 32); // + (div2 << 64) + (div1 << 96)
		
		const uint64_t result_val = (sign_lhs != sign_rhs) ? -result_val_ : result_val_;
		
		const fix64 result = fix64::reinterpret(result_val);
		return result;
	}
	
	template<typename Integer, std::enable_if_t<std::is_integral<Integer>::value, bool> = true>
	inline friend fix64 operator/ (fix64 lhs, Integer rhs){
		return fix64::reinterpret(lhs.value / static_cast<int64_t>(rhs));
	}

	inline fix64& operator+= (fix64 rhs){return *this = *this + rhs;}
	inline fix64& operator-= (fix64 rhs){return *this = *this - rhs;}
	inline fix64& operator*= (fix64 rhs){return *this = *this * rhs;}
	inline fix64& operator/= (fix64 rhs){return *this = *this / rhs;}
	
	template<typename Integer, std::enable_if_t<std::is_integral<Integer>::value, bool> = true>
	inline fix64& operator*= (Integer rhs){return this->value = this->value * static_cast<int64_t>(rhs);}

	template<typename Integer, std::enable_if_t<std::is_integral<Integer>::value, bool> = true>
	inline fix64& operator/= (Integer rhs){return this->value = this->value / static_cast<int64_t>(rhs);}

	// Comparison operators
	inline friend bool operator== (fix64 lhs, fix64 rhs){return lhs.value == rhs.value;}
	inline friend bool operator!= (fix64 lhs, fix64 rhs){return lhs.value != rhs.value;}
	inline friend bool operator< (fix64 lhs, fix64 rhs){return lhs.value < rhs.value;}
	inline friend bool operator> (fix64 lhs, fix64 rhs){return lhs.value > rhs.value;}
	inline friend bool operator<= (fix64 lhs, fix64 rhs){return lhs.value <= rhs.value;}
	inline friend bool operator>= (fix64 lhs, fix64 rhs){return lhs.value >= rhs.value;}
	
	static inline fix64 reinterpret(int64_t number){
		fix64 result;
		result.value = number;
		return result;
	}
	
	template<size_t other_frac_bits>
	explicit inline operator fix64<other_frac_bits> () {return fix64<other_frac_bits>(*this);}
	
	explicit inline operator int64_t (){return this->value >> fractional_bits;}
	explicit inline operator uint64_t (){return static_cast<uint64_t>(this->value >> fractional_bits);}
	
	explicit inline operator float (){
		 static float fnorm = 1.f / static_cast<float>(1 << fractional_bits);
		float fval = static_cast<float>(this->value);
		float result = fval * fnorm;
		return result;
	}
	
	explicit inline operator double (){
		 static double fnorm = 1.f / static_cast<double>(1 << fractional_bits);
		double fval = static_cast<double>(this->value);
		double result = fval * fnorm;
		return result;
	}
	
	inline int64_t static_cast_to_int64_t() const {return this->value >> fractional_bits;}
	inline int64_t reinterpret_as_int64_t() const {return this->value;}
	
	inline friend int64_t static_cast_to_int64_t(fix64 f){return f.value >> fractional_bits;}
	inline friend int64_t reinterpret_as_int64_t(fix64 f){return f.value;}
	
	template<class Stream>
	friend Stream& print(Stream& stream, fix64 f, size_t significant_places_after_comma=3) {
		if (f < 0) {
			stream << '-';
			f = -f;
		}
		uint64_t digits = f.value >> fractional_bits;
		uint64_t fractionals = f.value & ((1ULL << fractional_bits) - 1);

		int significant_places = 0;
		bool count_significant_enable = digits != 0;

		// print digits and decimal point
		stream << digits << '.';

		// print fractionals
		while (fractionals != 0 && significant_places < significant_places_after_comma) {
			significant_places += count_significant_enable;
			fractionals = fractionals * 10;
			uint64_t n = fractionals >> fractional_bits;
			count_significant_enable |= n != 0;
			fractionals = fractionals & ((1ULL << fractional_bits) - 1);
			char c = '0' + static_cast<char>(n);
			stream << c;
		}
		return stream;
	}


	template<class Stream>
	friend Stream& operator<<(Stream& stream, fix64 f){return print(stream, f);}
};