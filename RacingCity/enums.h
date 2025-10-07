#pragma once
#include <Windows.h>
#include <cstdint>

enum class InputDirection : uint8_t
{
	None = 0,
	Up = VK_UP,
	Down = VK_DOWN,
	Left = VK_LEFT,
	Right = VK_RIGHT
};

enum class RenderLayer : uint8_t
{
	Background = 0,
	ComputerCar,
	PlayerCar,
	UI,
	Heart,
	Misc
};

enum class ResourceID : uint8_t
{
	None = 0,
	PlayerCar,
	ComputerCar,
	FastCar,
	Explosion,
	Background,
	Number0,
	Number1,
	Number2,
	Number3,
	Number4,
	Number5,
	Number6,
	Number7,
	Number8,
	Number9,
	Dot,
	Life3,
	Life2,
	Life1,
	GameOver,
	Intro,
};

enum class CollisionType
{
	Circle,
	Rectangle
};

enum class PhysicsLayer
{
	None = 0,
	PlayerCar,
	ComputerCar,
};

enum class GameState
{
	Intro,
	Playing,
	GameOver
};