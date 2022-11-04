/**
 * Intel Library for Video Super Resolution
 *
 * Copyright (c) 2022 Intel Corporation
 * All rights reserved.
 */
#pragma once
#include <immintrin.h>

inline void load3x3_ps(float *img, unsigned int width, unsigned int height, unsigned int stride, __m256 *out_8neighbors_ps, __m256 *out_center_ps)
{
    __m128i mask_3pixels = _mm_setr_epi32(-1, -1, -1, 0);
    int index = (height - 1) * stride + (width - 1);
    // load 3x3 grid for lr image, including center pixel plus 8 neighbors
    __m128 row1_f = _mm_maskload_ps(img + index, mask_3pixels);
    index += stride;
    __m128 row2_f = _mm_maskload_ps(img + index, mask_3pixels);
    index += stride;
    __m128 row3_f = _mm_maskload_ps(img + index, mask_3pixels);

    *out_center_ps = _mm256_broadcastss_ps(_mm_insert_ps(row2_f, row2_f, 0x40));
    // load 8 neighbors (32bit floats) into 256 reg from lr image
    __m128 rowlo_f = _mm_insert_ps(row1_f, row2_f, 0x30);
    __m128 rowhi_f = _mm_insert_ps(row3_f, row2_f, 0xB0);
    *out_8neighbors_ps = _mm256_insertf128_ps(_mm256_castps128_ps256(rowlo_f), rowhi_f, 1);
}

inline __m256i compare3x3_ps(__m256 a, __m256 b, __m256i highbit_epi32);
inline int sumitup_256_epi32(__m256i acc);
inline float sumitup_ps_256(__m256 acc);

int inline CTRandomness_AVX256_32f(float *inYUpscaled32f, int cols, int r, int c, int pix);
float inline DotProdPatch_AVX256_32f(const float *buf, const float *filter);