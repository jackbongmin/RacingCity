#include "FastCarSpawner.h"
#include "Factory.h"
#include "ComputerCar.h"
#include "GameManager.h"

void FastCarSpawner::OnInitialize()
{
    SetPosition(0, -100);
    TimeSinceLastSpawn = 0.0f;
    TotalElapsed = 0.0f;
    SpawnInterval = 2.5f;
}

void FastCarSpawner::OnTick(float deltaTime)
{
    Actor::OnTick(deltaTime);
    TimeSinceLastSpawn += deltaTime;
    TotalElapsed += deltaTime;

    // 10초 이전엔 스폰x
    if (TotalElapsed < 10.0f)
        return;

    static float LastDifficultyUpdate = 10.0f;
    if (TotalElapsed - LastDifficultyUpdate >= 4.0f)
    {
        // 스폰 주기를 0.05초 줄이되, 1.5초보다 작아지지 않게 고정
        SpawnInterval -= 0.5f;
        if (SpawnInterval < 1.0f)
        {
            SpawnInterval = 1.0f;
        }

        LastDifficultyUpdate = TotalElapsed;
        OutputDebugString(L"Difficulty level up! Spawn cycle reduction\n");
    }


    //  1초마다 스폰
    if (TimeSinceLastSpawn >= SpawnInterval)
    {
        float spawnY = 139.0f + static_cast<float>(rand() % 191);
        float spawnX = static_cast<float>(GameManager::ScreenWidth + 100.0f);

        ComputerCar* newCar = Factory::Get().SpawnActor<ComputerCar>(
            ResourceID::FastCar, RenderLayer::ComputerCar);

        newCar->SetPosition(spawnX, spawnY);
        newCar->SetSpeed(-700.0f); // 역주행차 속도

        TimeSinceLastSpawn = 0.0f;
    }
}
