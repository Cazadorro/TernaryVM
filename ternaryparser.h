//
// Created by Shae Bolt on 8/13/2017.
//

#ifndef TERNARYVM_TERNARYPARSER_H
#define TERNARYVM_TERNARYPARSER_H

#include <exception>

typedef unsigned long long ULL;

// Delcare the litparser
template<ULL Sum, ULL num_val, char... Chars> struct litparser;

// Specialize on the case where there's at least one character left:
template<ULL Sum, ULL num_val,char Head, char... Rest>
struct litparser<Sum, num_val, Head, Rest...> {
// parse a digit. recurse with new sum and ramaining digits
    static_assert(Sum < 6);
    static const ULL value = litparser<
            (Head < '0' || Head >'2') ? throw std::exception() :
            Sum+1,
            Head != '2' ? num_val*4 + (Head - '0') : num_val*4 + 3,
            Rest...>::value;
};

// When 'Rest' finally is empty, we reach this terminating case
template<ULL Sum, ULL num_val>  struct litparser<Sum, num_val> {
    static const ULL value = num_val;
    //static_assert(Sum == 6);
};

#endif //TERNARYVM_TERNARYPARSER_H
