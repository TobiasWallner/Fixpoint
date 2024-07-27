#pragma once
/*

	Author: Tobias Wallner
	tobias.wallner1@gmx.net
	
*/


#include <cstddef>
#include <cinttypes>
#include <type_traits>

template<size_t fractional_bits>
class fix32{
private:
	int32_t value;
	
public:
	class ReinterpretToken{};

	constexpr fix32() = default;
	constexpr fix32(const fix32&) = default;
	constexpr fix32(int32_t number) : value(number << fractional_bits){}
	
	constexpr fix32(int32_t number, ReinterpretToken t) : value(number){}
	
	template<typename Integer, std::enable_if_t<std::is_integral<Integer>::value, bool> = true>
	fix32(Integer number) : fix32(static_cast<int32_t>(number)){}
	
	template<typename Integer, std::enable_if_t<std::is_integral<Integer>::value, bool> = true>
	fix32(Integer number, ReinterpretToken t) : fix32(static_cast<int32_t>(number), t){}

	inline fix32(float num) : value(0) {
		if (num != 0.f) {
			const uint32_t inum = *reinterpret_cast<const uint32_t*>(&num);
			const uint32_t float_mantissa = (inum & ((1 << 23) - 1)) | (1 << 23);
			const uint32_t float_exponent = ((inum & (((1 << 8) - 1) << 23)) >> 23) - 127;
			const bool float_sign = (inum & (1 << 31)) != 0;
			const int shifts = (static_cast<int>(fractional_bits) + static_cast<int>(float_exponent)) - static_cast<int>(23);
			const int32_t abs_value = (shifts >= 0) ? float_mantissa << shifts : float_mantissa >> -shifts;
			this->value = (float_sign) ? -abs_value : abs_value;
		}
		else {
			this->value = 0;
		}
	}
	
	inline fix32(double num) : fix32(static_cast<float>(num)){/*TODO: propper conversion*/}
	
