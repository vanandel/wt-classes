/*
 * wt-classes, utility classes used by Wt applications
 * Copyright (C) 2011 Boris Nagaev
 *
 * See the LICENSE file for terms of use.
 */

#include "config.hpp"
#include <climits>
#include <cstdlib>
#include <fstream>

#ifdef WC_HAVE_WRANDOM
#include <Wt/WRandom>
#else
#include <cstdlib>
#include <ctime>
#endif

#include "rand.hpp"
#include "util.hpp"

namespace Wt {

namespace Wc {

#ifndef WC_HAVE_WRANDOM
struct Srander {
    Srander() {
        std::srand(time(NULL));
    }
} srander;
#endif

const unsigned int UINT_MIN = 0;

unsigned int rr() {
#ifdef WC_HAVE_WRANDOM
    return WRandom::get();
#else
    // TODO use boost random if available
    return rand();
#endif
}

unsigned int rr(unsigned int stop) {
#ifdef WC_HAVE_WRANDOM
    return WRandom::get() % stop;
#else
    return rand() / (RAND_MAX / stop + 1);
#endif
}

unsigned int rr(unsigned int start, unsigned int stop) {
    return start + rr(stop - start);
}

unsigned int rr(unsigned int start, unsigned int stop, unsigned int step) {
    return start + (rr((stop - start) / step)) * step;
}

double drr(double start, double stop) {
    double stop_start = stop - start;
    const double max_min = UINT_MAX - UINT_MIN;
    return start + double(rr()) / max_min * stop_start;
}

ptrdiff_t rand_for_shuffle(ptrdiff_t i) {
    return rr(static_cast<unsigned int>(i));
}

std::string rand_string(int length) {
#ifdef WC_HAVE_WRANDOM
    return WRandom::generateId(length);
#else
    const std::string abc = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                            "abcdefghijklmnopqrstuvwxyz"
                            "0123456789";
    std::string result;
    result.reserve(length);
    for (int i = 0; i < length; i++) {
        result += abc[rr(abc.length())];
    }
    return result;
#endif
}

std::string good_password() {
    std::string result;
    std::string filename = unique_filename();
    std::string cmd = "pwqgen > " + filename;
    system(cmd.c_str());
    std::ifstream stream(filename.c_str());
    if (stream.good()) {
        stream >> result;
    }
    stream.close();
    remove(filename.c_str());
    if (result.size() < 4) {
        result = rand_string(16);
        result[rr(0, 15)] = '$';
        result[rr(0, 15)] = '%';
        result[rr(0, 15)] = '_';
    }
    return result;
}

}

}

