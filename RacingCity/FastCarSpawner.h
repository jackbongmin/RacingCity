#pragma once
#include "Actor.h"

class FastCarSpawner : public Actor
{
public:
    FastCarSpawner() = delete;
    FastCarSpawner(ResourceID resourceId) : Actor(resourceId) {};

    virtual void OnInitialize() override;
    virtual void OnTick(float deltaTime) override;

private:
    float SpawnInterval = 2.5f;       // 2.5초마다 스폰
    float TimeSinceLastSpawn = 0.0f;  // 경과 시간
    float TotalElapsed = 0.0f;        // 전체 경과 시간 (10초 이후부터 스폰)
};
