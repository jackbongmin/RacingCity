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

	bool LoadResourceFromFile(ResourceID InID, const wchar_t* path);	// ���Ͽ��� ���ҽ��� �ε��ϴ� �Լ�

	std::unordered_map<ResourceID, Gdiplus::Bitmap*> Resources;	// ��� ���ҽ��� �� ����ִ� ��
};

