#include "Timer.h"

void Timer::Init()
{
	QueryPerformanceCounter(&lastCount);
	QueryPerformanceFrequency(&frequency);
}

void Timer::Update()
{
	LARGE_INTEGER currentCount;
	QueryPerformanceCounter(&currentCount);

	deltaTime = static_cast<float>(currentCount.QuadPart - lastCount.QuadPart) / static_cast<float>(frequency.QuadPart);
	lastCount = currentCount;
}