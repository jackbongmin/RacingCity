#include "Background.h"
#include "GameManager.h"

Background::Background(ResourceID InID)
    :Actor(InID)
{
    Pivot.X = 0;    // ���� ���� �Ǻ�
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

    float Scale = static_cast<float>(GameManager::ScreenHeight) / ImgH; // ���α������� �ڵ� ������ ���

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

    //// �� �ϳ��� �׸���, ���������� �Ѿ �κ��� �̸� �̾ �׸�
    //for (int x = 0; x < GameManager::ScreenWidth / ImgW + 2; ++x)
    //{
    //    float drawX = -Offset + (x * ImgW);
    //    InGraphics->DrawImage(
    //        Image,
    //        static_cast<int>(drawX),
    //        0,              // Y�� �׻� 0 ? ���Ʒ��� �ݺ����� ����
    //        ImgW,
    //        ImgH);
    //}
}