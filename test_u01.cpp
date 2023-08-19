#include <string>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "include/qrand.h"

extern "C" {
    #include <TestU01.h>
}

uint32_t gen32(){
    static qrand rng(123);
    return rng();
}


int main(){
    std::string genName = "qrand";
    unif01_Gen* gen = unif01_CreateExternGenBits((char*) genName.c_str(), gen32);

    swrite_Basic = FALSE;
    bbattery_SmallCrush(gen);
    fflush(stdout);

    bbattery_Crush(gen);
    fflush(stdout);

    bbattery_BigCrush(gen);
    fflush(stdout);

    unif01_DeleteExternGenBits(gen);

    return 0;
}