/**
 * array_arithmetic.cpp
 * Namespace for methods needed for Karatsuba multiplication
 * http://stackoverflow.com/questions/9321/how-do-you-create-a-static-class-in-c
 * cf. Stroustrup, p. 200 bottom: Don't declare functions as static here!
 * (in constrast to the stackoverflow thread
 *
 * Created by Marshall Farrier on 6/27/2012.
 * Copyright (c) 2012 Marshall Farrier
 * license http://opensource.org/licenses/gpl-license.php GNU Public License
 */

#include <stdexcept>
#include <string>
#include <stdexcept>

#include "array_arithmetic.h"

namespace array_arithmetic {
	/**
	 * for "private" methods
	 */
	namespace {
	}
	/**
	 * conversion functions
	 */
	namespace convert {
		void from_int(const unsigned int& num, unsigned char* result, 
				const std::size_t& result_len) {
			unsigned int num_copy = num;
			std::size_t i = result_len;
			while (i > 0) {
				--i;
				result[i] = num_copy % RADIX;
				num_copy /= RADIX;
			}
		}
		unsigned int to_int(const unsigned char* arr, const std::size_t& arr_len) {
			unsigned int result = 0;
			for (std::size_t i = 0; i < arr_len; ++i) {
				result = result * RADIX + arr[i];
			}
			return result;
		}
		namespace helpers {
			unsigned char* to_digit_array(const std::string& str) {
				std::size_t len = str.size();
				unsigned char* result = new unsigned char[len];
				std::size_t i = 0;
				while (i < len) {
					if (0 <= str[i] - '0' && str[i] - '0' <= 9) {
						result[i] = str[i] - '0';
					}
					else {
						delete[] result;
						throw std::invalid_argument("Input can contain only decimal digits!");
					}
					++i;
				}
				return result;
			}
			unsigned char times_radix(unsigned char* decimal_digits, 
					const std::size_t& arr_len) {
				unsigned char carry = 0;
				unsigned int tmp;
				std::size_t i = arr_len;
				while (i > 0) {
					--i;
					tmp = carry + decimal_digits[i] * RADIX;
					decimal_digits[i] = tmp % 10;
					carry = tmp / 10;
				}
				return carry;
			}
			unsigned char times_ten(unsigned char* radix_digits, const std::size_t& arr_len) {
				unsigned char carry = 0;
				unsigned int tmp;
				std::size_t i = arr_len;
				while (i > 0) {
					--i;
					tmp = carry + radix_digits[i] * 10;
					radix_digits[i] = tmp % RADIX;
					carry = tmp / RADIX;
				}
				return carry;
			}
		}
	}
	namespace helpers {
		void copy_into(const unsigned char* source, unsigned char* target, const std::size_t&
				source_len, const std::size_t& target_len) {
			std::size_t i;
			for (i = 0; i < source_len; ++i) {
				target[i] = source[i];
			}
			for ( ; i < target_len; ++i) {
				target[i] = 0;
			}
		}
		void shift_right(unsigned char* arr, const size_t& places, const size_t& arr_size) {
			std::size_t i = arr_size - places;
			while (i > 0) {
				--i;
				arr[i + places] = arr[i];
			}
			i = places;
			while (i > 0) {
				--i;
				arr[i] = 0;
			}
		}
		bool less_or_equal(const unsigned char* arr1, const unsigned char* arr2, 
															 const std::size_t& len) {
			for (std::size_t i = 0; i < len; ++i) {
				if (arr1[i] < arr2[i]) { return true; }
				if (arr1[i] > arr2[i]) { return false; }
				// loop continues only when arr1[i] == arr2[i]
			}
			// nothing was returned in the loop, so values are equal
			return true;
		}
		bool is_zero(const unsigned char* arr, const std::size_t& len) {
			for (std::size_t i = 0; i < len; ++i) {
				if (arr[i] != 0) { return false; }
			}
			return true;
		}
		void set_to_zero(unsigned char* arr, const std::size_t& len) {
			for (std::size_t i = 0; i < len; ++i) { arr[i] = 0; }
		}
		namespace division {
			int align_dividend(unsigned char* dividend, const unsigned char* divisor, const std::size_t& len) {
				int result = 0;
				while (less_or_equal(divisor, dividend, len)) {
					bits::shift_right(dividend, 1, len);
					--result;
				}
				return result;
			}
			int align_divisor(const unsigned char* dividend, unsigned char* divisor, const std::size_t& len) {
			  int result = 0;
				while (!less_or_equal(divisor, dividend, len)) {
					bits::shift_right(divisor, 1, len);
					++result;
				}
				return result;
			}
			int align(unsigned char* dividend, unsigned char* divisor, const std::size_t& len) {
				int result = align_dividend(dividend, divisor, len);
				result += align_divisor(dividend, divisor, len);
				return result;
			}
			int readjust(const unsigned char* dividend, unsigned char* divisor, const std::size_t& len) {
				int adjustment = 0;
				while (!less_or_equal(divisor, dividend, len)) {
					bits::shift_right(divisor, 1, len);
					++adjustment;
				}
				return adjustment;
			}
		}
	}
	namespace bits {
		unsigned char get_mask(const std::size_t& bits_to_show) {
			unsigned char result = 0;
			for (std::size_t i = 0; i < bits_to_show; ++i) { result = 1 + (result << 1); }
			return result;
		}
		void shift_right(unsigned char* arr, const std::size_t& places, const std::size_t& arr_size) {
			std::size_t byte_places = places / 8;
			std::size_t bit_places = places % 8;
			helpers::shift_right(arr, byte_places, arr_size);
			// now we only have to worry about bit_places
			if (bit_places == 0) { return; }
			unsigned char mask = get_mask(bit_places);
			std::size_t offset = 8 - bit_places;
			unsigned char tmp;
			std::size_t i = arr_size - 1;
			// the first byte_places positions are all 0, so we don't have to worry about them
			while (i > byte_places) {
				// shift the given position
				arr[i] >>= bit_places;
				// get the relevant bits from the position to the left and add them in
				tmp = arr[i - 1] & mask;
				arr[i] += (tmp << offset);
				--i;
			}
			arr[byte_places] >>= bit_places;
		}
		void set(unsigned char* arr, const std::size_t& bit_to_set) {
			std::size_t array_index = bit_to_set / 8;
			std::size_t bit_position = bit_to_set % 8;
			unsigned char mask = 1 << (7 - bit_position);
			arr[array_index] |= mask;
		}
	}

