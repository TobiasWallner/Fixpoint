#pragma once
/*
	To disable boundary checks:
		Define: DISABLE_FIXPOINT_ASSERTIONS
		
	To handle errors, define one of the following
		FIXPOINT_THROW_ERROR			(default)
		FIXPOINT_EXIT_ERROR 			(calls exit(-1) to exit the program)
		FIXPOINT_TRAP_ERROR 			(traps the execution in a while loop)
		FIXPOINT_LOG_ERROR				(logs the error to some output stream and continues execution)
		FIXPOINT_CUSTOM_ERROR		 	(calls the user defined function: 'void fixpoint_custom_error_handler(const char* error_message)')
		
	Set the error output stream by defining (only for FIXPOINT_EXIT, FIXPOINT_TRAP or FIXPOINT_RETURN_ZERO):
		FIXPOINT_CERR	(default is std::cerr)
*/

#define FIXPOINT_ENABLE_IF(condition) typename std::enable_if_t<(condition), int> = 0

#ifdef DISABLE_FIXPOINT_ASSERTIONS
	#define fixpoint_assert(condition, message)
#else
	
	#ifndef FIXPOINT_CERR
		#define FIXPOINT_CERR std::cout
	#endif

	#if defined(FIXPOINT_CUSTOM_ERROR)
		void fixpoint_custom_error_handler(const char* error_message);
		#define fixpoint_assert(condition, message) if(!(condition)){std::stringstream s; s << message; fixpoint_custom_error_handler(s.str().c_str());}
	#elif defined(FIXPOINT_TRAP_ERROR)
		#define fixpoint_assert(condition, message) if(!(condition)){FIXPOINT_CERR << message; while(true){};}
	#elif defined(FIXPOINT_EXIT_ERROR)
		#define fixpoint_assert(condition, message) if(!(condition)){FIXPOINT_CERR << message; exit(-1);}
	#elif defined(FIXPOINT_LOG_ERROR)
		#define fixpoint_assert(condition, message) if(!(condition)){FIXPOINT_CERR << message;}
	#else
		#include <exception>
		#define fixpoint_assert(condition, message) if(!(condition)){std::stringstream s; s << message; throw std::runtime_error(s.str().c_str());}
	#endif

#endif