	constexpr fix32(const char* str, int radix=10) : value(0){
		bool sign = false;
		uint32_t digits = 0;
		uint32_t fractions = 0;
		char const * const str_first = str;
		
		// parse sign
		if (*str == '-') {
			sign = true;
			++str;
		}

		// select radix
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
			}else if (ALPHA_first <= *str && *str <= ALPHA_first) {
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
					fractions = fractions + ((static_cast<uint32_t>(*str) - static_cast<uint32_t>(digit_first)) << 28) / s;
				}else if (alpha_first <= *str && *str <= alpha_last) {
					fractions = fractions + ((static_cast<uint32_t>(*str) - static_cast<uint32_t>(alpha_first) + 10) << 28) / s;
				}else if (ALPHA_first <= *str && *str <= ALPHA_first) {
					fractions = fractions + ((static_cast<uint32_t>(*str) - static_cast<uint32_t>(ALPHA_first) + 10) << 28) / s;
				}else {
					break;
				}
				++str;
				size_t new_s = s * radix;
				s = (new_s > s) ? new_s : 0; //overflow protection
			}

			// shift fractions to the correct binary point
			int shifts = static_cast<int>(28) - static_cast<int>(fractional_bits);
			fractions = (shifts >= 0) ? fractions >> shifts : fractions << -shifts;
		}

		const uint32_t abs_value = digits | fractions;
		this->value = sign ? (-static_cast<int32_t>(abs_value)) : static_cast<int32_t>(abs_value);
	}

	fix32& assign(const char* str, int radix=10) {
		return *this = fix32<fractional_bits>(str, radix);
	}


	template<size_t other_frac_bits>
	constexpr fix32(const fix32<other_frac_bits>& other){
		if (fractional_bits >= other_frac_bits)
			this->value = (other.reinterpret_as_int32_t() << (static_cast<uint32_t>(fractional_bits - other_frac_bits)));
		else
			this->value = (other.reinterpret_as_int32_t() >> (static_cast<uint32_t>(other_frac_bits - fractional_bits)));
	}
	
	inline fix32& operator= (const fix32&) = default;
	
	// Arithmetic operators
	
	constexpr friend fix32 operator+ (fix32 lhs, fix32 rhs){
		fix32 result;
		result.value = lhs.value + rhs.value;
		return result;
	}
	
	constexpr friend fix32 operator- (fix32 a){
		fix32 result;
		result.value = -a.value;
		return result;
	}
	
	constexpr friend fix32 operator- (fix32 lhs, fix32 rhs){
		fix32 result;
		result.value = lhs.value - rhs.value;
		return result;
	}
	
	constexpr friend fix32 operator* (fix32 lhs, fix32 rhs){
		int64_t temp = static_cast<int64_t>(lhs.value) * static_cast<int64_t>(rhs.value);
		return fix32::reinterpret(static_cast<int32_t>(static_cast<uint32_t>(temp >> fractional_bits)));
	}
	
	template<typename Integer, std::enable_if_t<std::is_integral<Integer>::value, bool> = true>
	constexpr friend fix32 operator* (fix32 lhs, Integer rhs){
		return fix32::reinterpret(lhs.value * static_cast<int32_t>(rhs));
	}

	template<typename Integer, std::enable_if_t<std::is_integral<Integer>::value, bool> = true>
	constexpr friend fix32 operator* (Integer lhs, fix32 rhs){
		return fix32::reinterpret(lhs * rhs.value);
	}

	constexpr friend fix32 operator/ (fix32 lhs, fix32 rhs){
		int64_t temp = (static_cast<int64_t>(lhs.value) << fractional_bits) / static_cast<int64_t>(rhs.value);
		return fix32::reinterpret(static_cast<int32_t>(temp));
	}
	
	template<typename Integer, std::enable_if_t<std::is_integral<Integer>::value, bool> = true>
	constexpr friend fix32 operator/ (fix32 lhs, Integer rhs){
		return fix32::reinterpret(lhs.value / static_cast<int32_t>(rhs));
	}
	
	constexpr friend fix32 operator% (fix32 lhs, fix32 rhs){
		return fix32::reinterpret(lhs.value() % rhs.value);
	}

	inline fix32& operator+= (fix32 rhs){return *this = *this + rhs;}
	inline fix32& operator-= (fix32 rhs){return *this = *this - rhs;}
	inline fix32& operator*= (fix32 rhs){return *this = *this * rhs;}
	inline fix32& operator/= (fix32 rhs){return *this = *this / rhs;}
	
	template<typename Integer, std::enable_if_t<std::is_integral<Integer>::value, bool> = true>
	inline fix32& operator*= (Integer rhs){return this->value = this->value * static_cast<int32_t>(rhs);}

	template<typename Integer, std::enable_if_t<std::is_integral<Integer>::value, bool> = true>
	inline fix32& operator/= (Integer rhs){return this->value = this->value / static_cast<int32_t>(rhs);}

	// Comparison operators
	constexpr friend bool operator== (fix32 lhs, fix32 rhs){return lhs.value == rhs.value;}
	constexpr friend bool operator!= (fix32 lhs, fix32 rhs){return lhs.value != rhs.value;}
	constexpr friend bool operator< (fix32 lhs, fix32 rhs){return lhs.value < rhs.value;}
	constexpr friend bool operator> (fix32 lhs, fix32 rhs){return lhs.value > rhs.value;}
	constexpr friend bool operator<= (fix32 lhs, fix32 rhs){return lhs.value <= rhs.value;}
	constexpr friend bool operator>= (fix32 lhs, fix32 rhs){return lhs.value >= rhs.value;}
	
	static constexpr fix32 reinterpret(int32_t number){return fix32(number, ReinterpretToken());}
	
	template<size_t other_frac_bits>
	explicit constexpr operator fix32<other_frac_bits> () {return fix32<other_frac_bits>(*this);}
	
	explicit constexpr operator int32_t (){return this->value >> fractional_bits;}
	explicit constexpr operator uint32_t (){return static_cast<uint32_t>(this->value >> fractional_bits);}
	
	explicit constexpr operator float (){
		float result = static_cast<float>(this->value) / (1 << fractional_bits);
		return result;
	}
	
	explicit constexpr operator double (){
		double result = static_cast<double>(this->value) / (1 << fractional_bits);
		return result;
	}
	
	constexpr int32_t static_cast_to_int32_t() const {return this->value >> fractional_bits;}
	constexpr int32_t reinterpret_as_int32_t() const {return this->value;}
	
	constexpr friend int32_t static_cast_to_int32_t(fix32 f){return f.value >> fractional_bits;}
	constexpr friend int32_t reinterpret_as_int32_t(fix32 f){return f.value;}
	
	template<class Stream>
	friend Stream& print(Stream& stream, fix32 f, size_t significant_places_after_comma=3) {
		if(f < 0){
			stream << '-';
			f = -f;
		}
		uint32_t digits = f.value >> fractional_bits;
		uint32_t fractionals = f.value & ((1 << fractional_bits) - 1);

		int significant_places = 0;
		bool count_significant_enable = digits != 0;

		// print digits and decimal point
		stream << digits << '.';

		// print fractionals
		while(fractionals != 0 && significant_places < significant_places_after_comma){
			significant_places += count_significant_enable;
			fractionals = fractionals * 10;
			uint32_t n = fractionals >> fractional_bits;
			count_significant_enable |= n != 0;
			fractionals = fractionals & ((1 << fractional_bits) - 1);
			char c = '0' + static_cast<char>(n);
			stream << c;
		}
		return stream;
	}

	template<class Stream>
	friend Stream& operator<<(Stream& stream, fix32 f){return print(stream, f);}

	template<class Stream>
	friend Stream& operator>>(Stream& stream, fix32& f) {
		bool sign = false;
		uint32_t digits = 0;
		uint32_t fractions = 0;
		int radix = 10;
		
		// parse sign
		if (stream.peek() == '-') {
			sign = true;
			stream.get();
		}

		// select radix
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
			}else if (ALPHA_first <= stream.peek() && stream.peek() <= ALPHA_first) {
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
					fractions = fractions + ((static_cast<uint32_t>(stream.peek()) - static_cast<uint32_t>(digit_first)) << 28) / s;
				}else if (alpha_first <= stream.peek() && stream.peek() <= alpha_last) {
					fractions = fractions + ((static_cast<uint32_t>(stream.peek()) - static_cast<uint32_t>(alpha_first) + 10) << 28) / s;
				}else if (ALPHA_first <= stream.peek() && stream.peek() <= ALPHA_first) {
					fractions = fractions + ((static_cast<uint32_t>(stream.peek()) - static_cast<uint32_t>(ALPHA_first) + 10) << 28) / s;
				}else {
					break;
				}
				stream.get();
				size_t new_s = s * radix;
				s = (new_s > s) ? new_s : 0; //overflow protection
			}

			// shift fractions to the correct binary point
			int shifts = static_cast<int>(28) - static_cast<int>(fractional_bits);
			fractions = (shifts >= 0) ? fractions >> shifts : fractions << -shifts;
		}

		const uint32_t abs_value = digits | fractions;
		const int32_t value = sign ? (-static_cast<int32_t>(abs_value)) : static_cast<int32_t>(abs_value);

		f = fix32::reinterpret(value);
		return stream;
	}

};


