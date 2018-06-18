#include<iostream>
#include<thread>
#include<random>

#include"as.h"

#include"copit.h"

/// Helper

unsigned short termW, termH;

unsigned char colorFromVal(int val) {
    switch(val) {
        case 0: return copit::black;
        case 1: return copit::red;
        case 2: return copit::green;
        case 3: return copit::blue;
        default:return copit::lightwhite;
    }
}

void test(as::dautm<int>& a, long int millis) {
    for(int frame = 0; frame < 200; frame++) {
        auto start = std::chrono::high_resolution_clock::now();

        int map[20][20];
        a.getInto(&map[0][0], -10, -10, 20, 20);

        std::cout << copit::setcur(1, 1);
        for(int y = 0; y < 20; y++) {
            for(int x = 0; x < 20; x++) {
                int val = map[x][y];
                std::cout << copit::bg(colorFromVal(val));
                std::cout << ' ';
            }
            std::cout << "\n";
        }
        std::cout << std::flush;

        a.step(1);

        auto timePassed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count();
        std::this_thread::sleep_for(std::chrono::milliseconds(std::max(millis - timePassed, 0L)));
        std::cout << copit::setcur(1, 1);
    }
}

/// Game of Life

void gol() {
    as::dautm<int>::rulesets::neighbor::gol rules;
    as::dautm<int> a(&rules);

    a.set(-4, -4, 1);
    a.set(-3, -4, 1);
    a.set(-2, -4, 1);

    a.set(1, 0, 1);
    a.set(2, 1, 1);
    a.set(2, 2, 1);
    a.set(1, 2, 1);
    a.set(0, 2, 1);

    test(a, 100);
}

/// Rock Paper Scissors (variation of reddit.com/54wv60

void rps() {
    as::dautm<int>::rulesets::neighbor::lambda rules([](as::dautm<int>&, int neighbors[3][3]){
        int neighbs[4] = {0, 0, 0, 0};

        for(int nx = 0; nx < 3; nx++) {
            for(int ny = 0; ny < 3; ny++) {
                neighbs[neighbors[nx][ny]]++;
            }
        }

        if(neighbors[1][1] == 0) {
            if(neighbs[1] > neighbs[2] && neighbs[1] > neighbs[3])
                return 1;
            else if(neighbs[2] > neighbs[1] && neighbs[2] > neighbs[3])
                return 2;
            else if(neighbs[3] > neighbs[2] && neighbs[3] > neighbs[1])
                return 3;
            else
                return 0;
        } else if(neighbors[1][1] == 1) {
            if(neighbs[2] >= 3)
                return 2;
            else
                return 1;
        } else if(neighbors[1][1] == 2) {
            if(neighbs[3] >= 3)
                return 3;
            else
                return 2;
        } else if(neighbors[1][1] == 3) {
            if(neighbs[1] >= 3)
                return 1;
            else
                return 3;
        }
        return neighbors[1][1];
    });
    as::dautm<int> a(&rules);

    std::mt19937 rng(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> gen(-17, 3);

    for(int x = -10; x <= 10; x++) {
        for(int y = -10; y <= 10; y++) {
            a.set(x, y, std::max(0, gen(rng)));
        }
    }

    test(a, 100);
}

/// Gravity

void grav() {
    as::dautm<int>::rulesets::lambda rules([](as::dautm<int>&){

    });
    as::dautm<int> a(&rules);
    test(a, 50);
}

///

int main() {
    std::cout << copit::init();
    std::cout << copit::getsize(&termW, &termH);

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

//    gol();
    rps();
    grav();
    std::cout << copit::deinit();
    return 0;
}
