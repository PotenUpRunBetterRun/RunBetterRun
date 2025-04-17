#pragma once
#include "Singleton.h"
#include "config.h"

typedef class Player : public Singleton<Player>
{
public:
	virtual HRESULT Init();                   // ��� ������ �ʱ�ȭ, �޸� �Ҵ�
	virtual void Release();                   // �޸� ����
	virtual void Update();                    // ������ ������ ���� ���� ����(������ ���)
	virtual void Render(HDC hdc);             // ������ ������ ���(�̹���, �ؽ�Ʈ ��)
	void KeyInput(void);
	void MouseInput(void);
	void MoveCamera(float deltaTime);
	void RotateCamera(float deltaTime);

	FPOINT GetCameraPos() { return cameraPos; }
	FPOINT GetCameraVerDir() { return cameraVerDir; }
	FPOINT GetPlane() { return plane; }

private:
	FPOINT cameraPos;                         // ī�޶� ��ġ
	FPOINT cameraVerDir;                      // ī�޶� ����
	FPOINT cameraHorDir;                      // ī�޶� ���� ����
	FPOINT plane;                             // ī�޶� ���
	FPOINT moveInput;                         // ī�޶� �̵� ����
	FPOINT rotate;                            // ī�޶� ȸ��
	FLOAT moveSpeed;                          // ī�޶� �̵� �ӵ�
	FLOAT rotateSpeed;                        // ī�޶� ȸ�� �ӵ�
	float fov;                                // �þ߰�

	bool isShowMouse;
} Camera;


