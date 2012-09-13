/**
 * Executable using real_number.h and convergent_series.h
 * to find pi to a high degree of accuracy
 *
 * Created by Marshall Farrier on 5/26/12.
 * Copyright (c) 2012 Marshall Farrier
 * license http://opensource.org/licenses/gpl-license.php GNU Public License
 */

#include <iostream>
#include <string>
#include <stdexcept>
#include <exception>

#include "real_number.h"
#include "convergent_series.h"
//#include "karatsuba.h"

using namespace std;

int main(int argc, const char * argv[])
{
	//karatsuba::say_hello();

	string str = "1.0";
	RealNumber x(str);
	cout << "x is " << x.to_decimal_string() << endl;
	str = "300000000.0";
	RealNumber y(str);
	cout << "y is " << y.to_decimal_string() << endl;

	try {
		RealNumber("10000000000.0");
	} 
	catch (invalid_argument& ia) {
		cout << ia.what() << endl;
	}
	catch (exception& e) {
		cout << "this exception is expected" << endl;
	}
	RealNumber z = x / y;
	cout << "z is " << z.to_decimal_string() << endl;
	cout << "x.difference(y) is " << x.difference(y).to_decimal_string() << endl;
	
	RealNumber num("2.0");
	RealNumber guess("1.4");
	int iterations = 10;
	
	RealNumber sqrt_two = ConvergentSeries::babylonian_sqrt(num, guess, iterations);
	
	cout << "Square root of 2 is " << sqrt_two.to_decimal_string() << endl;
	cout << "or: " << ConvergentSeries::babylonian_sqrt(RealNumber("2.0"), RealNumber("1.4"), 
																											iterations).to_decimal_string() << endl;
	
	RealNumber pi = ConvergentSeries::gauss_legendre_pi(6, 10, sqrt_two);
	cout << "Pi is\n" << pi.to_decimal_string() << endl;
}