	/**
	 * result is assumed to have length a_len + b_len + 1
	 * this needs to be set up so that b_len <= a_len
	 */
	void karatsuba_multiply(const unsigned char* a, const unsigned char* b,
			unsigned char* result, const std::size_t& a_len, const std::size_t& b_len) {
	}
	
	/**
	 * result is assumed to have length a_len + b_len + 1
	 * this needs to be set up so that b_len <= a_len
	 */
	void school_multiply(const unsigned char* a, const unsigned char* b,
			unsigned char* result, const std::size_t& a_len, const std::size_t& b_len) {
		// end_pos tracks the least significant digit in the result array
		std::size_t a_index, b_index, result_index, end_pos;
		unsigned int carry, tmp;
		const std::size_t RESULT_LEN = a_len + b_len + 1;
		// initialize result to 0
		for (result_index = 0; result_index < RESULT_LEN; ++result_index) {
			result[result_index] = 0;
		}
		b_index = b_len;
		end_pos = RESULT_LEN;
		while (b_index > 0) {
			--b_index;
			result_index = end_pos;
			a_index = a_len;
			carry = 0;
			while (a_index > 0) {
				--a_index;
				--result_index;
				tmp = carry + result[result_index] + a[a_index] * b[b_index];
				result[result_index] = tmp % RADIX;
				carry = tmp / RADIX;
			}
			result[result_index - 1] = carry;
			--end_pos;
		}
	}

	void add(const unsigned char* a, const unsigned char* b, unsigned char* result,
			const std::size_t& len) {
		unsigned int carry = 0, tmp_sum;
		std::size_t i = len;
		while (i > 0) {
			--i;
			tmp_sum = carry + a[i] + b[i];
			result[i] = tmp_sum % RADIX;
			carry = tmp_sum / RADIX;
		}
	}

	void add(const unsigned char* a, const unsigned char* b,
			unsigned char* result, const std::size_t& a_len, const std::size_t& b_len) {
		unsigned int carry = 0, tmp_sum;
		unsigned int a_b_diff = a_len - b_len;
		std::size_t i = b_len;
		while (i > 0) {
			--i;
			tmp_sum = carry + a[i + a_b_diff] + b[i];
			result[i + a_b_diff + 1] = tmp_sum % RADIX;
			carry = tmp_sum / RADIX;
		}
		i = a_b_diff;
		while (i > 0) {
			--i;
			tmp_sum = carry + a[i];
			result[i + 1] = tmp_sum % RADIX;
			carry = tmp_sum / RADIX;
		}
		result[0] = carry;
	}

