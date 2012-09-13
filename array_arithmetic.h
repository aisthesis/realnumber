/**
 * array_arithmetic.h
 * Namespace for methods needed for Karatsuba multiplication
 * http://stackoverflow.com/questions/9321/how-do-you-create-a-static-class-in-c
 * cf. Stroustrup, p. 200 bottom: Don't declare functions as static here!
 * (in constrast to the stackoverflow thread
 *
 * Created by Marshall Farrier on 6/27/2012.
 * Copyright (c) 2012 Marshall Farrier
 * license http://opensource.org/licenses/gpl-license.php GNU Public License
 */

#ifndef ARRAY_ARITHMETIC_H
#define ARRAY_ARITHMETIC_H

#include <string>
#include <cstddef>

namespace array_arithmetic {
	/**
	 * length of larger array where we revert to school multiplication
	 */
	const unsigned int RADIX = 256;
	const unsigned int RADIX_MINUS_ONE = RADIX - 1;
	const std::size_t KARATSUBA_THRESHOLD = 4;
	/**
	 * conversion functions
	 */
	namespace convert {
		void from_int(const unsigned int& num, unsigned char* result, 
				const std::size_t& result_len);
		unsigned int to_int(const unsigned char* arr, const std::size_t& arr_len);
		namespace helpers {
			/**
			 * Converts a string of decimal digits into an array
			 * Throws an exception if a char is found that isn't a digit
			 */
			unsigned char* to_digit_array(const std::string& str);
			/**
			 * multiplies the decimal digit array in place and returns the
			 * number now to the left of the decimal
			 */
			unsigned char times_radix(unsigned char* decimal_digits, 
					const std::size_t& arr_len);
			/**
			 * multiplies in place the elements in an array representing RADIX fractional
			 * digits by 10 to get the next digit in the decimal representation
			 */
			unsigned char times_ten(unsigned char* radix_digits, const std::size_t& arr_len);
		}
	}
	namespace helpers {
		/**
		 * Copy source array into target array, which must be at least as big as
		 * source array, i.e.: target_len >= source_len
		 * the least significant (rightmost) digits in target are all set to 0
		 * target doesn't need to have been initialized
		 */
		void copy_into(const unsigned char* source, unsigned char* target, const std::size_t&
				source_len, const std::size_t& target_len);
		/**
		 * Shift array to the right the given number of places. Substitutes 0 for the empty
		 * places and drops the last places of the array
		 */
		void shift_right(unsigned char* arr, const size_t& places, const size_t& arr_size);
		/**
		 * Assumption:
		 * arr1 and arr2 have the same length
		 */
		bool less_or_equal(const unsigned char* arr1, const unsigned char* arr2, 
																 const std::size_t& len);
		/**
		 * test array to see if it represents 0--i.e., all indices contain the value 0
		 */
		bool is_zero(const unsigned char* arr, const std::size_t& len);
		/**
		 * sets all values to 0
		 */
		void set_to_zero(unsigned char* arr, const std::size_t& len);

		namespace division {
			/**
			 * Used together with align_divisor() in align()
			 * Assumptions:
			 * 1) both arrays are the same size
			 * 2) divisor is non-zero. Otherwise the loop is infinite and, to improve performance,
			 * we do not make this check
			 * Summary:
			 * shift dividend bitwise right until dividend < divisor (dividend < divisor is guaranteed after this
			 * operation)
			 * returns a negative integer representing the number of places moved
			 */
			int align_dividend(unsigned char* dividend, const unsigned char* divisor, const size_t& len);
			/**
			 * shift divisor bitwise right until it is <= dividend. returns the number of places moved
			 */
			int align_divisor(const unsigned char* dividend, unsigned char* divisor, const std::size_t& len);
			/**
			 * align dividend then align divisor and return net result
			 */
			int align(unsigned char* dividend, unsigned char* divisor, const std::size_t& len);
			/**
			 * readjust divisor as division continues
			 * shifts divisor right until divisor <= dividend
			 */
			int readjust(const unsigned char* dividend, unsigned char* divisor, const std::size_t& len);
		}
	}
	/**
	 * for accessing and manipulating bits
	 */
	namespace bits {
		/**
		 * returns a mask in which the given number of least significant bits are set
		 * If bits_to_show >= 8, all bits will be set (255)
		 */
		unsigned char get_mask(const std::size_t& bits_to_show);
		/**
		 * shift the array bits to the right
		 */
		void shift_right(unsigned char* arr, const std::size_t& places, const std::size_t& arr_size);
		/**
		 * It is assumed that bit_to_set < arr_len * 8, where arr_len is the 
		 * size of the input array (not known to the function)
		 */
		void set(unsigned char* arr, const size_t& bit_to_set);
	}
	
	/**
	 * Assumptions:
	 * 1) result has length a_len + b_len + 1
	 * 2) b_len <= a_len
	 */
	void karatsuba_multiply(const unsigned char* a, const unsigned char* b,
			unsigned char* result, const std::size_t& a_len, const std::size_t& b_len);

	/**
	 * Assumptions:
	 * 1) result must have length a_len + b_len + 1
	 * 2) b_len <= a_len
	 */
	void school_multiply(const unsigned char* a, const unsigned char* b,
			unsigned char* result, const std::size_t& a_len, const std::size_t& b_len); 

	/**
	 * This version uses NO OVERFLOW for the result!!!
	 * All arrays are assumed to have the same length
	 */
	void add(const unsigned char* a, const unsigned char* b, unsigned char* result,
			const std::size_t& len);
	/**
	 * Assumptions:
	 * 1) result has length a_len + 1
	 * 2) b_len <= a_len
	 */
	void add(const unsigned char* a, const unsigned char* b,
			unsigned char* result, const std::size_t& a_len, const std::size_t& b_len); 
	/**
	 * Assumptions:
	 * 1) all arrays have the same size
	 * 2) the number represented by b <= number represented by a
	 */
	void subtract(const unsigned char* a, const unsigned char* b, unsigned char* result,
			const std::size_t& len);
	/**
	 * Assumptions:
	 * 1) result is assumed to have length a_len
	 * 2) b_len <= a_len
	 * 3) the number represented by b <= number represented by a
	 */
	void subtract(const unsigned char* a, const unsigned char* b, unsigned char* result,
			const std::size_t& a_len, const std::size_t& b_len);
	/**
	 * Assumptions:
	 * 1) dividend, divisor and result all have the same length
	 * 2) It is NOT assumed that result has been initialized to 0
	 * @param int_digits We need to know how many digits represent integer places
	 */
	void divide(const unsigned char* dividend, const unsigned char* divisor, unsigned char* result,
			const std::size_t& len, const std::size_t& int_digits);
}
#endif
