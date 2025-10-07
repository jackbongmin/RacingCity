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
    float SpawnInterval = 2.5f;       // 2.5�ʸ��� ����
    float TimeSinceLastSpawn = 0.0f;  // ��� �ð�
    float TotalElapsed = 0.0f;        // ��ü ��� �ð� (10�� ���ĺ��� ����)
};
