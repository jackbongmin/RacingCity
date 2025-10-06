#pragma once
#include "Actor.h"

class ComputerCar : public Actor
{
public:
	ComputerCar() = delete;
	ComputerCar(ResourceID resourceId) : Actor(resourceId) {};

	virtual void OnInitialize() override;
	virtual void OnTick(float deltaTime) override;

	virtual void OnOverlap(Actor* other) override;

	// Getters
	// Setters
	void SetDropAcceleration(float newAcceleration) { dropAcceleration = newAcceleration; }

private:
	float dropAcceleration = 200.0f; // ��� ���ӵ�(�ʴ� �ȼ� ����)
	float dropSpeed = 0.0f; // ���� ��� �ӵ�
	float lifetime = 5.0f; // ��ź�� ����(�� ����)
};