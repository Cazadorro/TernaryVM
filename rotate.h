//
// Created by Shae Bolt on 8/12/2017.
//

#ifndef TERNARYVM_ROTATE_H
#define TERNARYVM_ROTATE_H

#include <climits>
#include <cassert>

template <typename UIntType>
constexpr UIntType rotl_t (UIntType n, unsigned int c)
{
    const unsigned int mask = (CHAR_BIT*sizeof(n) - 1);  // assumes width is a power of 2.

    // assert ( (c<=mask) &&"rotate by type width or more");
    c &= mask;
    return (n<<c) | (n>>( (-c)&mask ));
}
template <typename UIntType>
constexpr UIntType rotr_t (UIntType n, unsigned int c)
{
    const unsigned int mask = (CHAR_BIT*sizeof(n) - 1);

    // assert ( (c<=mask) &&"rotate by type width or more");
    c &= mask;
    return (n>>c) | (n<<( (-c)&mask ));
}








































#endif //TERNARYVM_ROTATE_H
