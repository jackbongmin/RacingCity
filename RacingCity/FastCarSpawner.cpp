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

    // 10�� ������ ����x
    if (TotalElapsed < 10.0f)
        return;

    static float LastDifficultyUpdate = 10.0f;
    if (TotalElapsed - LastDifficultyUpdate >= 4.0f)
    {
        // ���� �ֱ⸦ 0.05�� ���̵�, 1.5�ʺ��� �۾����� �ʰ� ����
        SpawnInterval -= 0.5f;
        if (SpawnInterval < 1.0f)
        {
            SpawnInterval = 1.0f;
        }

        LastDifficultyUpdate = TotalElapsed;
        OutputDebugString(L"Difficulty level up! Spawn cycle reduction\n");
    }


    //  1�ʸ��� ����
    if (TimeSinceLastSpawn >= SpawnInterval)
    {
        float spawnY = 139.0f + static_cast<float>(rand() % 191);
        float spawnX = static_cast<float>(GameManager::ScreenWidth + 100.0f);

        ComputerCar* newCar = Factory::Get().SpawnActor<ComputerCar>(
            ResourceID::FastCar, RenderLayer::ComputerCar);

        newCar->SetPosition(spawnX, spawnY);
        newCar->SetSpeed(-700.0f); // �������� �ӵ�

        TimeSinceLastSpawn = 0.0f;
    }
}
