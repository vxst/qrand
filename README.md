# qrand: High Quality Quick Random Number Generator

The qrand project is a high quality quick random number generator with
following features:

* High quality: Passes all tests in TestU01 BigCrush suite
* Quick: The overhead for random number generation is at the same order
  of magnitude as the time for a single function call. Actually, it's
  THE fastest random number generator passes TestU01 BigCrush suite
* Reproducible: The random number generator is deterministic and
  reproducible
* Long state: The state of the random number generator is 128 bytes, which
  is also 2 cache lines on most modern CPUs, making it as fast as possible
  but also retaining a long enough period for most applications

## Usage

The qrand library is compatible with STL random number generators. It can
be used as a drop-in replacement for STL random number generators. It also
includes a qrand_float class which generates random floating point numbers.

```c++
#include <random>
#include <iostream>
#include "include/qrand.h"

int main(){
    std::uniform_int_distribution<int> dist(0, 100);
    std::uniform_real_distribution<double> distf(0.0, 1.0);

    qrand rng;
    for(int i = 0; i < 10; i++){
        std::cout << dist(rng) << " " << distf(rng) << std::endl;
    }
}
```

To use the qrand library, simply include the header file `qrand.h`. The
qrand uses VAES or AES-NI instructions to accelerate the random number
generation. So it need to be compiled with `-mavx512f`, with either `-maes`
or `-mvaes` option. The easiest way to compile is to use the provided
`-march=native` option on a modern x86 CPU.

## Algorithm

The qrand library uses a AES round with 4 128 bit key to generate 512 bits
random numbers. The counter is incremented by a balanced random delta each
round.

```text
! All variable is 512 bit
! += is 64 bit addition

const add, add0, add1

ct += add
buf = AES_ROUND(ct, key)
key += add0
buf = AES_ROUND(buf, key)
key += add1
buf = AES_ROUND(buf, key)
key = buf
```

<!--
 Copyright (C) 2023 Chunqing Shan 
 
 qrand is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 qrand is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public License
 along with qrand. If not, see <http://www.gnu.org/licenses/>.
-->
