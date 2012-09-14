/**
 * real_number.h
 * RealNumber
 * Created by Marshall Farrier on 5/26/12.
 * Copyright (c) 2012 Marshall Farrier
 * license http://opensource.org/licenses/gpl-license.php GNU Public License
 
 * The integral part supported in this class is up to
 * 999,999,999 (9 decimal digits)
 * It is designed to support whatever precision is set for
 * the fractional part, which is converted to radix 256
 * (using an array of type unsigned char) for fast and efficient
 * calcuations, while allowing the use of unsigned int to 
 * take care of the temporary overflow during mathematical operations.
 
 * Real numbers on this implementation are unsigned but do support subtraction.
 * If subtraction is used, the client is responsible for making sure that
 * for a - b, a >= b. Behavior for a < b is undefined.
 
 * The current (5/26/2012) implementation doesn't support
 * truly arbitrary precision (limited only by hardware): Multiplication 
 * requires that we sum PRECISION values of up to 255, plus a carry. That means
 * that PRECISION * 255 must be < 4294967295, which will be the case if 
 * PRECISION * 2 ^ 8 <= 2 ^ 32, i.e.
 * PRECISION  <= 2 ^ 24 =  16,777,216
 * Note that using a long doesn't change anything fundamentally, as long is not
 * actually guaranteed to be more than 32 bits:
 * http://www.cplusplus.com/doc/tutorial/variables/
 * Multiplication by the school method as implemented here will also require
 * PRECISION ^ 2 bytes of storage, which should be fine when PRECISION < 1000, but
 * even at 10,000 we are then using 100MB of memory.
 */

#ifndef REAL_NUMBER_H
#define REAL_NUMBER_H

#include <string>

class RealNumber {
public:
	// determines digits of fractional part
	static const unsigned int PRECISION = 50;
	// 2.40823996 ~ log10(256)
	static const unsigned int DECIMAL_PRECISION = PRECISION * 2.40823996;
	static const unsigned int MAX_DECIMAL_INTEGER_DIGITS = 9;
	// first 4 digits in value array represent the integer part
	static const unsigned int INTEGER_DIGITS = 4;
	static const unsigned int ARRAY_SIZE = INTEGER_DIGITS + PRECISION;
	static const unsigned int ARRAY_BITS = ARRAY_SIZE * 8;
	static const unsigned int BITS_FOR_EQUALITY = 4;

private:
	// array will be of size INTEGER_DIGITS + PRECISION
	unsigned char* value;

	// MEMBER PRIVATE HELPER METHODS
	void init();
	// set integer places in value using an int input
	void set_integer_part(const int& integer_part);
	// convert the integer part of value array to an int
	unsigned int get_integer_part() const;
    
public:
	RealNumber();
	RealNumber(const std::string& str);
	// copy constructor
	RealNumber(const RealNumber& num);
	~RealNumber();
	// output string represents value in decimal format
	std::string to_decimal_string() const;
	RealNumber& operator=(const RealNumber& num);
	bool operator>(const RealNumber& num_to_compare) const;
	bool operator>=(const RealNumber& num_to_compare) const;
	bool operator<(const RealNumber& num_to_compare) const;
	bool operator<=(const RealNumber& num_to_compare) const;
	bool operator!=(const RealNumber& num_to_compare) const;
	/**
	 * True equality, which for real number calculations is inferior
	 * to equals(), which determines appropximate equality
	 */
	bool operator==(const RealNumber& num_to_compare) const;
	/**
	 * equality to within last BITS_FOR_EQUALITY
	 * i.e., difference between 2 numbers is <= the number defined by setting
	 * the last BITS_FOR_EQUALITY bits to 1 and all remaining bits to 0
	 */
	bool equals(const RealNumber& num_to_compare) const;
	RealNumber operator+(const RealNumber& num_to_add) const;
	// a - b is undefined if b > a
	RealNumber operator-(const RealNumber& num_to_subtract) const;
	RealNumber operator*(const RealNumber& multiplier) const;
	RealNumber operator/(const RealNumber& divisor) const;
	/**
	 * returns absolute value of difference and works regardless of
	 * which number is larger
	 */
	RealNumber difference(const RealNumber& num_to_subtract) const;
	/**
	 * assigns calling object the value specified in the decimal string str
	 */
	void assign_value(const std::string& str);
};
#endif
