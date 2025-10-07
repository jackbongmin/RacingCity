#include "Background.h"
#include "GameManager.h"

Background::Background(ResourceID InID)
    :Actor(InID)
{
    Pivot.X = 0;    // 왼쪽 위가 피봇
    Pivot.Y = 0;
}

void Background::OnTick(float InDeltaTime)
{
    Offset += ScrollSpeed * InDeltaTime;

    if (Offset > GameManager::ScreenWidth)
    {
        Offset = 0.0f;
    }
}

void Background::OnRender(Gdiplus::Graphics* InGraphics)
{

    if (!Image) return;

    const int ImgW = Image->GetWidth();
    const int ImgH = Image->GetHeight();

    float Scale = static_cast<float>(GameManager::ScreenHeight) / ImgH; // 세로기준으로 자동 스케일 계산

    for (int x = 0; x < GameManager::ScreenWidth / (ImgW * Scale) + 2; ++x)
    {
        float drawX = -Offset + (x * ImgW * Scale);
        InGraphics->DrawImage(
            Image,
            static_cast<int>(drawX),
            0,
            static_cast<int>(ImgW * Scale),
            static_cast<int>(ImgH * Scale));
    }

    //// 맵 하나만 그리되, 오른쪽으로 넘어갈 부분을 미리 이어서 그림
    //for (int x = 0; x < GameManager::ScreenWidth / ImgW + 2; ++x)
    //{
    //    float drawX = -Offset + (x * ImgW);
    //    InGraphics->DrawImage(
    //        Image,
    //        static_cast<int>(drawX),
    //        0,              // Y는 항상 0 ? 위아래로 반복하지 않음
    //        ImgW,
    //        ImgH);
    //}
}