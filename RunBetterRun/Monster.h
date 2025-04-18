#pragma once
#include "GameObject.h"
#include "structs.h"
#include <vector>

class Monster : public GameObject
{
private:
	
	// �� �߰��Ұ�?
	FPOINT position;
	float speed;
	bool isActive;
	FPOINT targetPosition;
	bool isMoving;

public:
	virtual HRESULT Init(FPOINT startPos, float speed);		// ��� ������ �ʱ�ȭ, �޸� �Ҵ�
	virtual void Release();		// �޸� ����
	virtual void Update();		// ������ ������ ���� ���� ����(������ ���)
	virtual void Render(HDC hdc);	// ������ ������ ���(�̹���, �ؽ�Ʈ ��)

	FPOINT GetPostion() { return position; }
	void SetPosition(FPOINT pos) { this->position = pos; }
	bool GetIsActive() { return isActive; }
	void SetActive(bool active) { active = isActive; }
	float GetSpeed() { return speed; }

	void SetTargetPosition(FPOINT target) { targetPosition = target; isMoving = true; }
	FPOINT GetTargetPosition() const { return targetPosition; }
	bool IsMoving() const { return isMoving; }
	void SetMoving(bool moving) { isMoving = moving; }
};

