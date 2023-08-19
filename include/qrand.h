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
#include <cstdlib>
#include <cstdio>
#include <stdexcept>
#include <limits>
#include <immintrin.h>

class qrand{
protected:
    typedef uint64_t v512 __attribute__ ((vector_size (64)));
    v512 keybuf, ct;
    uint_fast8_t buf_i;
    void fill()__attribute__ ((optimize("vect-cost-model=unlimited")))
    {
        __m512i dt, add;

        add = _mm512_set_epi64(0x45d36387550f4d99, 0x5b1c11bd09ee7529, 0x5b4a5a66c29deab1, 0xbb0a9761e05b3ec1,
                               0xa4cb9b1aab19e271, 0x6d1c8aab307ec70f, 0xf0e16e85425f1e95, 0x5f8486d6745a89e9);
        ct = (v512)_mm512_add_epi64((__m512i)ct, add);
#ifdef __VAES__
        dt = _mm512_aesenc_epi128((__m512i)ct, (__m512i)keybuf);
#else
        for(int i = 0; i < 4; i++)
            *(((__m128i*)&dt) + i) = _mm_aesenc_si128(*(((__m128i*)&ct) + i), *(((__m128i*)&keybuf) + i));
#endif
        add = _mm512_set_epi64(0xa8b53596c9a68e2b, 0xcae2958d699a4cec, 0xb45a6c1bb9b0b456, 0xdc154b5c2e6a2d9a,
                               0xe29a16a7ceb0bf04, 0x23756c3c1ad30fd1, 0x0dba4b531e33ad0b, 0xb1596c8ee3c475a4);
        keybuf = (v512)_mm512_add_epi64((__m512i)keybuf, add);
#ifdef __VAES__
        dt = _mm512_aesenc_epi128((__m512i)ct, (__m512i)keybuf);
#else
        for(int i = 0; i < 4; i++)
            *(((__m128i*)&dt) + i) = _mm_aesenc_si128(*(((__m128i*)&ct) + i), *(((__m128i*)&keybuf) + i));
#endif
        add = _mm512_set_epi64(0x4976b84e93a6ad70, 0x89e6756894aec8ad, 0xb165ad2cb31a91b6, 0xc6d40aed4af89273,
                               0x38973de0b03b7a23, 0xd56499172d9a4f54, 0x7d88cce4361d4aea, 0xcc95b93865b1f829);
        keybuf = (v512)_mm512_add_epi64((__m512i)keybuf, add);
#ifdef __VAES__
        dt = _mm512_aesenc_epi128((__m512i)ct, (__m512i)keybuf);
#else
        for(int i = 0; i < 4; i++)
            *(((__m128i*)&dt) + i) = _mm_aesenc_si128(*(((__m128i*)&ct) + i), *(((__m128i*)&keybuf) + i));
#endif
        keybuf = (v512)dt;

        buf_i = 0;
    }
public:
    using result_type = uint32_t;
    static constexpr result_type min() { return std::numeric_limits<result_type>::min(); }
    static constexpr result_type max() { return std::numeric_limits<result_type>::max(); }

    qrand(){
        arc4random_buf(&keybuf, sizeof(keybuf));
        for(int i = 0; i < 8; i++)
            ct[i] = 0;
        fill();
    }

    qrand(uint64_t seed){
        for(int i = 0; i < 8; i++){
            ct[i] = seed;
            keybuf[i] = 0;
        }
        fill();
        for(int i = 0; i < 8; i++)
            ct[i] = 0;
        fill();
    }

    result_type operator()(){
        if(buf_i == 16) fill();
        return ((uint32_t*)&keybuf)[buf_i++];
    }
};
