#include "SMath.h"
#include <stdio.h>      
#include <stdlib.h>    
#include <time.h>       

SMath::SMath() {}
SMath::~SMath() {}

int SMath::IRandRange(int Min, int Max) {
	return (rand() % (Max-Min)) + Min;
}
float SMath::FRandRange(float Min, float Max) {
	return Min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (Max - Min)));
}