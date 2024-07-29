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
	class ReinterpretToken{};

	constexpr fix64() = default;
	constexpr fix64(const fix64&) = default;
	constexpr fix64(int64_t number) : value(number << fractional_bits){}
	constexpr fix64(int64_t number, ReinterpretToken t) : value(number){}
	
	template<typename Integer, std::enable_if_t<std::is_integral<Integer>::value, bool> = true>
	fix64(Integer number) : fix64(static_cast<int64_t>(number)){}
	
	template<typename Integer, std::enable_if_t<std::is_integral<Integer>::value, bool> = true>
	fix64(Integer number, ReinterpretToken t) : fix64(static_cast<int64_t>(number), t){}

	inline fix64(float num) {
		if (num != 0.f) {
			const uint32_t inum = *reinterpret_cast<const uint32_t*>(&num);
			const uint32_t float_mantissa = (inum & ((1 << 23) - 1)) | (1 << 23);
			const uint32_t float_exponent = ((inum & (((1 << 8) - 1) << 23)) >> 23) - 127;
			const bool float_sign = (inum & (1 << 31)) != 0;
			const int shifts = (static_cast<int>(fractional_bits) + static_cast<int>(float_exponent)) - static_cast<int>(23);
			const uint64_t abs_value = (shifts >= 0) ? static_cast<uint64_t>(float_mantissa) << shifts : static_cast<uint64_t>(float_mantissa) >> -shifts;
			this->value = (float_sign) ? -abs_value : abs_value;
		}
		else {
			this->value = 0;
		}
	}
	
	inline fix64(double num) : fix64(static_cast<float>(num)){}
	
	constexpr fix64(const char* str, int radix = 10) : value(0) {
		bool sign = false;
		uint64_t digits = 0;
		uint64_t fractions = 0;
		char const * const str_first = str;
		
		// parse sign
		if (*str == '-') {
			sign = true;
			++str;
		}

		//select radix
		if (*str == '0') {
			++str;
			switch (*str) {
				case 'b': {++str; radix = 2; } break;
				case 'o': {++str; radix = 8; } break;
				case 'd': {++str; radix = 10; } break;
				case 'x': case 'X': {++str; radix = 16; } break;
				default: break;
			}
		}

		// select ranges
		char digit_first = '0';
		char digit_last = '0' + ((radix <= 10) ? (radix) : 10);
		char alpha_first = 'a';
		char alpha_last = 'a' + ((radix > 10) ? radix - 10 : 0);
		char ALPHA_first = 'A';
		char ALPHA_last = 'A' + ((radix > 10) ? radix - 10 : 0);


		// parse digits
		while (true) {
			if (digit_first <= *str && *str <= digit_last) {
				digits = digits * radix + (*str - digit_first);
			}else if (alpha_first <= *str && *str <= alpha_last) {
				digits = digits * radix + (*str - alpha_first + 10);
			}else if (ALPHA_first <= *str && *str <= ALPHA_last) {
				digits = digits * radix + (*str - ALPHA_first + 10);
			}else {
				break;
			}
			++str;
		}
		digits <<= fractional_bits;

		// parse fractions
		if (*str == '.') {
			++str;
			size_t s = radix;
			while (true) {
				if (digit_first <= *str && *str <= digit_last) {
					fractions = fractions + ((static_cast<uint64_t>(*str) - static_cast<uint64_t>(digit_first)) << 60) / s;
				}else if (alpha_first <= *str && *str <= alpha_last) {
					fractions = fractions + ((static_cast<uint64_t>(*str) - static_cast<uint64_t>(alpha_first) + 10) << 60) / s;
				}else if (ALPHA_first <= *str && *str <= ALPHA_last) {
					fractions = fractions + ((static_cast<uint64_t>(*str) - static_cast<uint64_t>(ALPHA_first) + 10) << 60) / s;
				}else {
					break;
				}
				++str;
				size_t new_s = s * radix;
				s = (new_s > s) ? new_s : 0; //overflow protection
			}

			// shift fractions to the correct binary point
			int shifts = static_cast<int>(60) - static_cast<int>(fractional_bits);
			fractions = (shifts >= 0) ? fractions >> shifts : fractions << -shifts;
		}

		const uint64_t abs_value = digits | fractions;
		const int64_t result = sign ? (-static_cast<int64_t>(abs_value)) : static_cast<int64_t>(abs_value);
		this->value = result;
	}

	inline fix64& assign(const char* str, int radix = 10) {
		return *this = fix64<fractional_bits>(str, radix);
	}
	
	template<size_t other_frac_bits>
	constexpr fix64(const fix64<other_frac_bits>& other){
		if (fractional_bits >= other_frac_bits)
			this->value = other.reinterpret_as_int64_t() << static_cast<uint32_t>(fractional_bits - other_frac_bits);
		else
			this->value = other.reinterpret_as_int64_t() >> static_cast<uint32_t>(other_frac_bits - fractional_bits);
	}
	
	inline fix64& operator= (const fix64&) = default;
	
	// Arithmetic operators
	
	constexpr friend fix64 operator+ (fix64 lhs, fix64 rhs){
		fix64 result;
		result.value = lhs.value + rhs.value;
		return result;
	}
	
	constexpr friend fix64 operator- (fix64 a){
		fix64 result;
		result.value = -a.value;
		return result;
	}
	
	constexpr friend fix64 operator- (fix64 lhs, fix64 rhs){
		fix64 result;
		result.value = lhs.value - rhs.value;
		return result;
	}
	
	constexpr friend fix64 operator* (fix64 lhs, fix64 rhs){
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
		
		const uint64_t ad_bc_upper = (ad_bc >> 32) & ((1ULL<<32)-1) | (ad_bc_carrie ? (1ULL << 32) : (0ULL));
		const uint64_t ad_bc_lower = ad_bc & ((1ULL<<32)-1);
		
		const uint64_t ad_bc_lower_shifted = (ad_bc_lower << 32);
		const uint64_t result_lower = bd + ad_bc_lower_shifted;
		const bool result_carrie = (result_lower < bd || result_lower < ad_bc_lower_shifted);
		
		const uint64_t result_upper = ac + ad_bc_upper + result_carrie;
		
		const uint64_t result_value = (result_upper << (64 - fractional_bits)) | (result_lower >> fractional_bits);
		
		const fix64 result = fix64::reinterpret(result_value);
		return result;
	}
	
	template<typename Integer, std::enable_if_t<std::is_integral<Integer>::value, bool> = true>
	constexpr friend fix64 operator* (fix64 lhs, Integer rhs){
		return fix64::reinterpret(lhs.value * static_cast<int64_t>(rhs));
	}

	template<typename Integer, std::enable_if_t<std::is_integral<Integer>::value, bool> = true>
	constexpr friend fix64 operator* (Integer lhs, fix64 rhs){
		return fix64::reinterpret(lhs * rhs.value);
	}

	constexpr friend fix64 operator/ (fix64 lhs, fix64 rhs){
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
	constexpr friend fix64 operator/ (fix64 lhs, Integer rhs){
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
	constexpr friend bool operator== (fix64 lhs, fix64 rhs){return lhs.value == rhs.value;}
	constexpr friend bool operator!= (fix64 lhs, fix64 rhs){return lhs.value != rhs.value;}
	constexpr friend bool operator< (fix64 lhs, fix64 rhs){return lhs.value < rhs.value;}
	constexpr friend bool operator> (fix64 lhs, fix64 rhs){return lhs.value > rhs.value;}
	constexpr friend bool operator<= (fix64 lhs, fix64 rhs){return lhs.value <= rhs.value;}
	constexpr friend bool operator>= (fix64 lhs, fix64 rhs){return lhs.value >= rhs.value;}
	
	static constexpr fix64 reinterpret(int64_t number){return fix64(number, ReinterpretToken());}
	
	template<size_t other_frac_bits>
	explicit constexpr operator fix64<other_frac_bits> () const {return fix64<other_frac_bits>(*this);}
	
	explicit constexpr operator int64_t () const {return this->value >> fractional_bits;}
	explicit constexpr operator uint64_t () const {return static_cast<uint64_t>(this->value >> fractional_bits);}
	
	explicit constexpr operator float () const {
		float result = static_cast<float>(this->value) / (1 << fractional_bits);
		return result;
	}
	
	explicit constexpr operator double () const {
		double result = static_cast<double>(this->value) / (1 << fractional_bits);
		return result;
	}
	
	constexpr int64_t static_cast_to_int64_t() const {return this->value >> fractional_bits;}
	constexpr int64_t reinterpret_as_int64_t() const {return this->value;}
	
	constexpr friend int64_t static_cast_to_int64_t(fix64 f){return f.value >> fractional_bits;}
	constexpr friend int64_t reinterpret_as_int64_t(fix64 f){return f.value;}
	
	template<class Stream>
	friend Stream& print(Stream& stream, fix64<fractional_bits> f, size_t significant_places_after_comma=3) {
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
	friend inline Stream& operator<<(Stream& stream, fix64 f){return print(stream, f);}

	template<class Stream>
	friend Stream& operator>>(Stream& stream, fix64& f){
		bool sign = false;
		uint64_t digits = 0;
		uint64_t fractions = 0;
		int radix = 10;
		
		// parse sign
		if (stream.peek() == '-') {
			sign = true;
			stream.get();
		}

		//select radix
		if (stream.peek() == '0') {
			stream.get();
			switch (stream.peek()) {
				case 'b': {stream.get(); radix = 2; } break;
				case 'o': {stream.get(); radix = 8; } break;
				case 'd': {stream.get(); radix = 10; } break;
				case 'x': case 'X': {stream.get(); radix = 16; } break;
				default: break;
			}
		}

		// select ranges
		char digit_first = '0';
		char digit_last = '0' + ((radix <= 10) ? (radix) : 10);
		char alpha_first = 'a';
		char alpha_last = 'a' + ((radix > 10) ? radix - 10 : 0);
		char ALPHA_first = 'A';
		char ALPHA_last = 'A' + ((radix > 10) ? radix - 10 : 0);


		// parse digits
		while (true) {
			if (digit_first <= stream.peek() && stream.peek() <= digit_last) {
				digits = digits * radix + (stream.peek() - digit_first);
			}else if (alpha_first <= stream.peek() && stream.peek() <= alpha_last) {
				digits = digits * radix + (stream.peek() - alpha_first + 10);
			}else if (ALPHA_first <= stream.peek() && stream.peek() <= ALPHA_last) {
				digits = digits * radix + (stream.peek() - ALPHA_first + 10);
			}else {
				break;
			}
			stream.get();
		}
		digits <<= fractional_bits;

		// parse fractions
		if (stream.peek() == '.') {
			stream.get();
			size_t s = radix;
			while (true) {
				if (digit_first <= stream.peek() && stream.peek() <= digit_last) {
					fractions = fractions + ((static_cast<uint64_t>(stream.peek()) - static_cast<uint64_t>(digit_first)) << 60) / s;
				}else if (alpha_first <= stream.peek() && stream.peek() <= alpha_last) {
					fractions = fractions + ((static_cast<uint64_t>(stream.peek()) - static_cast<uint64_t>(alpha_first) + 10) << 60) / s;
				}else if (ALPHA_first <= stream.peek() && stream.peek() <= ALPHA_last) {
					fractions = fractions + ((static_cast<uint64_t>(stream.peek()) - static_cast<uint64_t>(ALPHA_first) + 10) << 60) / s;
				}else {
					break;
				}
				stream.get();
				size_t new_s = s * radix;
				s = (new_s > s) ? new_s : 0; //overflow protection
			}

			// shift fractions to the correct binary point
			int shifts = static_cast<int>(60) - static_cast<int>(fractional_bits);
			fractions = (shifts >= 0) ? fractions >> shifts : fractions << -shifts;
		}

		const uint64_t abs_value = digits | fractions;
		const int64_t value = sign ? (-static_cast<int64_t>(abs_value)) : static_cast<int64_t>(abs_value);
		
		f = fix64::reinterpret(value);
		return stream;
	}
};