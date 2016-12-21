#pragma once
#ifndef TIMESTAMP_TO_STANDARD_H_INCLUDED
# define TIMESTAMP_TO_STANDARD_H_INCLUDED

#include <iostream>
#include <stdio.h>
#include <time.h>

using namespace std;

typedef struct times
{
	int Year;
	int Mon;
	int Day;
	int Hour;
	int Min;
	int Second;
}Times;

Times stamp_to_standard(int stampTime);

#endif 