#ifndef TIMER_H
#define TIMER_H

#include<iostream>

#ifdef _MSC_VER
#include<Windows.h>
#elif __GNUC__
#include <sys/time.h>
typedef long long int64_t ;
#endif

using namespace std;

class Timer{
public:
	Timer();
	void start();                       //start to count time
	void end();                         //stop counting time
	int64_t getRunningTime_MS();        //return running time (ms)
	void showRunningTime_MS();
private:
	timeval tv;
	int64_t startTime_;
	int64_t endTime_;
	int64_t runningTime_;
	int64_t freq_Ms;
};

#endif
