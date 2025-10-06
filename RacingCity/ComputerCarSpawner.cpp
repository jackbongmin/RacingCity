#include "ComputerCarSpawner.h"
#include "Factory.h"
#include "ComputerCar.h"

void ComputerCarSpawner::OnInitialize()
{
    SetPosition(0, -100); // ȭ�� �ۿ� ��ġ ����. �߿��Ѱ� �ƴ�.

    timeSinceLastSpawn = 0.0f;
    hasInitialDelayPassed = false;
}

void ComputerCarSpawner::OnTick(float deltaTime)
{
    Actor::OnTick(deltaTime); // �θ� Ŭ������ OnTick ȣ��

    // ��� �ð� ������Ʈ
    timeSinceLastSpawn += deltaTime;

    // �ʱ� ���� �ð��� �������� Ȯ��
    if (!hasInitialDelayPassed)
    {
        if (timeSinceLastSpawn > initialDelay)
        {
            hasInitialDelayPassed = true;
            timeSinceLastSpawn = 0.0f; // Ÿ�̸� ����
        }
        return; // �ʱ� ���� �ð��� ������ �ʾ����� ���⼭ ����
    }

    // �ʱ� ���� �ð��� ���� ���� ��ź ���� ����
    if (timeSinceLastSpawn > spawnInterval)
    {
        // ��ź ����
        ComputerCarSpawner* newComputerCarSpawner = Factory::Get().SpawnActor<ComputerCarSpawner>(ResourceID::ComputerCarSpawner, RenderLayer::ComputerCarSpawner);

        // Ÿ�̸� ���� (���� ���� ������ ����)
        timeSinceLastSpawn -= spawnInterval;
    }
}
