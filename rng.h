

#pragma once

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <ctime>

using namespace std;

/*
 * Function: randomInteger
 * Usage: int n = randomInteger(low, high);
 * ----------------------------------------
 * Returns a random integer in the range low to
 * high, inclusive.
 */
int randInteger(int low, int high) {
    time_t timer;
	
	srand(int(low + high + time(&timer)));
	rand();
        
    double d = rand() / (double(RAND_MAX) + 1);
    double s = d * (double(high) - low + 1);
    return int(floor(low + s));
}

double randDouble(double low, double high) {
	time_t timer;
	
	srand(low + high + time(&timer));
	rand();
        
    double d = rand() / (double(RAND_MAX) + 1);
    double s = d * (high - low + 1.0);
    return (low + s);
}
