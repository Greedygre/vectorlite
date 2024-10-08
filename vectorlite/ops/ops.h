#pragma once

#include <string_view>

#include "hwy/base.h"

// This file implements vector operations using google's Highway SIMD library.
// Based on the benchmark on my PC(i5-12600KF with AVX2 support),
// InnerProductDistance is 1.5x-3x faster than HNSWLIB's SIMD implementation
// when dealing with vectors with 256 elements or more. The performance gain is
// mainly due to Highway can leverage fused Multiply-Add instructions of AVX2
// while HNSWLIB can't(HNSWLIB uses Multiply-Add for AVX512 though). Due to
// using dynamic dispatch, the performance gain is not as good when dealing with
// vectors with less than 256 elements. Because the overhead of dynamic dispatch
// is not negligible.
namespace vectorlite {
namespace ops {

using DistanceFunc = float (*)(const float*, const float*, size_t);

// v1 and v2 MUST not be nullptr but can point to the same array.
HWY_DLLEXPORT float InnerProduct(const float* v1, const float* v2,
                                 size_t num_elements);
HWY_DLLEXPORT float InnerProductDistance(const float* v1, const float* v2,
                                         size_t num_elements);

// v1 and v2 MUST not be nullptr but can point to the same array.
HWY_DLLEXPORT float L2DistanceSquared(const float* v1, const float* v2,
                                      size_t num_elements);

// Nornalize the input vector in place.
HWY_DLLEXPORT void Normalize(float* HWY_RESTRICT inout, size_t num_elements);

// Normalize the input vector in place. Implemented using non-SIMD code for
// testing and benchmarking purposes.
HWY_DLLEXPORT void Normalize_Scalar(float* HWY_RESTRICT inout,
                                    size_t num_elements);

// Detect best available SIMD target to ensure future dynamic dispatch avoids
// the overhead of CPU detection. HWY_DLLEXPORT std::string_view DetectTarget();

}  // namespace distance
}  // namespace vectorlite
