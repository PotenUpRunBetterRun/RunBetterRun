#pragma once
#include "GameObject.h"
#include <vector>

class Sprite;

enum class ItemType
{
	NONE,
	ITEM,
	TRIGGER
};

class Item : public GameObject
{
private:
	Sprite* itemSprite;
	ItemType itemType;

public:
	virtual HRESULT Init();		// 멤버 변수의 초기화, 메모리 할당
	virtual void Release();		// 메모리 해제
	virtual void Update();		// 프레임 단위로 게임 로직 실행(데이터 계산)
	virtual void Render(HDC hdc);	// 프레임 단위로 출력(이미지, 텍스트 등)
};

