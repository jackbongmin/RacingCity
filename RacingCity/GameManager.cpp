#include "GameManager.h"
#include "Background.h"
#include "PlayerCar.h"

#include "ResourceManager.h"
#include "Factory.h"
#include "PhysicsComponent.h"

void GameManager::Initialize()
{
    BackBuffer = new Gdiplus::Bitmap(
        GameManager::ScreenWidth, GameManager::ScreenHeight, PixelFormat32bppARGB);
    BackBufferGraphics = Gdiplus::Graphics::FromImage(BackBuffer);
    if (!BackBufferGraphics)
    {
        // Ȥ�� �ȸ�������� ���� ����� ���� ���
        MessageBox(hMainWindow, L"�� ���� �׷��Ƚ� ���� ����", L"����", MB_OK | MB_ICONERROR);
    }

    MainPlayer = Factory::Get().SpawnActor<PlayerCar>(ResourceID::PlayerCar, RenderLayer::PlayerCar);
    Factory::Get().SpawnActor<Background>(ResourceID::Background, RenderLayer::Background);

    Spawner = Factory::Get().SpawnActor<BombSpawner>(ResourceID::None);
    Timer = Factory::Get().SpawnActor<TimerUI>(ResourceID::None, RenderLayer::UI);
}

void GameManager::Destroy()
{
    PhysicsComponents.clear(); // ���� ������Ʈ ����Ʈ �ʱ�ȭ
    for (auto pair : Actors)
    {
        for (Actor* actor : pair.second)
        {
            actor->OnDestroy();
            delete actor;
        }
        pair.second.clear();
    }
    Actors.clear();

    delete BackBufferGraphics;
    BackBufferGraphics = nullptr;
    delete BackBuffer;
    BackBuffer = nullptr;

    Spawner = nullptr;
    MainPlayer = nullptr;
}

void GameManager::Tick(float InDeltaTime)
{
    if (State == GameState::Playing)
    {
        for (const auto& pair : Actors)
        {
            for (Actor* actor : pair.second)
            {
                actor->OnTick(InDeltaTime);
            }
        }
        ProcessCollisions();    // �浹 ó��
        ProcessPendingDestroyActors();  // ���� ������ ���͵��� ��� ����
    }
}

void GameManager::Render()
{
    if (BackBufferGraphics)   // g_BackBufferGraphics �ʼ�
    {
        BackBufferGraphics->Clear(Gdiplus::Color(255, 0, 0, 0));

        for (const auto& pair : Actors)
        {
            for (Actor* actor : pair.second)
            {
                actor->OnRender(BackBufferGraphics);
            }
        }
    }
}

void GameManager::HandleKeyState(WPARAM InKey, bool InIsPressed)
{
    MainPlayer->HandleKeyState(InKey, InIsPressed);
}

void GameManager::RegistActor(RenderLayer InLayer, Actor* InActor)
{
    if (InActor)
    {
        Actors[InLayer].insert(InActor);

        PhysicsComponent* physicsComponent = InActor->GetComponent<PhysicsComponent>();
        if (physicsComponent && physicsComponent->GetLayer() != PhysicsLayer::None)
        {
            PhysicsComponents[physicsComponent->GetLayer()].push_back(physicsComponent);
        }
    }
}

void GameManager::UnregisteActor(Actor* InActor)
{
    std::set<Actor*>& actorSet = Actors[InActor->GetRenderLayer()];

    if (actorSet.find(InActor) != actorSet.end())
    {
        PhysicsComponent* physicsComponent = InActor->GetComponent<PhysicsComponent>();
        if (physicsComponent)
        {
            // ���� ������Ʈ ����
            auto& physicsObjects = PhysicsComponents[physicsComponent->GetLayer()];
            auto it = std::find(physicsObjects.begin(), physicsObjects.end(), physicsComponent);
            if (it != physicsObjects.end())
            {
                std::swap(*it, physicsObjects.back());	// ������ ���� ������Ʈ�� ����
                physicsObjects.pop_back();				// ������ ���� ������Ʈ ����
            }
        }
        actorSet.erase(InActor);
    }


    //for (size_t i = 0; i < actorSet.size(); i++)
    //{
    //    if (actorSet[i] == InActor)
    //    {
    //        if (i < actorSet.size() - 1)//�������� �ƴϸ�
    //        {
    //            std::swap(actorSet[i], actorSet.back());
    //        }
    //        actorSet.pop_back();
    //        break;
    //    }
    //}
}

void GameManager::ProcessCollisions()
{
    PhysicsComponent* player = *(PhysicsComponents[PhysicsLayer::PlayerCar].begin());	// �÷��̾�� 1����

    // �÷��̾ ��� ��ź�� �浹�ϴ����� Ȯ��
    // Ȯ�� �� ���� �ݸ��� Ÿ�Կ� ���� ó��(���� ��, ���� �簢��, �簢���� �簢�� �� 3���� ���̽�)
    for (auto& bomb : PhysicsComponents[PhysicsLayer::ComputerCar])
    {
        if (player->IsCollision(bomb)) // �÷��̾�� ��ź ���� �浹 Ȯ��
        {
            // �浹 �߻� �� �÷��̾�� ��ź�� OnOverlap ȣ��
            player->GetOwner()->OnOverlap(bomb->GetOwner());
            bomb->GetOwner()->OnOverlap(player->GetOwner());
        }
    }
}

void GameManager::ProcessPendingDestroyActors()
{
    for (Actor* actor : PendingDestroyActors)
    {
        if (actor)
        {
            UnregisteActor(actor);
            actor->OnDestroy();
            delete actor;
        }
    }
    PendingDestroyActors.clear();
}
