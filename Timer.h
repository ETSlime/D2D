#pragma once

#include "SingletonBase.h"
#include "Util.h"

class Timer : public SingletonBase<Timer>
{
public:
	friend class SingletonBase<Timer>;

	static bool Stopped() { return isTimerStopped; }
	static float Delta() { return isTimerStopped ? 0.0f : timeElapsed; }

	void Update();

	void Start();
	void Stop();

	float FPS() const { return framePerSecond; }
	float Running() const { return runningTime; }

private:
	Timer(void);
	~Timer(void);

	static bool isTimerStopped;///< Ÿ�̸� ����E
	static float timeElapsed;///< ����E���������κ��� ����ð�


	INT64 ticksPerSecond;///< �ʴ�Eƽī���
	INT64 currentTime;///< ����E�ð�
	INT64 lastTime;///< ����Eð�
	INT64 lastFPSUpdate;///< ������ FPS ������Ʈ �ð�
	INT64 fpsUpdateInterval;///< fps ������Ʈ ����

	UINT frameCount;///< ������ ��E
	float runningTime;///< ��ǁE�ð�
	float framePerSecond;///< FPS

};