Fix32
=====

A fixpoint class with 32 bits and variable fractional bits as a template parameter.
* Supports C++14 and C++17

# Use
It is a header only library. Just include the file 'fix32.hpp'

----

```C++
template<size_t fractional_bits> class fix32
```

## Constructors

```C++
constexpr fix32(); // 1
constexpr fix32(const fix32&); // 2
constexpr fix32(int32_t number); // 3

template<size_t other_frac_bits>
constexpr fix32(const fix32<other_frac_bits>& other); // 4
```

1) default constructs without initialisation
2) copy constructor
3) constructs from intager value with shift conversion like `static constexpr from(int32_t number)`
4) construct from a fixpoint type with different fractional number of bits

## Arithmetic Operators

```C++
constexpr friend fix32 operator+ (fix32 lhs, fix32 rhs);
constexpr friend fix32 operator- (fix32 lhs, fix32 rhs);
constexpr friend fix32 operator* (fix32 lhs, fix32 rhs);
constexpr friend fix32 operator/ (fix32 lhs, fix32 rhs);
```

```C++
constexpr fix32& operator+= (fix32 rhs);
constexpr fix32& operator-= (fix32 rhs);
constexpr fix32& operator*= (fix32 rhs);
constexpr fix32& operator/= (fix32 rhs);
```

## Comparison operators

```C++
constexpr friend bool operator== (fix32 lhs, fix32 rhs);
constexpr friend bool operator< (fix32 lhs, fix32 rhs);
constexpr friend bool operator> (fix32 lhs, fix32 rhs);
constexpr friend bool operator<= (fix32 lhs, fix32 rhs);
constexpr friend bool operator>= (fix32 lhs, fix32 rhs);
```

## Conversions

```C++
static constexpr from(int32_t number);
```
Constructs a fixpoint and converts the integer value to the fixpoint value by shifting it accordingly.
(static casting)

----
	
```C++
static constexpr reinterpret(int32_t number);
```
Constructs a fixpoint and reinterprets the integer as an fixpoint 'as is' without any operations.
(reinterpret casting)

---- 

```C++ 
constexpr operator int32_t ();
constexpr friend int32_t static_cast_to_int32_t(fix32 f);
```
static cast. Converts the fixpoint number to an integer by removeing all information stored in the fractional bits.

```C++
constexpr operator float ();
constexpr operator double ();
```
static cast. Converts the fixpoint to a floating point representation

----

```C++
constexpr friend int32_t reinterpret_as_int32_t(fix32 f);
```
reinterpret cast. Converts the fixpoint number to an integer by keeping all bits 'as is'.

----

```C++
template<size_t other_frac_bits>
explicit constexpr operator fix32<other_frac_bits> ()
```
static cast to a fixpoint representation with different number of fractional bits.
