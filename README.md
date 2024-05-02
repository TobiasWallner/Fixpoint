Fix32
=====

A fixpoint class with 32 bits and variable fractional bits as a template parameter.
* Supports C++14

``` 
template<size_t fractional_bits> class fix32
```

## Static Member Functions

```
static constexpr from(int32_t number)
```
Constructs a fixpoint and converts the integer value to the fixpoint value by shifting it accordingly.
(static casting)

----
	
```
static constexpr reinterpret_from(int32_t number)
```
Constructs a fixpoint and reinterprets the integer as an fixpoint 'as is' without any operations.
(reinterpret casting)

---- 
