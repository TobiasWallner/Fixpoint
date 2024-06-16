FixPoint Library
===

## Overview

This library provides two fixed-point arithmetic types, fix32 and fix64, which are header-only and templated to allow users to specify the number of fractional bits. These types offer a balance between the precision of floating-point numbers and the performance and determinism of integer arithmetic.
Features

* **Fixed-Point Arithmetic**: Perform arithmetic operations with fixed precision.
* **Templated Fractional Bits**: Specify the number of fractional bits at compile-time for flexibility.
* **Support for Various Data Types**: Construct fixed-point numbers from integers and floating-point numbers.
* **Stream Output**: Output fixed-point numbers to streams.
* **Comparison and Arithmetic Operators**: Includes all standard comparison and arithmetic operators.

## Usage

To use the fix32 and fix64 types in your project, include the corresponding header files and specify the number of fractional bits. Below are examples of how to use these types.
Example for fix32

```cpp
fix32<8> a = 3.1415926535897932f;          // 24 integer bits, 8 fractional bits
fix32<8> b = 1.5f;
fix32<8> c = a + b;
std::cout << "Result: " << c << std::endl; // Output> Result: 4.640
```

Example for fix64

```cpp
fix64<42> a = 3.1415926535897932;          // 16 integer bits, 48 fractional bits 
fix64<42> b = 1.5;
fix64<42> c = a * b;
std::cout << "Result: " << c << std::endl; // Output> Result: 4.742 
std::cout << "Result: "; 
print(std::cout, c, 8) << std::endl;       // Output> Result: 4.74218380
```

## API

## fix32 Class

### Constructors
```CPP
fix32() = default;
fix32(const fix32&) = default;
fix32(int32_t number);
fix32(uint32_t number);
fix32(float num);
fix32(double num);
template<size_t other_frac_bits> fix32(const fix32<other_frac_bits>& other);
```
### Arithmetic Operators
```CPP
friend fix32 operator+ (fix32 lhs, fix32 rhs);
friend fix32 operator- (fix32 a);
friend fix32 operator- (fix32 lhs, fix32 rhs);
friend fix32 operator* (fix32 lhs, fix32 rhs);
template<typename Integer> friend fix32 operator* (fix32 lhs, Integer rhs);
template<typename Integer> friend fix32 operator* (Integer lhs, fix32 rhs);
friend fix32 operator/ (fix32 lhs, fix32 rhs);
template<typename Integer> friend fix32 operator/ (fix32 lhs, Integer rhs);
```
### Comparison Operators
```CPP
friend bool operator== (fix32 lhs, fix32 rhs);
friend bool operator!= (fix32 lhs, fix32 rhs);
friend bool operator< (fix32 lhs, fix32 rhs);
friend bool operator> (fix32 lhs, fix32 rhs);
friend bool operator<= (fix32 lhs, fix32 rhs);
friend bool operator>= (fix32 lhs, fix32 rhs);
```
### Stream/Print Output
```CPP
friend Stream& print(Stream& stream, fix32 f, size_t significant_places_after_comma=3);
template<class Stream> friend Stream& operator<<(Stream& stream, fix32 f);
```
##fix64 Class

### Constructors
```CPP
fix64() = default;
fix64(const fix64&) = default;
fix64(int64_t number);
fix64(uint64_t number);
fix64(float num);
fix64(double num);
template<size_t other_frac_bits> fix64(const fix64<other_frac_bits>& other);
```
### Arithmetic Operators
```CPP
friend fix64 operator+ (fix64 lhs, fix64 rhs);
friend fix64 operator- (fix64 a);
friend fix64 operator- (fix64 lhs, fix64 rhs);
friend fix64 operator* (fix64 lhs, fix64 rhs);
template<typename Integer> friend fix64 operator* (fix64 lhs, Integer rhs);
template<typename Integer> friend fix64 operator* (Integer lhs, fix64 rhs);
friend fix64 operator/ (fix64 lhs, fix64 rhs);
template<typename Integer> friend fix64 operator/ (fix64 lhs, Integer rhs);
```
### Comparison Operators
```CPP
friend bool operator== (fix64 lhs, fix64 rhs);
friend bool operator!= (fix64 lhs, fix64 rhs);
friend bool operator< (fix64 lhs, fix64 rhs);
friend bool operator> (fix64 lhs, fix64 rhs);
friend bool operator<= (fix64 lhs, fix64 rhs);
friend bool operator>= (fix64 lhs, fix64 rhs);
```
### Stream/Print Output
```CPP
friend Stream& print(Stream& stream, fix64 f, size_t significant_places_after_comma=3);
template<class Stream> friend Stream& operator<<(Stream& stream, fix64 f);
```
## Installation

This library is header-only, so you can simply include the header files in your project.

Copy fix32.hpp and fix64.hpp to your project's include directory.
Include the headers in your source files where needed.

## License

This project is licensed under the MIT License. See the LICENSE file for details.
Author
