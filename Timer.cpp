#include "Timer.h"

bool Timer::isTimerStopped = true;
float Timer::timeElapsed = 0.0f;
float Timer::runningTime = 1.0f;
float Timer::framePerSecond = 0.0f;

Timer::Timer(void):
	ticksPerSecond(0), currentTime(0), lastTime(0), lastFPSUpdate(0), fpsUpdateInterval(0),
	frameCount(0)
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&ticksPerSecond);
	fpsUpdateInterval = ticksPerSecond >> 1;
}

Timer::~Timer(void)
{

}

void Timer::Update()
{
	if (isTimerStopped) return;

	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
	timeElapsed = (float)(currentTime - lastTime) / (float)ticksPerSecond;
	runningTime += timeElapsed;


	frameCount++;
	if (currentTime - lastFPSUpdate >= fpsUpdateInterval)
	{
		float tempCurrentTime = (float)currentTime / (float)ticksPerSecond;
		float tempLastTime = (float)lastFPSUpdate / (float)ticksPerSecond;
		framePerSecond = (float)frameCount / (tempCurrentTime - tempLastTime);

		lastFPSUpdate = (INT64)currentTime;
		frameCount = 0;
	}

	lastTime = currentTime;
}

void Timer::Start()
{
	if (!isTimerStopped)
		assert(false);

	QueryPerformanceCounter((LARGE_INTEGER*)&lastTime);
	isTimerStopped = false;
}

void Timer::Stop()
{
	if (isTimerStopped)
		assert(false);

	INT64 stopTime = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&stopTime);
	runningTime += (float)(stopTime - lastTime) / (float)ticksPerSecond;
	isTimerStopped = true;
}