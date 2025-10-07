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

    // Setter
    void SetSpeed(float newSpeed) { moveSpeed = newSpeed; }

private:
    float moveSpeed = -300.0f; // ������ ���� �̵� �ӵ�
    float lifetime = 6.0f;     // ���� (�� ����)
};
