
#include "ComputerCarSpawner.h"
#include "Factory.h"
#include "ComputerCar.h"
#include "GameManager.h"



void ComputerCarSpawner::OnInitialize()
{
    SetPosition(0, -100);
    timeSinceLastSpawn = 0.0f;
    hasInitialDelayPassed = false;
}

void ComputerCarSpawner::OnTick(float deltaTime)
{
    Actor::OnTick(deltaTime);
    timeSinceLastSpawn += deltaTime;

    static float TotalElapsed = 0.0f;  // 전체 플레이 시간 누적
    static float LastDifficultyUpdate = 0.0f; // 마지막 난이도 갱신 시점
    TotalElapsed += deltaTime;

    //10초마다 스폰 속도 조절
    if (TotalElapsed - LastDifficultyUpdate >= 4.0f)
    {
        // 스폰 주기를 0.05초 줄이되, 0.15초보다 작아지지 않게 고정
        spawnInterval -= 0.05f;
        if (spawnInterval < 0.15f)
        {
            spawnInterval = 0.15f;
        }

        LastDifficultyUpdate = TotalElapsed;
        OutputDebugString(L"Difficulty level up! Spawn cycle reduction\n");
    }

    if (!hasInitialDelayPassed)
    {
        if (timeSinceLastSpawn > initialDelay)
        {
            hasInitialDelayPassed = true;
            timeSinceLastSpawn = 0.0f;
        }
        return;
    }

    if (timeSinceLastSpawn > spawnInterval)
    {
        // Y 범위 (도로 영역)
        float spawnY = 139.0f + static_cast<float>(rand() % 191);
        // 오른쪽 화면 바깥 X
        float spawnX = static_cast<float>(GameManager::ScreenWidth + 100.0f);

        // ComputerCar 생성
        ComputerCar* newComputerCar = Factory::Get().SpawnActor<ComputerCar>(
            ResourceID::ComputerCar, RenderLayer::ComputerCar);

        newComputerCar->SetPosition(spawnX, spawnY);

        // 차마다 다른 랜덤 속도 부여 (-350 ~ -550)
        float randomSpeed = -400.0f - static_cast<float>(rand() % 150);
        newComputerCar->SetSpeed(randomSpeed);


        // 다음 스폰 준비
        timeSinceLastSpawn -= spawnInterval;
    }
}
