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
	float initialDelay = 2.0f;			// �ʱ� ���� ���� ���� �ð�
	float spawnInterval = 0.5f;			// �ʴ� ���� ���� ����

	bool hasInitialDelayPassed = false;	
	float timeSinceLastSpawn = 0.0f;	
};

