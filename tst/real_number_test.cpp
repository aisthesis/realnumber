/**
 * Test Suite for RealNumber class
 * command for running tests:
 * ./real_number_test --log_level=test_suite
 * http://www.alittlemadness.com/2009/03/31/c-unit-testing-with-boosttest/
 * http://www.ibm.com/developerworks/aix/library/au-ctools1_boost/
 * http://www.boost.org/doc/libs/1_34_1/libs/test/doc/components/test_tools/reference/index.html
 *
 * Created by Marshall Farrier on 6/22/12.
 * Copyright (c) 2012 Marshall Farrier
 * license http://opensource.org/licenses/gpl-license.php GNU Public License
 */

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <string>
#include <stdexcept>
#include "real_number.h"

struct RealNumberTestCases {
	std::string zero_str;
	std::string one_str;
	std::string two_str;
	std::string big_num_str;
	std::string small_num_str;
	// throws std::invalid_argument because it is too big
	std::string overflow_num_str;
	// sets maximum difference for 2 real numbers to be viewed as equal
	std::string very_small_num_str;
	std::string two_plus_small_str;

	RealNumber zero_rn;
	RealNumber one_rn;
	RealNumber two_rn;
	RealNumber big_num_rn;
	RealNumber small_num_rn;
	RealNumber very_small_num_rn;
	RealNumber two_plus_small_rn;

	RealNumberTestCases() {
		zero_str = "0.0";
		one_str = "1.0";
		two_str = "2.0";
		overflow_num_str = "10000000000.0";
		big_num_str = "900000000.0";
		// used as denominator in division test, so can't be so small as to cause overflow
		small_num_str = "0.001";
		// 10 ^ -50 should be fine in this context
		very_small_num_str = "0.00000000000000000000000000000000000000000000000001"; 
		two_plus_small_str = "2.001";

		zero_rn = RealNumber(zero_str);
		one_rn = RealNumber(one_str);
		two_rn = RealNumber(two_str);
		big_num_rn = RealNumber(big_num_str);
		small_num_rn = RealNumber(small_num_str);
		very_small_num_rn = RealNumber(very_small_num_str);
		two_plus_small_rn = RealNumber(two_plus_small_str);
	}
};

BOOST_FIXTURE_TEST_SUITE(constructor, RealNumberTestCases)
	BOOST_AUTO_TEST_CASE(default_constructor) {
		RealNumber default_zero;
		BOOST_REQUIRE_EQUAL(default_zero.to_decimal_string(), zero_rn.to_decimal_string());
	}

	BOOST_AUTO_TEST_CASE(equals1) {
		BOOST_REQUIRE_EQUAL(one_rn.to_decimal_string().substr(0, one_str.length()), one_str);
	}

	BOOST_AUTO_TEST_CASE(equals2) {
		BOOST_REQUIRE_EQUAL(two_rn.to_decimal_string().substr(0, two_str.length()), two_str);
	}

	BOOST_AUTO_TEST_CASE(equals_big_num_str) {
		BOOST_REQUIRE_EQUAL(big_num_rn.to_decimal_string().substr(0,
				big_num_str.length()), big_num_str);
	}

	BOOST_AUTO_TEST_CASE(equals_small_number1) {
		// tests whether all but the last digit match, as conversion will give back
		// "0.0009999999999..." on input "0.001"
		BOOST_REQUIRE_EQUAL(small_num_rn.to_decimal_string().substr(0, 
				small_num_str.length() - 1), small_num_str.substr(0, small_num_str.length() - 1));
	}

	BOOST_AUTO_TEST_CASE(equals_small_number2) {
		// tests only the last digit for the small number and allows 2 possible values
		std::string output = small_num_rn.to_decimal_string();
		char original_digit = small_num_str[small_num_str.length() - 1];
		char output_digit = output[small_num_str.length() - 1];
		bool the_same = original_digit == output_digit;
		if (!the_same) {
			BOOST_TEST_MESSAGE("Last digit of input differs from corresponding digit of output:");
			BOOST_TEST_MESSAGE(std::string("input digit: ").append(1, original_digit));
			BOOST_TEST_MESSAGE(std::string("output digit: ").append(1, output_digit));
		}
		bool close_enough = original_digit - output_digit == 1;
		close_enough = close_enough && (output.substr(small_num_str.length(), 3) == "999");
		if (close_enough) {
			BOOST_TEST_MESSAGE("Difference between input and output is minimal and results from base conversion.");
			BOOST_TEST_MESSAGE(std::string("Relevant digits: ")
					.append(output.substr(small_num_str.length() - 1, 4)));
		}
		BOOST_REQUIRE(the_same || close_enough);
	}

	BOOST_AUTO_TEST_CASE(number_too_big) {
		BOOST_CHECK_THROW(RealNumber overflow_num_rn(overflow_num_str), std::invalid_argument);
	}

	BOOST_AUTO_TEST_CASE(copy_constructor) {
		RealNumber two_rn_copy(two_rn);
		BOOST_REQUIRE_EQUAL(two_rn_copy.to_decimal_string(), two_rn.to_decimal_string());
	}

	BOOST_AUTO_TEST_CASE(assignment_operator) {
		RealNumber one_rn_assigned = one_rn;
		BOOST_REQUIRE_EQUAL(one_rn_assigned.to_decimal_string(), one_rn.to_decimal_string());
	}
BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(comparison_operators, RealNumberTestCases)
	BOOST_AUTO_TEST_CASE(greater_than) {
		BOOST_CHECK(big_num_rn > two_rn);
		BOOST_CHECK(big_num_rn > small_num_rn);
		BOOST_CHECK(!(one_rn > two_rn));
		BOOST_CHECK(!(two_rn > two_rn));
	}

	BOOST_AUTO_TEST_CASE(greater_or_equal) {
		RealNumber one_rn_again(one_str);
		BOOST_CHECK(big_num_rn >= one_rn);
		BOOST_CHECK(one_rn_again >= one_rn);
		BOOST_CHECK(one_rn >= one_rn_again);
		BOOST_CHECK(two_rn >= two_rn);
		BOOST_CHECK(one_rn >= small_num_rn);
		BOOST_CHECK(!(one_rn >= big_num_rn));
	}

	BOOST_AUTO_TEST_CASE(less_than) {
		BOOST_CHECK(small_num_rn < one_rn);
		BOOST_CHECK(small_num_rn < big_num_rn);
		BOOST_CHECK(!(big_num_rn < one_rn));
		BOOST_CHECK(!(one_rn < one_rn));
	}

	BOOST_AUTO_TEST_CASE(less_or_equal) {
		RealNumber big_num_rn_again(big_num_str);
		BOOST_CHECK(small_num_rn <= one_rn);
		BOOST_CHECK(small_num_rn <= big_num_rn);
		BOOST_CHECK(one_rn <= one_rn);
		BOOST_CHECK(big_num_rn_again <= big_num_rn);
		BOOST_CHECK(!(two_rn <= one_rn));
	}

	BOOST_AUTO_TEST_CASE(not_equal) {
		RealNumber big_num_rn_copy(big_num_rn);
		BOOST_CHECK(big_num_rn != two_rn);
		BOOST_CHECK(big_num_rn != small_num_rn);
		BOOST_CHECK(small_num_rn != one_rn);
		BOOST_CHECK(small_num_rn != big_num_rn);
		BOOST_CHECK(!(one_rn != one_rn));
		BOOST_CHECK(!(big_num_rn_copy != big_num_rn));
	}

	BOOST_AUTO_TEST_CASE(equals) {
		RealNumber big_num_rn_copy(big_num_rn);
		RealNumber two_rn_copy(two_rn);
		RealNumber small_num_again(small_num_str);
		BOOST_CHECK(big_num_rn == big_num_rn_copy);
		BOOST_CHECK(two_rn_copy == two_rn);
		BOOST_CHECK(small_num_rn == small_num_again);
		BOOST_CHECK(!(two_rn == one_rn));
	}
BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(arithmetic_operators, RealNumberTestCases)
	BOOST_AUTO_TEST_CASE(addition) {
		BOOST_CHECK(one_rn + one_rn == two_rn);
		BOOST_CHECK(one_rn + zero_rn == one_rn);
		BOOST_CHECK(two_rn + small_num_rn == two_plus_small_rn);
		BOOST_CHECK(big_num_rn + very_small_num_rn > big_num_rn);
		BOOST_CHECK(big_num_rn + very_small_num_rn != big_num_rn);
	}

	BOOST_AUTO_TEST_CASE(subtraction) {
		RealNumber one_rn_copy(one_rn);
		BOOST_CHECK(one_rn - one_rn_copy == zero_rn);
		BOOST_CHECK(two_rn - one_rn == one_rn_copy);
		BOOST_CHECK(big_num_rn - very_small_num_rn < big_num_rn);
	}

	BOOST_AUTO_TEST_CASE(multiplication) {
		BOOST_CHECK(one_rn * two_rn == two_rn);
		BOOST_CHECK(big_num_rn * zero_rn == zero_rn);
		BOOST_CHECK(two_rn * two_rn == RealNumber("4.0"));
		BOOST_CHECK(big_num_rn * small_num_rn < big_num_rn);
		BOOST_CHECK(two_rn < two_rn * RealNumber("1.01"));
		BOOST_CHECK(two_rn * very_small_num_rn > very_small_num_rn);
	}

	BOOST_AUTO_TEST_CASE(division) {
		BOOST_CHECK(one_rn / two_rn < RealNumber("0.50000001"));
		BOOST_CHECK(one_rn / two_rn > RealNumber("0.49999999"));
		BOOST_CHECK(two_rn / big_num_rn > zero_rn);
		BOOST_CHECK(one_rn / small_num_rn > one_rn);
		BOOST_CHECK_THROW(one_rn / zero_rn, std::invalid_argument);
	}
BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(other_methods, RealNumberTestCases)
	BOOST_AUTO_TEST_CASE(difference_method) {
		BOOST_CHECK(big_num_rn.difference(two_rn) == big_num_rn - two_rn);
		BOOST_CHECK(two_rn.difference(big_num_rn) == big_num_rn - two_rn);
		BOOST_CHECK(one_rn.difference(small_num_rn) == one_rn - small_num_rn);
		BOOST_CHECK(small_num_rn.difference(one_rn) == one_rn - small_num_rn);
	}
	BOOST_AUTO_TEST_CASE(equals_method) {
		RealNumber big_num_rn_copy(big_num_rn);
		BOOST_CHECK(two_rn.equals(two_rn));
		BOOST_CHECK(big_num_rn.equals(big_num_rn_copy));
		BOOST_CHECK(big_num_rn_copy.equals(big_num_rn));
		BOOST_CHECK(!zero_rn.equals(small_num_rn));
		BOOST_CHECK(!very_small_num_rn.equals(zero_rn));
	}
	BOOST_AUTO_TEST_CASE(assign_value) {
		RealNumber tmp;
		tmp.assign_value(one_str);
		BOOST_CHECK(one_rn == tmp);
		tmp.assign_value(big_num_str);
		BOOST_CHECK(big_num_rn == tmp);
		tmp.assign_value(very_small_num_str);
		BOOST_CHECK(very_small_num_rn == tmp);
	}
BOOST_AUTO_TEST_SUITE_END()
