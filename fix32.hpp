#pragma once

#include <cinttypes>

template<size_t fractional_bits>
class fix32{
private:
	int32_t value;
	
public:
	constexpr fix32() = default;
	constexpr fix32(const fix32&) = default;
	
	static constexpr fix32 from(int32_t number){
		fix32 result;
		result.value = number << fractional_bits;
		return result;
	}
	
	static constexpr fix32 reinterpret_from(int32_t number){
		fix32 result;
		result.value = number;
		return result;
	}
	
	constexpr fix32& operator= (const fix32&) = default;
	
	
};