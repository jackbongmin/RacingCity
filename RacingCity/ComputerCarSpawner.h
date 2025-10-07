#pragma once
#include "Actor.h"



class ComputerCarSpawner : public Actor
{
public:
	ComputerCarSpawner() = delete;
	ComputerCarSpawner(ResourceID resourceId) : Actor(resourceId) {};

	virtual void OnInitialize() override;
	virtual void OnTick(float deltaTime) override;

private:
	float initialDelay = 2.0f;			// 초기 차량 생성 지연 시간
	float spawnInterval = 0.5f;			// 초당 차량 생성 간격

	bool hasInitialDelayPassed = false;	
	float timeSinceLastSpawn = 0.0f;	
};

