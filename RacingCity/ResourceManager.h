#pragma once
#include <unordered_map>
#include "Common.h"
#include "Singleton.h"

class ResourceManager : public Singleton<ResourceManager>
{
	friend class Singleton<ResourceManager>;

public:
	void Initialize();
	void Destroy();

	Gdiplus::Bitmap* GetImage(ResourceID InID);

private:
	ResourceManager() = default;
	virtual ~ResourceManager() = default;

	bool LoadResourceFromFile(ResourceID InID, const wchar_t* path);	// 파일에서 리소스를 로딩하는 함수

	std::unordered_map<ResourceID, Gdiplus::Bitmap*> Resources;	// 모든 리소스가 다 들어있는 맵
};

