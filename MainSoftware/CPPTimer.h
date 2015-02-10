#ifndef _CPPTIMER_H_
#define _CPPTIMER_H_

#include <QObject>
#include <iostream>
#include <time.h>

class CPPTimer : public QObject
{
	Q_OBJECT

public:
	CPPTimer();
	~CPPTimer();

public:
	void start();
	void stop();
	void reset();
	bool isRunning();
	unsigned long getTime();
	bool isOver(unsigned long milliSeconds);

signals:
	void triggered();

private:
	bool resetted;
	bool running;
	unsigned beg;
	unsigned end;
};

#endif
