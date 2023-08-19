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

#include <cstdio>
#include <cstdint>

uint16_t balanced_rand16(){
    FILE* fp = fopen("/dev/urandom", "rb");
    uint16_t ret = 0;
    while(__builtin_popcount(ret) != 8)
        fread(&ret, sizeof(uint16_t), 1, fp);
    return ret;
}

uint64_t balanced_rand64(){
    uint64_t ret = 0;
    for(int i = 0; i < 4; i++)
        ret = (ret << 16) | balanced_rand16();
    return ret;
}

int main(){
    for(int i = 0; i < 4; i++)
        printf("0x%016llx, ", balanced_rand64());
    return 0;
}