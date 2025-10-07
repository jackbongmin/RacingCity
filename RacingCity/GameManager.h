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
	void ProcessCollisions();					// �浹 ó�� �Լ�
	void ProcessPendingDestroyActors();			// ���� ������ ���͵��� ������ �����ϴ� �Լ�

	std::map<RenderLayer, std::set<Actor*>> Actors;
	std::vector<Actor*> PendingDestroyActors;	// ���� ������ ���͵��� ���
	std::map<PhysicsLayer, std::vector<PhysicsComponent*>> PhysicsComponents; // ���� ������Ʈ ����Ʈ

	HWND hMainWindow = nullptr;
	Point AppPosition = Point(200, 100);
	Gdiplus::Bitmap* BackBuffer = nullptr;    // ����ۿ� ����
	Gdiplus::Graphics* BackBufferGraphics = nullptr;  // ����ۿ� ���̿� �׸��� ���� ����

	Gdiplus::Bitmap* IntroImage = nullptr;

	Gdiplus::Bitmap* GameOverImage = nullptr;

	PlayerCar* MainPlayer = nullptr;
	ComputerCarSpawner* Spawner = nullptr;
	TimerUI* Timer = nullptr;


	GameState State = GameState::Playing;
};