	void subtract(const unsigned char* a, const unsigned char* b, unsigned char* result,
			const std::size_t& len) {
		std::size_t i;
		// copy of a is needed because we have to modify it when we borrow
		unsigned char* a_copy = new unsigned char[len];
		for (i = 0; i < len; ++i) { a_copy[i] = a[i]; }
		i = len;
		int borrow_index;
		while (i > 0) {
			--i;
			if (a_copy[i] >= b[i]) { result[i] = a_copy[i] - b[i]; }
			else {
				result[i] = RADIX + a_copy[i] - b[i];
				borrow_index = i - 1;
				// get to a point where there is something to borrow
				while (borrow_index > 0 && a_copy[borrow_index] == 0) {
					a_copy[borrow_index--] = RADIX_MINUS_ONE;
				}
				--a_copy[borrow_index];
			}
		}
		delete[] a_copy;
	}
	void subtract(const unsigned char* a, const unsigned char* b, unsigned char* result,
			const std::size_t& a_len, const std::size_t& b_len) {
		std::size_t i;
		// copy of a is needed because we have to modify it when we borrow
		unsigned char* a_copy = new unsigned char[a_len];
		for (i = 0; i < a_len; ++i) { a_copy[i] = a[i]; }
		std::size_t ab_len_diff = a_len - b_len;
		i = b_len;
		int borrow_index;
		while (i > 0) {
			--i;
			if (a_copy[i + ab_len_diff] >= b[i]) { result[i + ab_len_diff] = a_copy[i + ab_len_diff] - b[i]; }
			else {
				result[i + ab_len_diff] = RADIX + a_copy[i + ab_len_diff] - b[i];
				borrow_index = i + ab_len_diff - 1;
				// get to a point where there is something to borrow
				while (borrow_index > 0 && a_copy[borrow_index] == 0) {
					a_copy[borrow_index--] = RADIX_MINUS_ONE;
				}
				--a_copy[borrow_index];
			}
		}
		i = ab_len_diff;
		while (i > 0) {
			--i;
			result[i] = a_copy[i];
		}
		delete[] a_copy;
	}
	void divide(const unsigned char* dividend, const unsigned char* divisor, unsigned char* result,
			const std::size_t& len, const std::size_t& int_digits) {
		// throw exception if client tries to divide by 0
		if (helpers::is_zero(divisor, len)) {
			throw std::invalid_argument("Division by 0 is not allowed!");
    }
		// initialize result to 0
		helpers::set_to_zero(result, len);
		// we are done if dividend is 0, so return
		if (helpers::is_zero(dividend, len)) { return; }
		// we will need copies of double size for manipulation
		const std::size_t WORKING_LEN = len * 2;
		// this is the bit length of the original values (not the expanded copies)
		const std::size_t BIT_LENGTH = len * 8;
		// 2 dividend arrays are needed because we need to subtract, then use result as new dividend
		unsigned char* dividend_copy1 = new unsigned char[WORKING_LEN];
		unsigned char* dividend_copy2 = new unsigned char[WORKING_LEN];
		unsigned char* divisor_copy = new unsigned char[WORKING_LEN];
		// copy dividend into copy1 (we don't yet need to do anything with dividend_copy2)
		helpers::copy_into(dividend, dividend_copy1, len, WORKING_LEN);
		helpers::copy_into(divisor, divisor_copy, len, WORKING_LEN);
		int starting_bit = int_digits * 8 - 1;
		starting_bit += helpers::division::align(dividend_copy1, divisor_copy, WORKING_LEN);
		bool throw_overflow_exception = (starting_bit < 0);
		std::size_t nonnegative_starting_bit = starting_bit;
		if (!throw_overflow_exception) {
			while (nonnegative_starting_bit < BIT_LENGTH) {
				bits::set(result, nonnegative_starting_bit);
				subtract(dividend_copy1, divisor_copy, dividend_copy2, WORKING_LEN);
				nonnegative_starting_bit += helpers::division::readjust(dividend_copy2, divisor_copy, WORKING_LEN);
				// check the condition of the while loop since we are using double storage
				if (nonnegative_starting_bit >= BIT_LENGTH) { break; }
				bits::set(result, nonnegative_starting_bit);
				subtract(dividend_copy2, divisor_copy, dividend_copy1, WORKING_LEN);
				nonnegative_starting_bit += helpers::division::readjust(dividend_copy1, divisor_copy, WORKING_LEN);
			}
		}
		delete[] dividend_copy1;
		delete[] dividend_copy2;
		delete[] divisor_copy;
		if (throw_overflow_exception) {
			throw std::overflow_error("Result of division is too large to compute!");
		}
	}
}
