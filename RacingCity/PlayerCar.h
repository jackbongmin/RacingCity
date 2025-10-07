#pragma once
#include <unordered_map>
#include "enums.h"
#include "Actor.h"

class PlayerCar : public Actor
{
public:
	PlayerCar() = delete;
	PlayerCar(ResourceID InID) : Actor(InID) {};

	virtual void OnInitialize() override;
	virtual void OnTick(float InDeltaTime) override;
	virtual void OnRender(Gdiplus::Graphics* InGraphics) override;
	virtual void OnOverlap(Actor* InOther) override;

	void HandleKeyState(WPARAM InKey, bool InIsPressed);

	void LoseLife();

	inline float GetSpeed() const { return Speed; }
	inline void SetSpeed(float InSpeed) { Speed = InSpeed; }
	inline int GetLife() const { return Life; }

private:
	// 플레이어의 이동 속도
	float Speed = 250.0f;

	// 플레이어 키 입력 상태
	std::unordered_map<InputDirection, bool> KeyWasPressedMap;

	int Life = 3;
	bool IsInvincible = false;
	float InvincibleTimer = 0.0f;

	bool IsExploding = false;
	float ExplosionTimer = 0.0f;
	Gdiplus::Image* ExplosionImage = nullptr;

};