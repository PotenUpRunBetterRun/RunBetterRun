#pragma once
#include "GameObject.h"
#include <vector>

class Image;
class Monster : public GameObject
{

	static std::vector<COLORREF> monsterTextureData;
	static int monsterWidth, mosteHeight;

public:
	virtual HRESULT Init();		// ��� ������ �ʱ�ȭ, �޸� �Ҵ�
	virtual void Release();		// �޸� ����
	virtual void Update();		// ������ ������ ���� ���� ����(������ ���)
	virtual void Render(HDC hdc);	// ������ ������ ���(�̹���, �ؽ�Ʈ ��)
};

