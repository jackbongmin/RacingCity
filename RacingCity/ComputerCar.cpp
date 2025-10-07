#include "ComputerCar.h"
#include "GameManager.h"
#include "PhysicsComponent.h"

void ComputerCar::OnInitialize()
{
    SetSize(GameManager::ActorDefaultSize / 1.2f);

    // ���� ���� ���� Y ��ġ (111~252)
    float spawnY = 111.0f + static_cast<float>(rand() % 142);

    // ������ ȭ�� �ۿ��� ����
    Position.X = static_cast<float>(GameManager::ScreenWidth + Size);
    Position.Y = spawnY;

    //Angle = 180.0f; // ���� ����

    // ���� �浹 �ڽ� ����
    float HalfSize = Size * 0.5f;
    PhysicsComponent* physicsComponent =
        new PhysicsComponent(this, CollisionType::Rectangle, PhysicsLayer::ComputerCar);
    physicsComponent->SetWidth(HalfSize);
    physicsComponent->SetHeight(HalfSize * 0.75f);
    AddComponent(physicsComponent);
}

void ComputerCar::OnTick(float deltaTime)
{
    Actor::OnTick(deltaTime);

    // ������ �ӵ��� ���� �̵�
    Position.X += moveSpeed * deltaTime;

    // ���� ����
    lifetime -= deltaTime;

    // ���� ȭ�� ������ ����ų� ���� ���� �� ����
    if (Position.X < -Size || lifetime < 0.0f)
    {
        DestroyActor();
    }
}

void ComputerCar::OnOverlap(Actor* other)
{
    if (other && other != this)
    {
        DestroyActor();
    }
}
