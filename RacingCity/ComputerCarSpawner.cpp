
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

    static float TotalElapsed = 0.0f;  // ��ü �÷��� �ð� ����
    static float LastDifficultyUpdate = 0.0f; // ������ ���̵� ���� ����
    TotalElapsed += deltaTime;

    //10�ʸ��� ���� �ӵ� ����
    if (TotalElapsed - LastDifficultyUpdate >= 4.0f)
    {
        // ���� �ֱ⸦ 0.05�� ���̵�, 0.15�ʺ��� �۾����� �ʰ� ����
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
        // Y ���� (���� ����)
        float spawnY = 139.0f + static_cast<float>(rand() % 191);
        // ������ ȭ�� �ٱ� X
        float spawnX = static_cast<float>(GameManager::ScreenWidth + 100.0f);

        // ComputerCar ����
        ComputerCar* newComputerCar = Factory::Get().SpawnActor<ComputerCar>(
            ResourceID::ComputerCar, RenderLayer::ComputerCar);

        newComputerCar->SetPosition(spawnX, spawnY);

        // ������ �ٸ� ���� �ӵ� �ο� (-350 ~ -550)
        float randomSpeed = -400.0f - static_cast<float>(rand() % 150);
        newComputerCar->SetSpeed(randomSpeed);


        // ���� ���� �غ�
        timeSinceLastSpawn -= spawnInterval;
    }
}
