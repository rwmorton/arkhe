//arkhe local includes (foundation)
#include "ArkheTimer.h"
#include "ArkheException.h"

namespace arkhe
{

namespace foundation
{

Timer::Timer()
{
	m_stopped = true;
	m_startTime = m_endTime = m_prevTime = 0.0;
#ifdef WIN32
	QueryPerformanceFrequency(&m_frequency);
	m_startCount.QuadPart = 0;
	m_endCount.QuadPart = 0;
#else
	m_startCount.tv_sec = m_startCount.tv_usec = 0;
	m_endCount.tv_sec = m_endCount.tv_usec = 0;
#endif //WIN32
}

void Timer::start()
{
	m_stopped = false;
#ifdef WIN32
	QueryPerformanceCounter(&m_startCount);
#else
	gettimeofday(&m_startCount,NULL);
#endif //WIN32
}

void Timer::stop()
{
	m_stopped = true;
#ifdef WIN32
	QueryPerformanceCounter(&m_endCount);
#else
	gettimeofday(&m_endCount,NULL);
#endif //WIN32
}

//get elapsed time. defaults to microseconds
double Timer::elapsed(Timer::Unit unit)
{
#ifdef WIN32
	if(!m_stopped)
	{
		QueryPerformanceCounter(&m_endCount);
	}
	m_startTime = m_startCount.QuadPart * (1000000.0 / m_frequency.QuadPart); //internally in millseconds
	m_endTime = m_endCount.QuadPart * (1000000.0 / m_frequency.QuadPart); //internally in millseconds
#else
	if(!m_stopped)
	{
		gettimeofday(&m_endCount,NULL);
	}
	m_startTime = (m_startCount.tv_sec * 1000000.0) + m_startCount.tv_usec; //internally in millseconds
	m_endTime = (m_endCount.tv_sec * 1000000.0) + m_endCount.tv_usec; //internally in millseconds
#endif //WIN32

	double time = m_endTime - m_startTime;

	switch(unit)
	{
	case Timer::SECONDS:
		time *= 0.000001;
		break;
	case Timer::MILLISECONDS:
		time *= 0.001;
		break;
	case Timer::MICROSECONDS:
		//already in microseconds
		break;
	default:
		throw Exception(Exception::SWITCH_CASE_UNDEFINED);
	}

	return time;
}

double Timer::delta(Timer::Unit unit)
{
	double e = elapsed(unit);
	double d = e - m_prevTime;
	m_prevTime = e;
	return d;
}

} //namespace foundation

} //namespace arkhe
