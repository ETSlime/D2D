#pragma once

#include "SingletonBase.h"
#include "Util.h"

class Timer : public SingletonBase<Timer>
{
public:
	friend class SingletonBase<Timer>;

	static bool Stopped() { return isTimerStopped; }
	static float Delta() { return isTimerStopped ? 0.0f : timeElapsed; }
	static float TotalTime() { return runningTime; }

	void Update();

	void Start();
	void Stop();

	static float FPS() { return framePerSecond; }
	float Running() const { return runningTime; }

private:
	Timer(void);
	~Timer(void);

	static bool isTimerStopped;
	static float timeElapsed;


	INT64 ticksPerSecond;
	INT64 currentTime;
	INT64 lastTime;
	INT64 lastFPSUpdate;
	INT64 fpsUpdateInterval;

	UINT frameCount;
	static float runningTime;
	static float framePerSecond;

};