#pragma once

#include "fix32.hpp"
#include "fix64.hpp"

// ================ Basic operations ================

// ---------------- abs ----------------

template<size_t N> constexpr fix32<N> abs(fix32<N> a){return (a >= 0) ? a : -a;}
template<size_t N> constexpr fix64<N> abs(fix64<N> a){return (a >= 0) ? a : -a;}

// ---------------- mod ----------------
// note: this is the arithmetic modulo: the result is always in the range [0, r)
// returns the equivalent of: l - round_down(a / b) * b

template<size_t N> constexpr fix32<N> mod(fix32<N> l, fix32<N> r){
	return (l < 0 != r < 0) ? ((l % r) + r) : (l % r);
}
template<size_t N> constexpr fix64<N> mod(fix64<N> l, fix64<N> r){
	return (l < 0 != r < 0) ? ((l % r) + r) : (l % r);
}

// ---------------- remainder ----------------

template<size_t N> constexpr fix32<N> remainder(fix32<N> l, fix32<N> r){return l % r;}
template<size_t N> constexpr fix64<N> remainder(fix64<N> l, fix64<N> r){return l % r;}

// ---------------- min ----------------

template<size_t N> constexpr fix32<N> min(fix32<N> l, fix32<N> r){return (l < r) ? l : r;}
template<size_t N> constexpr fix64<N> min(fix64<N> l, fix64<N> r){return (l < r) ? l : r;}

// ---------------- max ----------------

template<size_t N> constexpr fix32<N> max(fix32<N> l, fix32<N> r){return (l > r) ? l : r;}
template<size_t N> constexpr fix64<N> max(fix64<N> l, fix64<N> r){return (l > r) ? l : r;}


// ================ Interpolation ================

// ---------------- linear_interpolation / lerp ----------------

template<size_t N> constexpr fix32<N> lerp(fix32<N> a, fix32<N> b, fix32<N> t){return a + (b - a) * t;}
template<size_t N> constexpr fix64<N> lerp(fix64<N> a, fix64<N> b, fix64<N> t){return a + (b - a) * t;}

template<size_t N> constexpr fix32<N> lerp(fix32<N> x0, fix32<N> x1, fix32<N> y0, fix32<N> y1, fix32<N> x){
	return (x - x0) * (y1 - y0) / (x0 - x1) + y0;
}
template<size_t N> constexpr fix64<N> lerp(fix64<N> x0, fix64<N> x1, fix64<N> y0, fix64<N> y1, fix64<N> x){
	return (x - x0) * (y1 - y0) / (x0 - x1) + y0;
}

namespace fixmath_detail{
	template<class Iterator, class T>
	Iterator my_lower_bound(Iterator first, Iterator last, const T& value){
		Iterator it = first;
		auto count = std::distance(first, last);
	 
		while (count > 0){
			it = first;
			auto step = count / 2;
			std::advance(it, step);
	 
			if (*it < value){
				first = ++it;
				count -= step + 1;
			}else{
				count = step;
			}
		}
	 
		return first;
	}	
}

template<size_t N, class Iterator>
constexpr fix32<N> lerp(Iterator x_first, Iterator x_last, Iterator y_first, Iterator y_last, fix32<N> x){
	if(x <= *x_first){
		return lerp(*x_first, *(x_first+1), *y_first, *(y_first+1), x);
	}else if(*(x_last-1) <= x){
		return lerp(*(x_last-2), *(x_last-1), *(y_last-2), *(y_last-1), x);
	}else{
		
		Iterator x_itr = x_first;
		Iterator y_itr = y_first;
		
		{// binary search
			auto count = std::distance(x_first, x_last);
		 
			while (count > 0){
				x_itr = x_first;
				auto step = count / 2;
				std::advance(x_itr, step);
				std::advance(y_itr, step);
		 
				if (*x_itr < x){
					x_first = ++x_itr;
					y_first = ++y_itr;
					count -= step + 1;
				}else{
					count = step;
				}
			}
		}
		
		fix32<N> result = lerp(*x_first, *(x_first+1), *y_first, *(y_first+1), x);
		return result;
	}
}

