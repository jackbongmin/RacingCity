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
        //  Y 범위 (도로 영역)
        float spawnY = 139.0f + static_cast<float>(rand() % 191);
        // 오른쪽 화면 바깥 X
        float spawnX = static_cast<float>(GameManager::ScreenWidth + 100.0f);

        //  ComputerCar 생성
        ComputerCar* newComputerCar = Factory::Get().SpawnActor<ComputerCar>(
            ResourceID::ComputerCar, RenderLayer::ComputerCar);

        newComputerCar->SetPosition(spawnX, spawnY);

        // 차마다 다른 랜덤 속도 부여 (-200 ~ -350)
        float randomSpeed = -350.0f - static_cast<float>(rand() % 150);
        newComputerCar->SetSpeed(randomSpeed);


        // 다음 스폰 준비
        timeSinceLastSpawn -= spawnInterval;
    }
}
