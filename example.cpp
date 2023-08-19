#include <random>
#include <iostream>
#include "include/qrand.h"

int main(){
    std::uniform_int_distribution<int> dist(0, 1000);
    std::uniform_real_distribution<double> distf(0.0, 1.0);

    std::cout << "Random" << std::endl;
    qrand rng;
    for(int i = 0; i < 4; i++)
        std::cout << dist(rng) << " " << distf(rng) << std::endl;

    std::cout << "Seeded" << std::endl;
    qrand rng_seeded(1);
    for(int i = 0; i < 2; i++)
        std::cout << dist(rng_seeded) << " " << distf(rng_seeded) << std::endl;
    qrand rng_seeded2(12345);
    for(int i = 0; i < 2; i++)
        std::cout << dist(rng_seeded2) << " " << distf(rng_seeded2) << std::endl;
}