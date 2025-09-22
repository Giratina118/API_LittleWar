#pragma once
#include <Windows.h>

class Timer
{
public:
	void Init();
	void Update();

	float GetDeltaTime() { return deltaTime; }

private:
	LARGE_INTEGER frequency;
	LARGE_INTEGER lastCount;

	float deltaTime;
};