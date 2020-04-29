//
// Created by Alexander Dovzhikov on 13/4/20.
//

#include "number_utils.h"

namespace xlc::number {

    // next power of 2
    std::size_t npo2(std::size_t x)
    {
        x = x - 1;
        x |= x >> 1u;
        x |= x >> 2u;
        x |= x >> 4u;
        x |= x >> 8u;
        x |= x >> 16u;
        x |= x >> 32u;

        return x + 1;
    }

  }

