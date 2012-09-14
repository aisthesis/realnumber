/**
 * real_number.cpp
 * RealNumber class implementation
 *
 * Created by Marshall Farrier on 5/26/12.
 * Copyright (c) 2012 Marshall Farrier
 * license http://opensource.org/licenses/gpl-license.php GNU Public License
 */

#include <iostream>
#include <string>
#include <stdexcept>
#include <sstream>

#include "real_number.h"
#include "array_arithmetic.h"

// Constructors and destructor
RealNumber::RealNumber() { init(); }

RealNumber::RealNumber(const std::string& str) {
    init();
    assign_value(str);
}

RealNumber::RealNumber(const RealNumber& num) {
    value = new unsigned char[ARRAY_SIZE];
    for (unsigned int i = 0; i < ARRAY_SIZE; ++i) {
        value[i] = num.value[i];
    }
}

RealNumber::~RealNumber() {
    delete[] value;
}

// public methods
std::string RealNumber::to_decimal_string() const {
    std::stringstream result;
    result << get_integer_part() << '.';
    // make a copy of value because we will be changing it
    unsigned char* fractional_part = new unsigned char[PRECISION];
    std::size_t i;
    for (i = 0; i < PRECISION; ++i) { fractional_part[i] = value[i + INTEGER_DIGITS]; }
		for (i = 0; i < DECIMAL_PRECISION; ++i) { 
			result << (int)(array_arithmetic::convert::helpers::times_ten(fractional_part, PRECISION)); 
		}    
    delete[] fractional_part;
    return result.str();
}

RealNumber& RealNumber::operator=(const RealNumber& num) {
    if (this == &num) { return *this; }
    for (std::size_t i = 0; i < ARRAY_SIZE; ++i) {
        this->value[i] = num.value[i];
    }
    return *this;
}

bool RealNumber::operator>(const RealNumber& num_to_compare) const {
    for (std::size_t i = 0; i < ARRAY_SIZE; ++i) {
        if (this->value[i] > num_to_compare.value[i]) { return true; }
        else if (this->value[i] < num_to_compare.value[i]) { return false; }
    }
    return false;
}

bool RealNumber::operator>=(const RealNumber& num_to_compare) const {
    for (std::size_t i = 0; i < ARRAY_SIZE; ++i) {
        if (this->value[i] > num_to_compare.value[i]) { return true; }
        else if (this->value[i] < num_to_compare.value[i]) { return false; }
    }
    return true;
}

bool RealNumber::operator<(const RealNumber& num_to_compare) const {
    for (std::size_t i = 0; i < ARRAY_SIZE; ++i) {
        if (this->value[i] < num_to_compare.value[i]) { return true; }
        else if (this->value[i] > num_to_compare.value[i]) { return false; }
    }
    return false;
}

bool RealNumber::operator<=(const RealNumber& num_to_compare) const {
    for (std::size_t i = 0; i < ARRAY_SIZE; ++i) {
        if (this->value[i] < num_to_compare.value[i]) { return true; }
        else if (this->value[i] > num_to_compare.value[i]) { return false; }
    }
    return true;
}

bool RealNumber::operator!=(const RealNumber& num_to_compare) const {
    for (std::size_t i = 0; i < ARRAY_SIZE; ++i) {
        if (this->value[i] != num_to_compare.value[i]) { return true; }
    }
    return false;
}

bool RealNumber::operator==(const RealNumber& num_to_compare) const {
    return !(*this != num_to_compare);
}

bool RealNumber::equals(const RealNumber& num_to_compare) const {
	// get the real number against which to measure difference
	std::size_t total_bits = ARRAY_SIZE * 8;
	std::size_t first_bit_to_set = total_bits - BITS_FOR_EQUALITY;
	RealNumber max_diff;
	for (std::size_t i = first_bit_to_set; i < total_bits; ++i) {
		array_arithmetic::bits::set(max_diff.value, i);
	}
	return this->difference(num_to_compare) <= max_diff;
}

RealNumber RealNumber::operator+(const RealNumber& num_to_add) const {
    RealNumber result = RealNumber();
    int carry = 0, tmp_sum = 0;
    for (int i = ARRAY_SIZE - 1; i >= 0; --i) {
        tmp_sum = carry + this->value[i] + num_to_add.value[i];
        result.value[i] = tmp_sum % array_arithmetic::RADIX;
        carry = tmp_sum / array_arithmetic::RADIX;
    }
    return result;
}

RealNumber RealNumber::operator-(const RealNumber& num_to_subtract) const {
    RealNumber result = RealNumber();
    array_arithmetic::subtract(this->value, num_to_subtract.value, result.value, ARRAY_SIZE);
    return result;
}

RealNumber RealNumber::operator*(const RealNumber& multiplier) const {
	unsigned char* result_arr = new unsigned char[2 * ARRAY_SIZE + 1];
	RealNumber result;
	array_arithmetic::school_multiply(this->value, multiplier.value, result_arr, ARRAY_SIZE,
			ARRAY_SIZE);
	std::size_t offset = INTEGER_DIGITS + 1;
	for (std::size_t i = 0; i < ARRAY_SIZE; ++i) {
		result.value[i] = result_arr[i + offset];
	}
	delete[] result_arr;
	return result;
}

RealNumber RealNumber::operator/(const RealNumber& divisor) const {
	RealNumber result;
	array_arithmetic::divide(this->value, divisor.value, result.value, ARRAY_SIZE, INTEGER_DIGITS);
	return result;
}

RealNumber RealNumber::difference(const RealNumber &num_to_subtract) const {
    if (*this >= num_to_subtract) { return *this - num_to_subtract; }
    return num_to_subtract - *this;
}
void RealNumber::assign_value(const std::string& str) {
    std::size_t dot_position = str.find('.');
		std::size_t i;
    int integer_part = 0;
    if (dot_position > MAX_DECIMAL_INTEGER_DIGITS) {
        throw std::invalid_argument("Too many digits before decimal!");
    }
    for (i = 0; i < dot_position; ++i) {
        if (0 <= str[i] - '0' && str[i] - '0' <= 9) {
            integer_part = 10 * integer_part + (str[i] - '0');
        }
        else {
            throw std::invalid_argument("Input can contain only decimal digits!");
        }   
    }
    set_integer_part(integer_part);
		std::size_t fractional_digits_len = str.size() - dot_position - 1;
    unsigned char* decimal_digits = array_arithmetic::convert::helpers::to_digit_array(str.substr(dot_position + 1));
    for (i = INTEGER_DIGITS; i < ARRAY_SIZE; ++i) {
        value[i] = array_arithmetic::convert::helpers::times_radix(decimal_digits, fractional_digits_len);
    }
    delete[] decimal_digits;
}

// private methods
void RealNumber::init() {
    value = new unsigned char[ARRAY_SIZE];
    for (std::size_t i = 0; i < ARRAY_SIZE; ++i) {
        value[i] = 0;
    }
}

void RealNumber::set_integer_part(const int& integer_part) {
    unsigned int tmp = integer_part;
    for (int i = INTEGER_DIGITS - 1; i >= 0; --i) {
        value[i] = tmp % array_arithmetic::RADIX;
        tmp /= array_arithmetic::RADIX;
    }
}

unsigned int RealNumber::get_integer_part() const {
    unsigned int result = 0;
    for (std::size_t i = 0; i < INTEGER_DIGITS; ++i) {
        result = result * array_arithmetic::RADIX + value[i];
    }
    return result;
}
