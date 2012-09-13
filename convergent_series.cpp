//
//  convergent_series.cpp
//  RealNumber
//
//  Created by Marshall Farrier on 5/27/12.
//  Copyright (c) 2012 Marshall Farrier. All rights reserved.
//

#include <iostream>
#include "convergent_series.h"

RealNumber ConvergentSeries::babylonian_sqrt(const RealNumber& num, const RealNumber& guess,
                                            const std::size_t& iterations) {
    RealNumber result = guess;
    RealNumber two("2.0");
    for (std::size_t i = 0; i < iterations; ++i) {
        result = (result + (num / result)) / two;
    }
    return result;
}

RealNumber ConvergentSeries::gauss_legendre_pi(const std::size_t& pi_iterations,
                                               const std::size_t& sqrt_iterations, const RealNumber& sqrt_two) {
    RealNumber a("1.0");
    RealNumber a_next;
    RealNumber b = RealNumber("1.0") / sqrt_two;
    RealNumber b_next;
    RealNumber t("0.25");
    RealNumber t_next;
    RealNumber p("1.0");
    RealNumber p_next;
    RealNumber tmp1, tmp2;
    
    const RealNumber one_half("0.5");
    const RealNumber two("2.0");
    for (std::size_t i = 0; i < pi_iterations; ++i) {
        a_next = (a + b) * one_half;
        b_next = babylonian_sqrt(a * b, a_next, sqrt_iterations);
        tmp1 = a - a_next;
        t_next = t - p * tmp1 * tmp1;
        p_next = two * p;
        a = a_next;
        b = b_next;
        t = t_next;
        p = p_next;
    }
    tmp1 = a + b;
    tmp2 = RealNumber("4.0") * t;
    return tmp1 * tmp1 / tmp2;
}
