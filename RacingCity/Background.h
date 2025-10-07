#pragma once
#include "Actor.h"

class Background : public Actor
{
public:
	Background() = delete;
	Background(ResourceID InID);

	virtual void OnTick(float InDeltaTime) override;
	virtual void OnRender(Gdiplus::Graphics* InGraphics) override;

private:
	// 스크롤 속도
	float ScrollSpeed = 500.0f;

	float Offset = 0.0f;
};

