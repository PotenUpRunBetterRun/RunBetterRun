#pragma once
#include "Singleton.h"
#include "config.h"
#include <functional>

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
	void UpdateFOV();

	FPOINT GetCameraPos() { return cameraPos; }
	FPOINT GetCameraVerDir() { return cameraVerDir; }
	FPOINT GetPlane() { return plane; }

	// ī�޶� ����


private:
	FPOINT cameraPos;                         // ī�޶� ��ġ
	FPOINT cameraVerDir;                      // ī�޶� ����
	FPOINT cameraHorDir;                      // ī�޶� ���� ����
	FPOINT plane;                             // ī�޶� ���

	FPOINT moveInput;                         // ī�޶� �̵� ����
	FLOAT moveSpeed;                          // ī�޶� �̵� �ӵ�
	FLOAT runSpeed;
	FLOAT defaultSpeed;

	FPOINT rotate;                            // ī�޶� ȸ��
	FLOAT rotateSpeed;                        // ī�޶� ȸ�� �ӵ�

	float fov;                                // �þ߰�
} Camera;


