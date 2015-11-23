#include<iostream>

#include"Timer.h"

using namespace std;

#ifdef _MSC_VER
	Timer::Timer(){
		startTime_ = 0;
		endTime_ = 0;
		runningTime_ = 0;
		QueryPerformanceFrequency((LARGE_INTEGER*)&freq_Ms);
		freq_Ms /= 1000;
	}
	void Timer::start(){
		QueryPerformanceCounter((LARGE_INTEGER*)&startTime_);
	}
	void Timer::end(){
		QueryPerformanceCounter((LARGE_INTEGER*)&endTime_);
		runningTime_ = endTime_ - startTime_;
	}
	int64_t Timer::getRunningTime_MS(){
		return runningTime_ / freq_Ms;
	}
#elif __GNUC__
	Timer::Timer(){
		startTime_ = 0;
		endTime_ = 0;
		runningTime_ = 0;
	}
	void Timer::start(){
		gettimeofday(&tv, NULL);
		startTime_ = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	}
	void Timer::end(){
		gettimeofday(&tv, NULL);
		endTime_ = tv.tv_sec * 1000 + tv.tv_usec / 1000;
		runningTime_ = endTime_ - startTime_;
	}
	void Timer::showRunningTime_MS(){
		cout << "Running time is " << runningTime_ * 1.0 / freq_Ms << " ms." << endl;
	}
	int64_t Timer::getRunningTime_MS(){
		return runningTime_;
	}
#endif