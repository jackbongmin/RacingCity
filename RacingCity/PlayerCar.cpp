#include "PlayerCar.h"
#include "Common.h"
#include "GameManager.h"

void PlayerCar::OnInitialize()
{
    SetSize(GameManager::ActorDefaultSize / 1.2f);
    Position.X = 30.0f;
    Position.Y = 180.0f;
    KeyWasPressedMap[InputDirection::Up] = false;
    KeyWasPressedMap[InputDirection::Down] = false;
    KeyWasPressedMap[InputDirection::Left] = false;
    KeyWasPressedMap[InputDirection::Right] = false;

    PhysicsComponent* physicsComponent = new PhysicsComponent(this, CollisionType::Circle, PhysicsLayer::PlayerCar);
    physicsComponent->SetRadius(static_cast<float>(Size * 0.5f)); // 반지름 설정
    AddComponent(physicsComponent); // 물리 컴포넌트 추가
}

void PlayerCar::OnTick(float InDeltaTime)
{
    float MoveDistance = InDeltaTime * Speed;
    // 좌우 이동
    if (KeyWasPressedMap[InputDirection::Left])
    {
        Position.X -= MoveDistance;
    }
    if (KeyWasPressedMap[InputDirection::Right])
    {
        Position.X += MoveDistance;
    }
    // 위아래 이동
    if (KeyWasPressedMap[InputDirection::Up])
    {
        Position.Y -= MoveDistance;
    }
    if (KeyWasPressedMap[InputDirection::Down])
    {
        Position.Y += MoveDistance;
    }

    const float RoadTop = 139.0f;   
    const float RoadBottom = 330.0f; 
    const float RoadLeft = 30.0f;      
    const float RoadRight = 380.0f;

    if (Position.Y < RoadTop)
        Position.Y = RoadTop;
    else if (Position.Y > RoadBottom)
        Position.Y = RoadBottom;

    if (Position.X < RoadLeft)
        Position.X = RoadLeft;
    else if (Position.X > RoadRight)
        Position.X = RoadRight;

}

void PlayerCar::OnRender(Gdiplus::Graphics* InGraphics)
{
    Actor::OnRender(InGraphics);    // 부모의 OnRender 실행

    // 부모의 OnRender 실행한 후 추가로 동작 실행
    if (!Image)
    {
        Gdiplus::SolidBrush RedBrush(Gdiplus::Color(255, 255, 0, 0));
        InGraphics->FillEllipse(
            &RedBrush,
            static_cast<int>(Position.X - Size * Pivot.X),    // 그려질 위치
            static_cast<int>(Position.Y - Size * Pivot.Y),
            Size, Size);
    }
}

void PlayerCar::OnOverlap(Actor* InOther)
{
    OutputDebugString(L"Player::OnOverlap called\n");
    if (InOther && InOther != this)
    {
        // 게임 오버 처리
        GameManager::Get().SetGameState(GameState::GameOver);
    }
}

void PlayerCar::HandleKeyState(WPARAM InKey, bool InIsPressed)
{
    if (InKey == VK_LEFT || InKey == VK_RIGHT || InKey == VK_UP || InKey == VK_DOWN)
    {
        KeyWasPressedMap[static_cast<InputDirection>(InKey)] = InIsPressed;
    }
}
