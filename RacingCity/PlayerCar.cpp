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
    physicsComponent->SetRadius(static_cast<float>(Size * 0.5f)); // ������ ����
    AddComponent(physicsComponent); // ���� ������Ʈ �߰�

    ExplosionImage = ResourceManager::Get().GetImage(ResourceID::Explosion);
}

void PlayerCar::OnTick(float InDeltaTime)
{

    // ���� ������ �� �ð� ����
    if (IsInvincible)
    {
        InvincibleTimer -= InDeltaTime;
        if (InvincibleTimer <= 0.0f)
        {
            IsInvincible = false;
        }
    }

    // ���� ���̸� ���� Ÿ�̸� ó��
    if (IsExploding)
    {
        ExplosionTimer -= InDeltaTime;
        if (ExplosionTimer <= 0.0f)
        {
            IsExploding = false;
        }
        return; // ���� �߿� �÷��̾� �̵�/�Է� ó�� X
    }

    float MoveDistance = InDeltaTime * Speed;
    // �¿� �̵�
    if (KeyWasPressedMap[InputDirection::Left])
    {
        Position.X -= MoveDistance;
    }
    if (KeyWasPressedMap[InputDirection::Right])
    {
        Position.X += MoveDistance;
    }
    // ���Ʒ� �̵�
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
        //���� �̹��� ǥ��
        InGraphics->DrawImage(
            ExplosionImage,
            static_cast<int>(Position.X - Size),
            static_cast<int>(Position.Y - Size),
            Size * 2, Size * 2);
        return;
    }

    // ���� ���̸� �����Ÿ��� (0.1�� ��������)
    if (IsInvincible)
    {
        int Blink = static_cast<int>(InvincibleTimer * 10) % 2;
        if (Blink == 0) return; // �׸� Ÿ�̹��� �ƴϸ� skip
    }
    Actor::OnRender(InGraphics);    // �θ��� OnRender ����

    // �θ��� OnRender ������ �� �߰��� ���� ����
    if (!Image)
    {
        Gdiplus::SolidBrush RedBrush(Gdiplus::Color(255, 255, 0, 0));
        InGraphics->FillEllipse(
            &RedBrush,
            static_cast<int>(Position.X - Size * Pivot.X),    // �׷��� ��ġ
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
        // ���� ���� ó��
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
    IsExploding = true;      // ���� ����
    ExplosionTimer = 0.5f;    // 1�� ���� ���� �̹��� ����
    IsInvincible = true;     // ���� ����
    InvincibleTimer = 1.5f;   // 2�� ���� ����

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
