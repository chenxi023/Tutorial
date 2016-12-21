#include "timestamp_to_standard.h"
#include <iostream>
#include <stdio.h>
#include <time.h>


using namespace std;

Times stamp_to_standard(int stampTime)
{
	time_t tick = (time_t)stampTime;
	struct tm tm;
	char s[100];
	Times standard;

	//tick = time(NULL);
	tm = *localtime(&tick);
	strftime(s, sizeof(s), "%Y-%m-%d %H:%M:%S", &tm);
	printf("%d: %s\n", (int)tick, s);


	standard.Year = atoi(s);
	standard.Mon = atoi(s + 5);
	standard.Day = atoi(s + 8);
	standard.Hour = atoi(s + 11);
	standard.Min = atoi(s + 14);
	standard.Second = atoi(s + 17);

	return standard;
}
