#include "ComputerCar.h"
#include "GameManager.h"
#include "PhysicsComponent.h"

void ComputerCar::OnInitialize()
{
    SetSize(GameManager::ActorDefaultSize / 1.2f);

    // 도로 위의 랜덤 Y 위치 (111~252)
    float spawnY = 111.0f + static_cast<float>(rand() % 142);

    // 오른쪽 화면 밖에서 시작
    Position.X = static_cast<float>(GameManager::ScreenWidth + Size);
    Position.Y = spawnY;

    //Angle = 180.0f; // 왼쪽 방향

    // 물리 충돌 박스 설정
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

    // 일정한 속도로 왼쪽 이동
    Position.X += moveSpeed * deltaTime;

    // 수명 감소
    lifetime -= deltaTime;

    // 왼쪽 화면 밖으로 벗어나거나 수명 만료 시 삭제
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
