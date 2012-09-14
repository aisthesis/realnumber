/**
 * Test Suite for array_arithmetic namespace methods
 * command for running tests:
 * ./array_arithmetic_test --log_level=test_suite
 * http://www.alittlemadness.com/2009/03/31/c-unit-testing-with-boosttest/
 * http://www.ibm.com/developerworks/aix/library/au-ctools1_boost/
 * http://www.boost.org/doc/libs/1_34_1/libs/test/doc/components/test_tools/reference/index.html
 *
 * Created by Marshall Farrier on 6/28/2012.
 * Copyright (c) 2012 Marshall Farrier
 * license http://opensource.org/licenses/gpl-license.php GNU Public License
 */

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <iostream>
#include <stdexcept>

#include "array_arithmetic.h"

struct ArraySize {
	int small;
	int large;
	unsigned char* small_array;
	unsigned char* large_array;
	ArraySize() {
		small = 4;
		large = 8;
		small_array = new unsigned char[small];
		large_array = new unsigned char[large];
	}
	~ArraySize() {
		delete[] small_array;
		delete[] large_array;
	}
};

BOOST_FIXTURE_TEST_SUITE(conversion, ArraySize)
	BOOST_AUTO_TEST_CASE(one) {
		const unsigned int ONE = 1;
		array_arithmetic::convert::from_int(ONE, small_array, small);
		array_arithmetic::convert::from_int(ONE, large_array, large);
		BOOST_CHECK_EQUAL(array_arithmetic::convert::to_int(small_array, small), ONE);
		BOOST_CHECK_EQUAL(array_arithmetic::convert::to_int(large_array, large), ONE);
	}

	BOOST_AUTO_TEST_CASE(one_billion) {
		const unsigned int ONE_BILLION = 1000000000;
		array_arithmetic::convert::from_int(ONE_BILLION, small_array, small);
		array_arithmetic::convert::from_int(ONE_BILLION, large_array, large);
		BOOST_CHECK_EQUAL(array_arithmetic::convert::to_int(small_array, small), ONE_BILLION);
		BOOST_CHECK_EQUAL(array_arithmetic::convert::to_int(large_array, large), ONE_BILLION);
	}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(conversion_helpers)
	BOOST_AUTO_TEST_CASE(to_digit_array) {
		unsigned char* digits1 = array_arithmetic::convert::helpers::to_digit_array("123");
		BOOST_CHECK_EQUAL(digits1[0], static_cast<unsigned char>(1));
		BOOST_CHECK_EQUAL(digits1[1], static_cast<unsigned char>(2));
		BOOST_CHECK_EQUAL(digits1[2], static_cast<unsigned char>(3));
		delete[] digits1;
		BOOST_CHECK_THROW(array_arithmetic::convert::helpers::to_digit_array("12."), std::invalid_argument); 
	}

	BOOST_AUTO_TEST_CASE(times_radix) {
		unsigned char digits[] = {3, 3};
		BOOST_CHECK_EQUAL(array_arithmetic::convert::helpers::times_radix(digits, 2), 84);
		BOOST_CHECK_EQUAL(digits[0], 4);
		BOOST_CHECK_EQUAL(digits[1], 8);
	}
	BOOST_AUTO_TEST_CASE(times_ten) {
		unsigned char digits[] = {100, 100};
		BOOST_CHECK_EQUAL(array_arithmetic::convert::helpers::times_ten(digits, 2), 3);
		BOOST_CHECK_EQUAL(digits[0], 235);
		BOOST_CHECK_EQUAL(digits[1], 232);
	}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(helpers)
	BOOST_AUTO_TEST_CASE(copy_into) {
		unsigned char source_arr[] = {0, 1, 2};
		unsigned char target_arr[5];
		array_arithmetic::helpers::copy_into(source_arr, target_arr, 3, 5);
		BOOST_CHECK_EQUAL(target_arr[1], 1);
		BOOST_CHECK_EQUAL(target_arr[3], 0);
	}
	BOOST_AUTO_TEST_CASE(shift_right) {
		unsigned char arr[] = {1, 2, 3, 4, 5};
		array_arithmetic::helpers::shift_right(arr, 2, 5);
		BOOST_CHECK_EQUAL(arr[0], 0);
		BOOST_CHECK_EQUAL(arr[1], 0);
		BOOST_CHECK_EQUAL(arr[2], 1);
		BOOST_CHECK_EQUAL(arr[4], 3);
	}
	BOOST_AUTO_TEST_CASE(less_or_equal) {
		unsigned char arr1[] = {1, 1};
		unsigned char arr2[] = {1, 2};
		unsigned char arr3[] = {2, 1};
		// case of equality
		BOOST_CHECK(array_arithmetic::helpers::less_or_equal(arr1, arr1, 2));
		// inequality
		BOOST_CHECK(array_arithmetic::helpers::less_or_equal(arr1, arr2, 2));
		BOOST_CHECK(array_arithmetic::helpers::less_or_equal(arr1, arr3, 2));
		// false case
		BOOST_CHECK(!array_arithmetic::helpers::less_or_equal(arr2, arr1, 2));
	}
	BOOST_AUTO_TEST_CASE(is_zero) {
		unsigned char arr1[] = {0, 0};
		unsigned char arr2[] = {0, 0, 0, 0, 1};
		unsigned char arr3[] = {1, 0};
		BOOST_CHECK(array_arithmetic::helpers::is_zero(arr1, 2));
		BOOST_CHECK(!array_arithmetic::helpers::is_zero(arr2, 5));
		BOOST_CHECK(!array_arithmetic::helpers::is_zero(arr3, 2));
	}
	BOOST_AUTO_TEST_CASE(set_to_zero) {
		unsigned char arr[] = {1, 129, 255};
		array_arithmetic::helpers::set_to_zero(arr, 3);
		BOOST_CHECK_EQUAL(arr[0], 0);
		BOOST_CHECK_EQUAL(arr[1], 0);
		BOOST_CHECK_EQUAL(arr[2], 0);
	}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(division_helpers)
	BOOST_AUTO_TEST_CASE(align_dividend) {
		unsigned char arr1[] = {129, 0};
		unsigned char arr2[] = {0, 129};
		int result = array_arithmetic::helpers::division::align_dividend(arr1, arr2, 2);
		// verify proper return value
		BOOST_CHECK_EQUAL(result, -9);
		// verify that arr1 was properly shifted
		BOOST_CHECK_EQUAL(arr1[0], 0);
		BOOST_CHECK_EQUAL(arr1[1], 64);
		// verify that arr2 is unchanged
		BOOST_CHECK_EQUAL(arr2[0], 0);
		BOOST_CHECK_EQUAL(arr2[1], 129);
	}
	BOOST_AUTO_TEST_CASE(align_divisor) {
		unsigned char arr1[] = {129, 0};
		unsigned char arr2[] = {0, 129};
		// verify proper return value
		BOOST_CHECK_EQUAL(array_arithmetic::helpers::division::align_divisor(arr2, arr1, 2), 8);
		// verify that arr1 was properly shifted
		BOOST_CHECK_EQUAL(arr1[0], 0);
		BOOST_CHECK_EQUAL(arr1[1], 129);
		// verify that arr2 is unchanged
		BOOST_CHECK_EQUAL(arr2[0], 0);
		BOOST_CHECK_EQUAL(arr2[1], 129);
	}
	BOOST_AUTO_TEST_CASE(align) {
		unsigned char arr1[] = {129, 0};
		unsigned char arr2[] = {0, 129};
		// verify proper return value
		BOOST_CHECK_EQUAL(array_arithmetic::helpers::division::align(arr1, arr2, 2), -8);
		// arr1 properly shifted
		BOOST_CHECK_EQUAL(arr1[0], 0);
		BOOST_CHECK_EQUAL(arr1[1], 64);
		// arr2 properly shifted
		BOOST_CHECK_EQUAL(arr2[0], 0);
		BOOST_CHECK_EQUAL(arr2[1], 64);
	}
	BOOST_AUTO_TEST_CASE(readjust) {
		unsigned char arr1[] = {129, 0};
		unsigned char arr2[] = {0, 129};
		unsigned char zero_arr[] = {0, 0};
		unsigned char arr3[] = {129, 129};
		// verify proper return value
		BOOST_CHECK_EQUAL(array_arithmetic::helpers::division::readjust(arr2, arr1, 2), 8);
		// arr1 properly shifted
		BOOST_CHECK_EQUAL(arr1[0], 0);
		BOOST_CHECK_EQUAL(arr1[1], 129);
		// arr2 unchanged
		BOOST_CHECK_EQUAL(arr2[0], 0);
		BOOST_CHECK_EQUAL(arr2[1], 129);
		// check edge case when dividend is 0
		BOOST_CHECK_EQUAL(array_arithmetic::helpers::division::readjust(zero_arr, arr3, 2), 16);
		BOOST_CHECK_EQUAL(arr3[0], 0);
		BOOST_CHECK_EQUAL(arr3[1], 0);
	}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(bits)
	BOOST_AUTO_TEST_CASE(get_mask) {
		BOOST_CHECK_EQUAL(array_arithmetic::bits::get_mask(0), 0);
		BOOST_CHECK_EQUAL(array_arithmetic::bits::get_mask(2), 3);
		BOOST_CHECK_EQUAL(array_arithmetic::bits::get_mask(8), 255);
		BOOST_CHECK_EQUAL(array_arithmetic::bits::get_mask(7), 127);
	}
	BOOST_AUTO_TEST_CASE(shift_right) {
		unsigned char arr[] = {129, 129, 129};
		array_arithmetic::bits::shift_right(arr, 1, 3);
		BOOST_CHECK_EQUAL(arr[0], 64);
		BOOST_CHECK_EQUAL(arr[1], 192);
		BOOST_CHECK_EQUAL(arr[2], 192);
		// reset array to original values
		for (std::size_t i = 0; i < 3; ++i) { arr[i] = 129; }
		array_arithmetic::bits::shift_right(arr, 15, 3);
		BOOST_CHECK_EQUAL(arr[0], 0);
		BOOST_CHECK_EQUAL(arr[1], 1);
		BOOST_CHECK_EQUAL(arr[2], 3);
	}
	BOOST_AUTO_TEST_CASE(set) {
		unsigned char arr[] = {0, 0};
		// test setting first bit at each index
		array_arithmetic::bits::set(arr, 0);
		BOOST_CHECK_EQUAL(arr[0], 128);
		array_arithmetic::bits::set(arr, 8);
		BOOST_CHECK_EQUAL(arr[1], 128);
		// reset arr
		for (std::size_t i = 0; i < 2; ++i) { arr[i] = 0; }
		// test setting last bit at each index
		array_arithmetic::bits::set(arr, 7);
		BOOST_CHECK_EQUAL(arr[0], 1);
		array_arithmetic::bits::set(arr, 15);
		BOOST_CHECK_EQUAL(arr[1], 1);
		// verify that there is no change to bits that are already set
		arr[0] = 255;
		array_arithmetic::bits::set(arr, 3);
		BOOST_CHECK_EQUAL(arr[0], 255);
	}
