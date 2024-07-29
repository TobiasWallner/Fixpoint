cmake_minimum_required(VERSION 3.18.0)



# Basic compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)

# project building process: 
project(test_fix32)
add_executable(test_fix32
	test/test_fix32.cpp
	fix32.hpp
)

project(test_fix64)
add_executable(test_fix64
	test/test_fix64.cpp
	fix64.hpp
)

project(test_fixmath)
add_executable(test_fixmath
	test/test_fixmath.cpp
	fix32.hpp
	fix64.hpp
	fixmath.hpp
)

include_directories(
	.
)

# Compiler Options for Clang:
# ===========================
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	set(COMPILER_FLAGS 
		
		# set compiler flags for debugging
		#---------------------------------
		$<$<CONFIG:Debug>:
			-O0	# disable optimisations
			-g	# enable debug information
		>
		
		# set compiler flags for release
		#-------------------------------
		$<$<CONFIG:Release>:
			-O3 # Enable all Optimisations
		>
	)
# Compiler Options for GNU:
# =========================
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	set(COMPILER_FLAGS
		
		# Enable link time optimisations
		-flto
		
		# set compiler flags for debugging
		#---------------------------------
		$<$<CONFIG:Debug>:
			-O0	# disable optimisations
			-g	# enable debug information
			-ggdb # extra flag for the debugger
		>
		
		# set compiler flags for release
		#-------------------------------
		$<$<CONFIG:Release>:
			-O3 # Enable all Optimisations
		>
	)
# Compiler Options for MSVC:
# ==========================
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	set(COMPILER_FLAGS
		/W3
		/Zc:__cplusplus # enable __cplusplus
		# set compiler flags for debugging
		#---------------------------------
		$<$<CONFIG:Debug>:
		
			# disable optimisations for debugging
			/Od 
			
			# produces a PDB file in a format that supports the Edit and Continue feature. 
			# To use Edit and Continue debugging features, you must use this option. 
			/ZI 
		>
		
		# set compiler flags for release versions
		#----------------------------------------
		$<$<CONFIG:Release>:
			
			# Creates fast code
			/O2 
		>
	)
else()
	message( SEND_ERROR "The compiler you are using is not handeled. Add your compiler and compiler flags to the if(CMAKE_CXX_COMPILER_ID STREQUAL <your compiler>) in the CMakeLists.txt of the root folder")
endif()


target_compile_options(test_fix32 PUBLIC
	${COMPILER_FLAGS}
)
target_compile_options(test_fix64 PUBLIC
	${COMPILER_FLAGS}
)
target_compile_options(test_fixmath PUBLIC
	${COMPILER_FLAGS}
)

target_link_libraries(test_fix32 PUBLIC

)
target_link_libraries(test_fix64 PUBLIC

)
target_link_libraries(test_fixmath PUBLIC

)