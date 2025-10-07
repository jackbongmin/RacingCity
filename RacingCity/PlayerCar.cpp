#include "PlayerCar.h"
#include "Common.h"
#include "GameManager.h"
#include "ResourceManager.h"

void PlayerCar::OnInitialize()
{
    SetSize(GameManager::ActorDefaultSize / 1.2f);
    Position.X = 30.0f;
    Position.Y = 230.0f;
    KeyWasPressedMap[InputDirection::Up] = false;
    KeyWasPressedMap[InputDirection::Down] = false;
    KeyWasPressedMap[InputDirection::Left] = false;
    KeyWasPressedMap[InputDirection::Right] = false;

    PhysicsComponent* physicsComponent = new PhysicsComponent(this, CollisionType::Circle, PhysicsLayer::PlayerCar);
    physicsComponent->SetRadius(static_cast<float>(Size * 0.5f)); // 반지름 설정
    AddComponent(physicsComponent); // 물리 컴포넌트 추가

    ExplosionImage = ResourceManager::Get().GetImage(ResourceID::Explosion);
}

void PlayerCar::OnTick(float InDeltaTime)
{

    // 무적 상태일 때 시간 감소
    if (IsInvincible)
    {
        InvincibleTimer -= InDeltaTime;
        if (InvincibleTimer <= 0.0f)
        {
            IsInvincible = false;
        }
    }

    // 폭발 중이면 폭발 타이머 처리
    if (IsExploding)
    {
        ExplosionTimer -= InDeltaTime;
        if (ExplosionTimer <= 0.0f)
        {
            IsExploding = false;
        }
        return; // 폭발 중엔 플레이어 이동/입력 처리 X
    }

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
    if (IsExploding && ExplosionImage)
    {
        //폭발 이미지 표시
        InGraphics->DrawImage(
            ExplosionImage,
            static_cast<int>(Position.X - Size),
            static_cast<int>(Position.Y - Size),
            Size * 2, Size * 2);
        return;
    }

    // 무적 중이면 깜빡거리게 (0.1초 간격으로)
    if (IsInvincible)
    {
        int Blink = static_cast<int>(InvincibleTimer * 10) % 2;
        if (Blink == 0) return; // 그릴 타이밍이 아니면 skip
    }
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
    if (IsExploding || IsInvincible)
        return;
    
    if (InOther && InOther != this)
    {
        LoseLife();
        // 게임 오버 처리
        //GameManager::Get().SetGameState(GameState::GameOver);
    }
}

void PlayerCar::HandleKeyState(WPARAM InKey, bool InIsPressed)
{

    if (InKey == VK_LEFT || InKey == VK_RIGHT || InKey == VK_UP || InKey == VK_DOWN)
    {
        KeyWasPressedMap[static_cast<InputDirection>(InKey)] = InIsPressed;
    }
}

void PlayerCar::LoseLife()
{
    if (IsExploding || IsInvincible) return;

    Life--;
    IsExploding = true;      // 폭발 시작
    ExplosionTimer = 0.5f;    // 1초 동안 폭발 이미지 유지
    IsInvincible = true;     // 무적 시작
    InvincibleTimer = 1.5f;   // 2초 동안 무적

    if (Life > 0)
    {
        OutputDebugString(L"Player crash! Still alive\n");
    }
    else
    {
        GameManager::Get().SetGameState(GameState::GameOver);
        OutputDebugString(L"GameOver\n");
    }

}
