#pragma once
#include <time.h>
#include <stdint.h>

class GlTimer
{
public:
	GlTimer();
	~GlTimer();

	void Start();
	float Now();
	void Stop();
	bool Running();

private:
	int32_t mStartTime;
	int32_t mStopTime;
	int32_t mPassTime;
	int32_t mCurTime;
	bool	mIsRunning;
};

