#pragma once
#include <Windows.h>
#include <vector>
#include <set>
#include <map>
#include "Common.h"
#include "PhysicsComponent.h"
#include "Actor.h"
#include "PlayerCar.h"
#include "Singleton.h"
#include "ComputerCarSpawner.h"
#include "FastCarSpawner.h"
#include "TimerUI.h"


class GameManager : public Singleton<GameManager>
{
	friend class Singleton<GameManager>;

public:
	void Initialize();
	void Destroy();
	void Tick(float InDeltaTime);
	void Render();
	void HandleKeyState(WPARAM InKey, bool InIsPressed);



	void RegistActor(RenderLayer InLayer, Actor* InActor);
	inline void RequestDestroy(Actor* DestroyTarget) { PendingDestroyActors.push_back(DestroyTarget); };

	static constexpr unsigned int ScreenWidth = 600;
	static constexpr unsigned int ScreenHeight = 400;
	static constexpr unsigned int ActorDefaultSize = 64;

	// Getter
	inline const HWND GetMainWindowHandle() const { return hMainWindow; }
	inline const Point& GetAppPosition() const { return AppPosition; }
	inline Gdiplus::Bitmap* GetBackBuffer() const { return BackBuffer; }
	inline PlayerCar* GetMainPlayer() const { return MainPlayer; }

	// Setter
	inline void SetMainWindowHandle(HWND InWindowHandle) {
		if (hMainWindow == nullptr)
		{
			hMainWindow = InWindowHandle;	
		}
	}
	inline void SetGameState(GameState InState) { State = InState; }
protected:
private:
	GameManager() = default;
	virtual ~GameManager() = default;

	void UnregisteActor(Actor* InActor);
	void ProcessCollisions();					// 충돌 처리 함수
	void ProcessPendingDestroyActors();			// 삭제 예정인 액터들을 실제로 정리하는 함수

	std::map<RenderLayer, std::set<Actor*>> Actors;
	std::vector<Actor*> PendingDestroyActors;	// 삭제 예정인 액터들의 목록
	std::map<PhysicsLayer, std::vector<PhysicsComponent*>> PhysicsComponents; // 물리 컴포넌트 리스트

	HWND hMainWindow = nullptr;
	Point AppPosition = Point(200, 100);
	Gdiplus::Bitmap* BackBuffer = nullptr;    // 백버퍼용 종이
	Gdiplus::Graphics* BackBufferGraphics = nullptr;  // 백버퍼용 종이에 그리기 위한 도구

	Gdiplus::Bitmap* IntroImage = nullptr;

	Gdiplus::Bitmap* GameOverImage = nullptr;

	PlayerCar* MainPlayer = nullptr;
	ComputerCarSpawner* Spawner = nullptr;
	TimerUI* Timer = nullptr;


	GameState State = GameState::Playing;
};

