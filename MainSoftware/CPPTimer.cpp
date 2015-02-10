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
		}
		else
		{
			beg -= end - (unsigned long)clock();
		}
		running = true;
		resetted = false;
	}
}