template<size_t N, class Iterator>
constexpr fix64<N> lerp(Iterator x_first, Iterator x_last, Iterator y_first, Iterator y_last, fix64<N> x){
	if(x <= *x_first){
		return lerp(*x_first, *(x_first+1), *y_first, *(y_first+1), x);
	}else if(*(x_last-1) <= x){
		return lerp(*(x_last-2), *(x_last-1), *(y_last-2), *(y_last-1), x);
	}else{
		
		Iterator x_itr = x_first;
		Iterator y_itr = y_first;
		
		{// binary search
			auto count = std::distance(x_first, x_last);
		 
			while (count > 0){
				x_itr = x_first;
				auto step = count / 2;
				std::advance(x_itr, step);
				std::advance(y_itr, step);
		 
				if (*x_itr < x){
					x_first = ++x_itr;
					y_first = ++y_itr;
					count -= step + 1;
				}else{
					count = step;
				}
			}
		}
		
		fix64<N> result = lerp(*x_first, *(x_first+1), *y_first, *(y_first+1), x);
		return result;
	}
}

// ================ Exponential Functions  ================

// ---------------- exp, exp2, exp10, expm1 ----------------

template<size_t N> constexpr fix32<N> exp2(fix32<N> a){
	// TODO: increase precision by calculating the fractions with fix<30>
	const int32_t value = a.reinterpret_as_int32();
		
	if (value >= 0){
		const uint32_t digits = value & ((1<<N)-1);
		const uint32_t fractions = value & ((1<<N)-1);
		
		// perform the calculation 2^fix = 2^(digits + fractions/2^N) = 2^digits * 2^(fractions/2^N)	
		const uint32_t exp2_digits = (1 << (N + digits));
		const fix32<N> fix_exp2_digits = fix32<N>::reinterpret(exp2_digits);
		
		// f(x) = 2^(x/2^N) with x in [0, 2^N] --> f'(t) = 2^t with t in [0, 1]
		// approximate f'(t) = 2^t with a quadratic through the points t={0, 1/2, 1}
		const fix32<N> t = fix32<N>::reinterpret(fractions);
		constexpr fix32<N> a = (6 - 4 * sqrt(fix32<N>(2))); // todo: calculate bitpattern for fix<30>
		constexpr fix32<N> b = (4 * sqrt(fix32<N>(4)) - 5); // todo: calculate bitpattern for fix<30>
		constexpr fix32<N> c = 1; // todo: calculate bitpattern for fix<30>
		const fix32<N> fix_exp2_fractions = a * t * t + b * t + c;
		
		// result = 2^digits * 2^(fractions/2^N)	
		const fix32<N> result = fix_exp2_digits * fix_exp2_fractions;
		return result;
	}else{
		const uint32_t digits = (-value) & ((1<<N)-1);
		const uint32_t fractions = (-value) & ((1<<N)-1);
		
		// perform the calculation 2^fix = 2^(digits + fractions/2^N) = 2^digits * 2^(fractions/2^N)
		const uint32_t exp2_digits = (1 << (N - digits));
		const fix32<N> fix_exp2_digits = fix32<N>::reinterpret(exp2_digits);
		
		// --> f'(t) = 2^-t with t in [0, 1]
		// approximate f'(t) = 2^-t with a quadratic through the points t={0, 1/2, 1}
		const fix32<N> t = fix32<N>::reinterpret(fractions);
		constexpr fix32<N> a = fix32<N>(3) - 2 * sqrt(fix32<N>(2)); // todo: calculate bitpattern for fix<30>
		constexpr fix32<N> b = 2 * sqrt(fix32<N>(2))-(fix32<N>(7)/fix32<N>(2)); // todo: calculate bitpattern for fix<30>
		constexpr fix32<N> c = 1; // todo: calculate bitpattern for fix<30>
		
		const fix32<N> fix_exp2_fractions = a * t * t + b * t + c;
		
		// result = 2^digits * 2^(fractions/2^N)	
		const fix32<N> result = fix_exp2_digits * fix_exp2_fractions;
		return result;
	}
}
template<size_t N> constexpr fix64<N> exp2(fix64<N> a){
	// TODO: increase precision by calculating the fractions with fix<30>
	const int32_t value = a.reinterpret_as_int64();
		
	if (value >= 0){
		const uint64_t digits = value & ((1<<N)-1);
		const uint64_t fractions = value & ((1<<N)-1);
		
		// perform the calculation 2^fix = 2^(digits + fractions/2^N) = 2^digits * 2^(fractions/2^N)	
		const uint64_t exp2_digits = (1 << (N + digits));
		const fix64<N> fix_exp2_digits = fix64<N>::reinterpret(exp2_digits);
		
		// f(x) = 2^(x/2^N) with x in [0, 2^N] --> f'(t) = 2^t with t in [0, 1]
		// approximate f'(t) = 2^t with a quadratic through the points t={0, 1/2, 1}
		const fix64<N> t = fix64<N>::reinterpret(fractions);
		constexpr fix64<N> a = (6 - 4 * sqrt(fix64<N>(2))); // todo: calculate bitpattern for fix<30>
		constexpr fix64<N> b = (4 * sqrt(fix64<N>(4)) - 5); // todo: calculate bitpattern for fix<30>
		constexpr fix64<N> c = 1; // todo: calculate bitpattern for fix<30>
		const fix64<N> fix_exp2_fractions = a * t * t + b * t + c;
		
		// result = 2^digits * 2^(fractions/2^N)	
		const fix64<N> result = fix_exp2_digits * fix_exp2_fractions;
		return result;
	}else{
		const uint64_t digits = (-value) & ((1<<N)-1);
		const uint64_t fractions = (-value) & ((1<<N)-1);
		
		// perform the calculation 2^fix = 2^(digits + fractions/2^N) = 2^digits * 2^(fractions/2^N)
		const uint64_t exp2_digits = (1 << (N - digits));
		const fix64<N> fix_exp2_digits = fix64<N>::reinterpret(exp2_digits);
		
		// --> f'(t) = 2^-t with t in [0, 1]
		// approximate f'(t) = 2^-t with a quadratic through the points t={0, 1/2, 1}
		const fix64<N> t = fix64<N>::reinterpret(fractions);
		constexpr fix64<N> a = fix64<N>(3) - 2 * sqrt(fix64<N>(2)); // todo: calculate bitpattern for fix<30>
		constexpr fix64<N> b = 2 * sqrt(fix64<N>(2))-(fix64<N>(7)/fix64<N>(2)); // todo: calculate bitpattern for fix<30>
		constexpr fix64<N> c = 1; // todo: calculate bitpattern for fix<30>
		
		const fix64<N> fix_exp2_fractions = a * t * t + b * t + c;
		
		// result = 2^digits * 2^(fractions/2^N)	
		const fix64<N> result = fix_exp2_digits * fix_exp2_fractions;
		return result;
	}
}

