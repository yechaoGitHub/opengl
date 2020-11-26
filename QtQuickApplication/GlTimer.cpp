#include "GlTimer.h"

GlTimer::GlTimer():
	mStartTime(0),
	mStopTime(0),
	mPassTime(0),
	mCurTime(0),
	mIsRunning(false)
{
}

GlTimer::~GlTimer()
{
}

void GlTimer::Start()
{
	if (!mIsRunning) 
	{
		mPassTime += mStopTime - mStartTime;
		mStartTime = ::clock();
		mIsRunning = true;
	}
}

float GlTimer::Now()
{
	if (mIsRunning)
	{
		mCurTime = ::clock();
	}
	
	return static_cast<double>(mCurTime - mStartTime + mPassTime) / 1000.0;
}

void GlTimer::Stop()
{
	if (mIsRunning) 
	{
		mStopTime = ::clock();
		mIsRunning = false;
	}
}

bool GlTimer::Running()
{
	return mIsRunning;
}
