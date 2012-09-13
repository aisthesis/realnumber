//
//  convergent_series.h
//  RealNumber
//
//  Created by Marshall Farrier on 5/27/12.
//  Copyright (c) 2012 Marshall Farrier. All rights reserved.
//

#ifndef CONVERGENT_SERIES_H
#define CONVERGENT_SERIES_H

#include "real_number.h"

namespace ConvergentSeries {
    RealNumber babylonian_sqrt(const RealNumber& num, const RealNumber& guess, 
                               const size_t& iterations);
    // 4 pi iterations with 10 sqrt iterations and sqrt 2 using 10 iterations and a guess of
    // 1.4 gives accuracy to 45 decimal digits
    // 5 pi iterations gives accuracy to 83 decimal digits
    // 6 iterations is accurate to 100+ digits
    RealNumber gauss_legendre_pi(const size_t& pi_iterations, const size_t& sqrt_iterations,
                                 const RealNumber& sqrt_two);
}

#endif