template<size_t N> constexpr fix32<N> exp(fix64<N> a){
	constexpr fix32<N> lambda("2.44269504889"); // log2(e);
	return exp2(lambda * a);
}
template<size_t N> constexpr fix64<N> exp(fix64<N> a){
	constexpr fix64<N> lambda("2.44269504889"); // log2(e);
	return exp2(lambda * a);
}

template<size_t N> constexpr fix32<N> exp10(fix64<N> a){
	constexpr fix32<N> lambda("3.3219280948874"); // log2(10);
	return exp2(lambda * a);
}
template<size_t N> constexpr fix64<N> exp10(fix64<N> a){
	constexpr fix64<N> lambda("3.3219280948874"); // log2(10);
	return exp2(lambda * a);
}

template<size_t N> constexpr fix32<N> expm1(fix32<N> a){return exp(a)-1;}
template<size_t N> constexpr fix64<N> expm1(fix64<N> a){return exp(a)-1;}

// ---------------- log, log2, log10, log1p ----------------

namespace fixpoint_detail{
	// returns log2(1+x) with x in [0, 1)
	// approximated with a polinom order 4 at x={0, 1/4, 1/2, 3/4, 1}
	constexpr fix32<30> log2_1p_0_1(fix32<30> x){
		constexpr fix32<30> a("-0.0784753434979");
		constexpr fix32<30> b("0.3123978638466");
		constexpr fix32<30> c("-0.6711149475331");
		constexpr fix32<30> d("1.4371924271845");
		
		const fix32<30> x2 = x * x;
		const fix32<30> x3 = x2 * x;
		const fix32<30> x4 = x2 * x2;
		
		const fix32<30> result = a * x4 + b * x3 + c * x2 + d * x;
		return result;
	}
	
