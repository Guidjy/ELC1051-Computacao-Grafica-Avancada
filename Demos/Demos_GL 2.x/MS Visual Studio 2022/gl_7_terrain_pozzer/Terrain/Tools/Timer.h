// Timer.h: interface for the Timer class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _TIMER_H
#define _TIMER_H

#include <time.h>


class Timer  
{
	clock_t t1, t2;   
public:
	Timer()
	{
	    t1 = clock();
	}

	float getElapsedTime()
	{
		float tempo;

	    t2 = clock();
		tempo  = (float)(t2 - t1) / CLOCKS_PER_SEC;
		return tempo;
	}

	void resetTimer()
	{
		t1 = clock();
	}

	int getTime()
	{
	    return (int)clock();
	}

	float diffTime(clock_t ta, clock_t tb)
	{
		float tempo;
		tempo  = (float)(tb - ta) / CLOCKS_PER_SEC;
		return tempo;
	}

};

#endif
