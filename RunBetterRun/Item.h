#pragma once
#include "GameObject.h"
#include <vector>

class Sprite;

enum class ItemType
{
	NONE,
	ITEM
};

class Item : public GameObject
{
private:
	Sprite* itemSprite;
	ItemType itemType;

public:
	virtual HRESULT Init();		// ��� ������ �ʱ�ȭ, �޸� �Ҵ�
	virtual void Release();		// �޸� ����
	virtual void Update();		// ������ ������ ���� ���� ����(������ ���)
	virtual void Render(HDC hdc);	// ������ ������ ���(�̹���, �ؽ�Ʈ ��)
};

