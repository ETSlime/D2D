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

	static bool isTimerStopped;///< ﾅｸﾀﾌｸﾓ ﾁﾟﾁ・
	static float timeElapsed;///< ﾀﾌﾀ・ﾇﾁｷｹﾀﾓﾀｸｷﾎｺﾎﾅﾍ ｰ豌悅ﾃｰ｣


	INT64 ticksPerSecond;///< ﾃﾊｴ・ﾆｽﾄｫｿ鏆ｮ
	INT64 currentTime;///< ﾇ・ｽﾃｰ｣
	INT64 lastTime;///< ﾀﾌﾀ・ﾃｰ｣
	INT64 lastFPSUpdate;///< ｸｶﾁｷ FPS ｾ･ﾀﾌﾆｮ ｽﾃｰ｣
	INT64 fpsUpdateInterval;///< fps ｾ･ﾀﾌﾆｮ ｰ｣ｰﾝ

	UINT frameCount;///< ﾇﾁｷｹﾀﾓ ｼ・
	float runningTime;///< ﾁ・ｽﾃｰ｣
	float framePerSecond;///< FPS

};