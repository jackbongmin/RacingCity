#pragma once
#include "UserInterface.h"

class LifeUI : public UserInterface
{
public:
    LifeUI() = delete;
    LifeUI(ResourceID resourceId) : UserInterface(resourceId) {}

    virtual void OnInitialize() override;
    virtual void OnRender(Gdiplus::Graphics* graphics) override;

private:
    Gdiplus::Bitmap* life3 = nullptr;
    Gdiplus::Bitmap* life2 = nullptr;
    Gdiplus::Bitmap* life1 = nullptr;
};