	// returns log2(1+x) with x in [0, 1)
	// approximated with a polinom order 4 at x={0, 1/4, 1/2, 3/4, 1}
	constexpr fix64<62> log2_1p_0_1(fix64<62> x){
		constexpr fix64<62> a("-0.0784753434979");
		constexpr fix64<62> b("0.3123978638466");
		constexpr fix64<62> c("-0.6711149475331");
		constexpr fix64<62> d("1.4371924271845");
		
		const fix64<62> x2 = x * x;
		const fix64<62> x3 = x2 * x;
		const fix64<62> x4 = x2 * x2;
		
		const fix64<62> result = a * x4 + b * x3 + c * x2 + d * x;
		return result;
	}
}

template<size_t N> constexpr fix32<N> log2(fix32<N> a){
	// TODO: assertion for negative numbers
	
	// calculate: log2(v * 2^b) = log2(v) + b = log2(1+x) + b
	const uint32_t ai = static_cast<uint32_t>(a.reinterpret_as_int32());

	const int32_t bsr = fixpoint_detail::bit_scan_reverse(ai);
	const int32_t b = bsr - N;
	
	// TODO: check if the shifts are correct
	const fix32<30> v = fix32<30>::reinterpret((30-bsr >= 0) ? ai << (30-bsr) : ai >> (30-bsr));
	const fix32<30> x = v-1;
	
	const fix32<N> k = fixpoint_detail::log2_1p_0_1(x);
	const fix32<N> result = k + b;
	return result;
}

// ================ Rounding ================
// ---------------- round_down / floor ----------------

template<size_t N>
constexpr fix32<N> round_down(fix32<N> x){
	return fix32<N>::reinterpret(x.reinterpret_as_int32() & (~((static_cast<int32_t>(1) << N)-1)));
}

template<size_t N> constexpr fix32<N> floor(fix32<N> x){return round_down(x);}


template<size_t N>
constexpr fix64<N> round_down(fix64<N> x){
	return fix64<N>::reinterpret(x.reinterpret_as_int64() & (~((static_cast<int64_t>(1) << N)-1)));
}

template<size_t N> constexpr fix64<N> floor(fix64<N> x){return round_down(x);}

// ---------------- round_up / ceil ----------------

template<size_t N>
constexpr fix32<N> round_up(fix32<N> x){
	const fix32<N> rd = round_down(x);
	return (rd == x) ? rd : (rd + 1);
}

template<size_t N> constexpr fix32<N> ceil(fix32<N> x){return round_up(x);}

template<size_t N>
constexpr fix64<N> round_up(fix64<N> x){
	const fix64<N> rd = round_down(x);
	return (rd == x) ? rd : (rd + 1);
}

template<size_t N> constexpr fix64<N> ceil(fix64<N> x){return round_up(x);}