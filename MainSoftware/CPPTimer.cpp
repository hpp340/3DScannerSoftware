#include "CPPTimer.h"


CPPTimer::CPPTimer()
{
	resetted = true;
	running = false;
	beg = 0;
	end = 0;
}

CPPTimer::~CPPTimer()
{
}

void CPPTimer::start()
{
	if (! running)
	{
		if (resetted)
		{
			beg = (unsigned long)clock();
			int currTime = (unsigned long)clock();
			int triggerTime = (unsigned long)clock();
			while (true)
			{
				if (triggerTime - currTime >= 250)
				{
					emit triggered();
					currTime = (unsigned long)clock();
				}
				triggerTime = (unsigned long)clock();
			}
		}
		else
		{
			beg -= end - (unsigned long)clock();
		}
		running = true;
		resetted = false;
	}
}

void CPPTimer::stop()
{
	if (running)
	{
		end = (unsigned long)clock();
		running = false;
	}
}

void CPPTimer::reset()
{
	bool wereRunning = running;
	if (wereRunning)
	{
		stop();
	}
	resetted = true;
	beg = 0;
	end = 0;
	if (wereRunning)
	{
		start();
	}
}

bool CPPTimer::isRunning()
{
	return running;
}

unsigned long CPPTimer::getTime()
{
	if (running)
	{
		return ((unsigned long)clock() - beg);
	}
	else
	{
		return end - beg;
	}
}

bool CPPTimer::isOver(unsigned long seconds)
{
	return seconds >= getTime();
}