BOOST_AUTO_TEST_SUITE_END()

struct MathTestCases {
	unsigned int one;
	unsigned int two;
	unsigned int three;
	unsigned int x1;
	unsigned int x2;
	unsigned int one_billion;
	unsigned int two_billion;
	unsigned int three_billion;
	std::size_t small_arr_len;
	std::size_t bigger_arr_len;	// to be used for a array
	unsigned char* small_arr1;
	unsigned char* small_arr2;
	unsigned char* small_arr3;
	unsigned char* smaller_arr;
	unsigned char* small_result_arr;
	unsigned char* bigger_arr1;
	unsigned char* bigger_arr2;
	unsigned char* bigger_arr3;
	unsigned char* big_arr;
	unsigned char* big_sum_arr;
	unsigned char* big_product_arr;
	MathTestCases() {
		one = 1;
		two = 2;
		three = 3;
		x1 = 128;
		x2 = 256;
		one_billion = 1000000000;
		two_billion = 2000000000;
		three_billion = 3000000000;
		small_arr_len = 2;
		bigger_arr_len = 4;
		small_arr1 = new unsigned char[small_arr_len];
		small_arr2 = new unsigned char[small_arr_len];
		small_arr3 = new unsigned char[small_arr_len];
		smaller_arr = new unsigned char[small_arr_len - 1];
		small_result_arr = new unsigned char[small_arr_len + 1];
		bigger_arr1 = new unsigned char[bigger_arr_len];
		bigger_arr2 = new unsigned char[bigger_arr_len];
		bigger_arr3 = new unsigned char[bigger_arr_len];
		big_arr = new unsigned char[bigger_arr_len - 2];
		big_sum_arr = new unsigned char[bigger_arr_len + 1];
		big_product_arr = new unsigned char[2 * bigger_arr_len - 1];
	}
	~MathTestCases() {
		delete[] small_arr1;
		delete[] small_arr2;
		delete[] small_arr3;
		delete[] smaller_arr;
		delete[] small_result_arr;
		delete[] bigger_arr1;
		delete[] bigger_arr2;
		delete[] bigger_arr3;
		delete[] big_arr;
		delete[] big_sum_arr;
		delete[] big_product_arr;
	}
};
BOOST_FIXTURE_TEST_SUITE(addition, MathTestCases)
	// uniform array length addition
	BOOST_AUTO_TEST_CASE(uniform_length) {
		// small array size
		array_arithmetic::convert::from_int(one, small_arr1, small_arr_len);
		array_arithmetic::convert::from_int(two, small_arr2, small_arr_len);
		array_arithmetic::add(small_arr1, small_arr2, small_arr3, small_arr_len);
		BOOST_CHECK_EQUAL(array_arithmetic::convert::to_int(small_arr3, small_arr_len), three);
		array_arithmetic::convert::from_int(x1, small_arr1, small_arr_len);
		array_arithmetic::add(small_arr1, small_arr1, small_arr3, small_arr_len);
		BOOST_CHECK_EQUAL(array_arithmetic::convert::to_int(small_arr3, small_arr_len), x2);
		// large array size
		array_arithmetic::convert::from_int(one_billion, bigger_arr1, bigger_arr_len);
		array_arithmetic::convert::from_int(two_billion, bigger_arr2, bigger_arr_len);
		array_arithmetic::add(bigger_arr1, bigger_arr2, bigger_arr3, bigger_arr_len);
		BOOST_CHECK_EQUAL(array_arithmetic::convert::to_int(bigger_arr3, bigger_arr_len),
				three_billion);
	}
	// adding arrays of different sizes
	BOOST_AUTO_TEST_CASE(different_lengths) {
		// inputs of equal size
		array_arithmetic::convert::from_int(one_billion, bigger_arr1, bigger_arr_len);
		array_arithmetic::convert::from_int(two_billion, bigger_arr2, bigger_arr_len);
		array_arithmetic::add(bigger_arr1, bigger_arr2, big_sum_arr, bigger_arr_len, bigger_arr_len);
		BOOST_CHECK_EQUAL(array_arithmetic::convert::to_int(big_sum_arr, bigger_arr_len + 1),
				three_billion);
		// different input sizes: small
		array_arithmetic::convert::from_int(one, small_arr1, small_arr_len);
		array_arithmetic::convert::from_int(two, smaller_arr, small_arr_len - 1);
		array_arithmetic::add(small_arr1, smaller_arr, small_result_arr, small_arr_len, small_arr_len - 1);
		BOOST_CHECK_EQUAL(array_arithmetic::convert::to_int(small_result_arr, small_arr_len + 1), three);
		// different input sizes: large
		array_arithmetic::convert::from_int(one_billion, bigger_arr1, bigger_arr_len);
		array_arithmetic::convert::from_int(255, big_arr, bigger_arr_len - 2);
		array_arithmetic::add(bigger_arr1, big_arr, big_sum_arr, bigger_arr_len, bigger_arr_len - 2);
		BOOST_CHECK_EQUAL(array_arithmetic::convert::to_int(big_sum_arr, bigger_arr_len + 1),
				static_cast<unsigned int>(1000000255));
	}
BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(subtraction, MathTestCases)
	// uniform array length
	BOOST_AUTO_TEST_CASE(uniform_length) {
		array_arithmetic::convert::from_int(three_billion, bigger_arr1, bigger_arr_len);
		array_arithmetic::convert::from_int(two_billion, bigger_arr2, bigger_arr_len);
		array_arithmetic::subtract(bigger_arr1, bigger_arr2, bigger_arr3, bigger_arr_len);
		BOOST_CHECK_EQUAL(array_arithmetic::convert::to_int(bigger_arr3, bigger_arr_len),
			one_billion);
	}
	// different array lengths
	BOOST_AUTO_TEST_CASE(different_lengths) {
		array_arithmetic::convert::from_int(1000000255, bigger_arr1, bigger_arr_len);
		array_arithmetic::convert::from_int(255, big_arr, bigger_arr_len - 2);
		array_arithmetic::subtract(bigger_arr1, big_arr, bigger_arr2, bigger_arr_len, bigger_arr_len - 2);
		BOOST_CHECK_EQUAL(array_arithmetic::convert::to_int(bigger_arr2, bigger_arr_len), one_billion);
	}
BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(multiplication, MathTestCases)
	BOOST_AUTO_TEST_CASE(school_multiply) {
		array_arithmetic::convert::from_int(5000000, bigger_arr1, bigger_arr_len);
		array_arithmetic::convert::from_int(400, big_arr, bigger_arr_len - 2);
		array_arithmetic::school_multiply(bigger_arr1, big_arr, big_product_arr, bigger_arr_len, bigger_arr_len - 2);
		BOOST_CHECK_EQUAL(array_arithmetic::convert::to_int(big_product_arr, bigger_arr_len * 2 - 1), 
				two_billion);
	}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(division)
	BOOST_AUTO_TEST_CASE(divide_by_zero) {
		unsigned char divisor[] = {0, 0, 0};
		unsigned char dividend[] = {1, 1, 1};
		unsigned char result[] = {129, 129, 129};
		BOOST_CHECK_THROW(array_arithmetic::divide(dividend, divisor, result, 3, 1), std::invalid_argument);
	}
	BOOST_AUTO_TEST_CASE(dividend_is_zero) {
		unsigned char dividend[] = {0, 0};
		unsigned char divisor[] = {0, 1};
		unsigned char result[] = {129, 13};
		array_arithmetic::divide(dividend, divisor, result, 2, 1);
		BOOST_CHECK_EQUAL(result[0], 0);
		BOOST_CHECK_EQUAL(result[1], 0);
	}
	BOOST_AUTO_TEST_CASE(result_too_big) {
		unsigned char dividend[] = {128, 0};
		unsigned char divisor[] = {0, 128};	// represents 0.5
		unsigned char result[] = {255, 255};
		BOOST_CHECK_THROW(array_arithmetic::divide(dividend, divisor, result, 2, 1), std::overflow_error);
	}
	BOOST_AUTO_TEST_CASE(exact_result) {
		// this also tests that we exit the loop properly when the long division subtraction
		// gives a 0 result
		unsigned char dividend[] = {1, 0, 0};
		unsigned char divisor[] = {2, 0, 0};	// result should represent 0.5
		unsigned char result[] = {3, 4, 5};
		array_arithmetic::divide(dividend, divisor, result, 3, 1);
		BOOST_CHECK_EQUAL(result[0], 0);
		BOOST_CHECK_EQUAL(result[1], 128);
		BOOST_CHECK_EQUAL(result[2], 0);
	}
	BOOST_AUTO_TEST_CASE(approximate_result) {
		unsigned char dividend[] = {1, 0, 0, 0};
		unsigned char divisor[] = {0, 3, 0, 0};
		unsigned char result[] = {0, 0, 0, 0};
		// result will represent 256 / 3 = 85.3333
		array_arithmetic::divide(dividend, divisor, result, 4, 1);
		BOOST_CHECK_EQUAL(result[0], 85);
		// 85 infinitely repeating is the representation of 1 / 3 in base 256
		BOOST_CHECK_EQUAL(result[1], 85);
		BOOST_CHECK_EQUAL(result[2], 85);
		BOOST_CHECK_EQUAL(result[3], 85);
	}
BOOST_AUTO_TEST_SUITE_END()
