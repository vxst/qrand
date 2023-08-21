// Copyright (C) 2023 Chunqing Shan
// 
// qrand is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// qrand is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with qrand. If not, see <http://www.gnu.org/licenses/>.

#pragma once
#include <cstdint>
#include <cstdio>
#include <immintrin.h>

static inline void _qrand_vaes_round(__m512i& dt, __m512i key) __attribute__ ((always_inline));
static inline void _qrand_vaes_round(__m512i& dt, __m512i key)
{
#if defined(__VAES__) && defined(__AVX512F__)
    dt = _mm512_aesenc_epi128(dt, key);
#else
    for(int i = 0; i < 4; i++)
        *(((__m128i*)&dt) + i) = _mm_aesenc_si128(*(((__m128i*)&dt) + i), *(((__m128i*)&key) + i));
#endif
}

static inline void _qrand_vadd_epi64(__m512i& v, std::uint64_t a, std::uint64_t b, std::uint64_t c, std::uint64_t d,
                                                 std::uint64_t e, std::uint64_t f, std::uint64_t g, std::uint64_t h)
                                    __attribute__ ((always_inline));
static inline void _qrand_vadd_epi64(__m512i& v, std::uint64_t a, std::uint64_t b, std::uint64_t c, std::uint64_t d,
                                                 std::uint64_t e, std::uint64_t f, std::uint64_t g, std::uint64_t h)
{
    __m512i add;
#if defined(__AVX512F__)
    add = _mm512_set_epi64(a, b, c, d, e, f, g, h);
    v = _mm512_add_epi64(v, add);
#elif defined(__AVX2__)
    *(((__m256i*)&add) + 0) = _mm256_set_epi64x(e, f, g, h);
    *(((__m256i*)&v) + 0) = _mm256_add_epi64(*(((__m256i*)&v) + 0), *(((__m256i*)&add) + 0));
    *(((__m256i*)&add) + 1) = _mm256_set_epi64x(a, b, c, d);
    *(((__m256i*)&v) + 1) = _mm256_add_epi64(*(((__m256i*)&v) + 1), *(((__m256i*)&add) + 1));
#else
    *(((__m128i*)&add) + 0) = _mm_set_epi64x(g, h);
    *(((__m128i*)&v) + 0) = _mm_add_epi64(*(((__m128i*)&v) + 0), *(((__m128i*)&add) + 0));
    *(((__m128i*)&add) + 1) = _mm_set_epi64x(e, f);
    *(((__m128i*)&v) + 1) = _mm_add_epi64(*(((__m128i*)&v) + 1), *(((__m128i*)&add) + 1));
    *(((__m128i*)&add) + 2) = _mm_set_epi64x(c, d);
    *(((__m128i*)&v) + 2) = _mm_add_epi64(*(((__m128i*)&v) + 2), *(((__m128i*)&add) + 2));
    *(((__m128i*)&add) + 3) = _mm_set_epi64x(a, b);
    *(((__m128i*)&v) + 3) = _mm_add_epi64(*(((__m128i*)&v) + 3), *(((__m128i*)&add) + 3));
#endif
}

class qrand{
protected:
    typedef std::uint64_t v512 __attribute__ ((vector_size (64)));
    v512 keybuf, ct;
    std::uint_fast8_t buf_i;
    void fill() __attribute__ ((optimize("vect-cost-model=unlimited")))
    {
        __m512i dt;

        _qrand_vadd_epi64((__m512i&)ct,
                          0x45d36387550f4d99, 0x5b1c11bd09ee7529, 0x5b4a5a66c29deab1, 0xbb0a9761e05b3ec1,
                          0xa4cb9b1aab19e271, 0x6d1c8aab307ec70f, 0xf0e16e85425f1e95, 0x5f8486d6745a89e9);
        
        dt = (__m512i)ct;
        _qrand_vaes_round(dt, (__m512i)keybuf);
        _qrand_vadd_epi64((__m512i&)keybuf,
                          0xa8b53596c9a68e2b, 0xcae2958d699a4cec, 0xb45a6c1bb9b0b456, 0xdc154b5c2e6a2d9a,
                          0xe29a16a7ceb0bf04, 0x23756c3c1ad30fd1, 0x0dba4b531e33ad0b, 0xb1596c8ee3c475a4);
        _qrand_vaes_round(dt, (__m512i)keybuf);
        _qrand_vadd_epi64((__m512i&)keybuf,
                          0x4976b84e93a6ad70, 0x89e6756894aec8ad, 0xb165ad2cb31a91b6, 0xc6d40aed4af89273,
                          0x38973de0b03b7a23, 0xd56499172d9a4f54, 0x7d88cce4361d4aea, 0xcc95b93865b1f829);
        _qrand_vaes_round(dt, (__m512i)keybuf);

        keybuf = (v512)dt;

        buf_i = 0;
    }
public:
    using result_type = std::uint32_t;
    static constexpr result_type min() { return 0; }
    static constexpr result_type max() { return UINT32_MAX; }

    qrand(){
        FILE* urandom = fopen("/dev/urandom", "rb");
        fread(&keybuf, sizeof(keybuf), 1, urandom);
        fclose(urandom);
        for(int i = 0; i < 8; i++)
            ct[i] = 0;
        fill();
    }

    qrand(std::uint64_t seed){
        for(int i = 0; i < 8; i++){
            ct[i] = 0;
            keybuf[i] = seed;
        }
        fill();
    }

    result_type operator()(){
        if(buf_i == 16) fill();
        return ((std::uint32_t*)&keybuf)[buf_i++];
    }
};
