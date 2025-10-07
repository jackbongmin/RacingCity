#include "GameManager.h"
#include "Background.h"
#include "PlayerCar.h"
#include "LifeUI.h"

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
        // 혹시 안만들어졌을 때를 대비한 에러 출력
        MessageBox(hMainWindow, L"백 버퍼 그래픽스 생성 실패", L"오류", MB_OK | MB_ICONERROR);
    }

    IntroImage = ResourceManager::Get().GetImage(ResourceID::Intro);

    GameOverImage = ResourceManager::Get().GetImage(ResourceID::GameOver);

    State = GameState::Intro;
    if (!IntroImage)
    {
        OutputDebugString(L"[IntroImage] 이미지 로드 실패!\n");
    }
    else
    {
        OutputDebugString(L"[IntroImage] 이미지 로드 성공!\n");
    }

}

void GameManager::Destroy()
{
    PhysicsComponents.clear(); // 물리 컴포넌트 리스트 초기화
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
        ProcessCollisions();    // 충돌 처리
        ProcessPendingDestroyActors();  // 삭제 예정인 액터들을 모두 삭제
    }
}

void GameManager::Render()
{
    if (BackBufferGraphics)   
    {
        BackBufferGraphics->Clear(Gdiplus::Color(255, 0, 0, 0));

        if (State == GameState::Intro && IntroImage)
        {
            const int imgW = IntroImage->GetWidth();
            const int imgH = IntroImage->GetHeight();

            // 화면 크기에 맞게 비율 조정
            float scaleX = static_cast<float>(ScreenWidth) / imgW;
            float scaleY = static_cast<float>(ScreenHeight) / imgH;
            float scale = min(scaleX, scaleY); // 비율 유지

            int drawW = static_cast<int>(imgW * scale);
            int drawH = static_cast<int>(imgH * scale);

            int centerX = (ScreenWidth - drawW) / 2;
            int centerY = (ScreenHeight - drawH) / 2;

            BackBufferGraphics->DrawImage(IntroImage, centerX, centerY, drawW, drawH);
            return;
        }

        for (const auto& pair : Actors)
        {
            for (Actor* actor : pair.second)
            {
                actor->OnRender(BackBufferGraphics);
            }
        }

        if (State == GameState::GameOver && GameOverImage)
        {
            const int imgW = GameOverImage->GetWidth();
            const int imgH = GameOverImage->GetHeight();

            // 화면 크기에 맞게 변경
            float scaleX = static_cast<float>(ScreenWidth) / imgW;
            float scaleY = static_cast<float>(ScreenHeight) / imgH;
            float scale = min(scaleX, scaleY) * 0.75f; // 크기조정

            int drawW = static_cast<int>(imgW * scale);
            int drawH = static_cast<int>(imgH * scale);

            //  중앙 정렬
            int centerX = (ScreenWidth - drawW) / 2;
            int centerY = (ScreenHeight - drawH) / 2;

            //  게임오버 이미지 그리기
            BackBufferGraphics->DrawImage(
                GameOverImage,
                centerX,
                centerY,
                drawW,
                drawH
            );
        }
    }
}

void GameManager::HandleKeyState(WPARAM InKey, bool InIsPressed)
{
    // 인트로
    if (State == GameState::Intro)
    {
        State = GameState::Playing;

        // 액터들 생성
        Factory::Get().SpawnActor<Background>(ResourceID::Background, RenderLayer::Background);
        MainPlayer = Factory::Get().SpawnActor<PlayerCar>(ResourceID::PlayerCar, RenderLayer::PlayerCar);
        Factory::Get().SpawnActor<LifeUI>(ResourceID::None, RenderLayer::UI);
        Factory::Get().SpawnActor<ComputerCarSpawner>(ResourceID::None);
        Factory::Get().SpawnActor<FastCarSpawner>(ResourceID::None);
        Timer = Factory::Get().SpawnActor<TimerUI>(ResourceID::None, RenderLayer::UI);
        return;
    }

    // 플레이 중일 때만 키 먹히게 함
    if (State == GameState::Playing)
    {
        MainPlayer->HandleKeyState(InKey, InIsPressed);
    }
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
            auto& physicsObjects = PhysicsComponents[physicsComponent->GetLayer()];
            auto it = std::find(physicsObjects.begin(), physicsObjects.end(), physicsComponent);
            if (it != physicsObjects.end())
            {
                std::swap(*it, physicsObjects.back());
                physicsObjects.pop_back();				
            }
        }
        actorSet.erase(InActor);
    }
}

void GameManager::ProcessCollisions()
{
    PhysicsComponent* player = *(PhysicsComponents[PhysicsLayer::PlayerCar].begin());

    for (auto& bomb : PhysicsComponents[PhysicsLayer::ComputerCar])
    {
        if (player->IsCollision(bomb))
        {
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
