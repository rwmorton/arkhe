#ifndef __ARKHE_TIMER_H__
#define __ARKHE_TIMER_H__

#ifdef WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif //WIN32

namespace arkhe
{

namespace foundation
{

class Timer
{
public:
	//enums
	enum Unit
	{
		SECONDS,
		MILLISECONDS,
		MICROSECONDS
	}; //enum Unit
	//constructor
	Timer();
	//destructor
	~Timer() {}
	//methods
	void start();
	void stop();
	double elapsed(Unit unit=Timer::MICROSECONDS);
	double delta(Unit unit=Timer::MICROSECONDS);
private:
	//variables
	double m_startTime,m_endTime; //in micro-seconds
	double m_prevTime; //in micro-seconds
	bool m_stopped;
	//system specific variables
#ifdef WIN32
	LARGE_INTEGER m_frequency,m_startCount,m_endCount;
#else
	timeval m_startCount,m_endCount;
#endif //WIN32
protected:
	//
}; //class Timer

} //namespace foundation

} //namespace arkhe

#endif //__ARKHE_TIMER_H__
