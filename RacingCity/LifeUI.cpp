#include "LifeUI.h"
#include "ResourceManager.h"
#include "GameManager.h"
#include "PlayerCar.h"

void LifeUI::OnInitialize()
{
    life3 = ResourceManager::Get().GetImage(ResourceID::Life3);
    life2 = ResourceManager::Get().GetImage(ResourceID::Life2);
    life1 = ResourceManager::Get().GetImage(ResourceID::Life1);
}

void LifeUI::OnRender(Gdiplus::Graphics* graphics)
{
    if (!life3 || !life2 || !life1) return;

    PlayerCar* player = GameManager::Get().GetMainPlayer();
    if (!player) return;

    int life = player->GetLife();

    Gdiplus::Bitmap* img = nullptr;
    if (life >= 3) img = life3;
    else if (life == 2) img = life2;
    else if (life == 1) img = life1;

    const float scale = 3.0f;

    if (img)
    {
        graphics->DrawImage(
            img,
            10,  
            10,  
            img->GetWidth() * static_cast<int>(scale),
            img->GetHeight() * static_cast<int>(scale)
        );
    }
}
