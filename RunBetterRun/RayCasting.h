#pragma once
#include "GameObject.h"
#include <queue>

#define CEILING_COLOR 0x383838       // õ�� ����
#define FLOOR_COLOR 0x717171         // �ٴ� ����

#define MAP_COLUME 24                // �� ���� Ÿ�� ��
#define MAP_ROW 24                   // �� ���� Ÿ�� ��
#define MOVE_SPEED 1.2f              // ī�޶� �̵� �ӵ�
#define ROTATE_SPEED 0.8f            // ī�޶� ȸ�� �ӵ�
#define SHADE_VALUE 1.5f             // �Ÿ� ���� ���̵� ���

#define FLOAT(n) static_cast<float>(n)  // float ĳ���� ���� ��ũ��
#define INT(n) static_cast<int>(n)      // int ĳ���� ���� ��ũ��

#define TILE_ROW_SIZE 20             // �ؽ�ó Ÿ�� �� ��
#define TILE_COLUME_SIZE 9           // �ؽ�ó Ÿ�� �� ��
#define TILE_SIZE 32                 // Ÿ�� �ϳ��� �ȼ� ũ��

#define THREAD_NUM 4                 // �������� ������ ����

class RayCasting;

// ������ ���� ����ü
typedef struct tagThreadData {
	RayCasting* pThis;          // RayCasting �ν��Ͻ� ������
	BOOL			exit;           // ������ ���� ����
	BOOL			done;           // ������ �Ϸ� ����
	queue<POINT>* queue;          // �������� �� ���� ť
	LPHANDLE		threadMutex;    // ������ ����ȭ�� ���ؽ�
	LPHANDLE		queueMutex;     // ť ����ȭ�� ���ؽ�
} ThreadData;

// Ray(����) ����ü
typedef struct tagRay
{
	int			column;         // ������ ����� ȭ���� ��
	int			row;            // �� ��ǥ
	float		distance;       // �浹 ���������� �Ÿ�
	int			side;           // ���� ���� (x �Ǵ� y��)
	int			height;         // �� ����
	FPOINT		ray_pos;        // ���� ���� ��ġ
	FPOINT		ray_dir;        // ���� ����
	FPOINT		map_pos;        // Ÿ�� ��ǥ
	FPOINT		side_dist;      // ���� x, y ������ �Ÿ�
	FPOINT		delta_dist;     // x, y �̵� �� �Ÿ� ����
	FPOINT		step;           // �̵� ����
	float		wall_x;         // �� �浹 ������ x ��ǥ
	FPOINT		floor_wall;     // �ٴ� ���� ��ǥ
	FPOINT		c_floor;        // ������ �ٴ� ��ǥ

	tagRay(FPOINT pos, FPOINT plane, FPOINT cameraDir, float cameraX);
} Ray;


class RayCasting: public GameObject
{
	// ��Ƽ������ ����
	HANDLE			threads[THREAD_NUM];            // ������ �ڵ�
	ThreadData		threadDatas[THREAD_NUM];        // �� ������� ������
	HANDLE			threadMutex[THREAD_NUM];        // �����庰 ���ؽ�
	LONG			colsPerThread;                  // �� �����尡 ó���� �� ��
	queue<POINT>	threadQueue;                    // �� �۾� ť
	HANDLE			queueMutex;                     // ť ���� ����ȭ

	// ������ ����
	BITMAPINFO		bmi;                            // ȭ�� ��¿� ��Ʈ�� ����
	BYTE			pixelData[WINSIZE_X * WINSIZE_Y * 3]; // �ȼ� ���� (RGB)

	vector<COLORREF>textureData;                    // Ÿ�� �ؽ�ó ������
	DWORD			tileWidth, tileHeight;          // �ؽ�ó�� ���� ����
	DWORD			tileRowSize;                    // �ؽ�ó �� ����

	BOOL			changeScreen;                   // ȭ�� ���� ����

	// ī�޶� �� ������
	int     renderScale;               // �ػ� ����
	int     currentFPS;                // FPS ����
	int     fpsCheckCounter;          // FPS ���� ī����
	float   fpsCheckTime;             // FPS ���� �ð�

	float	fov;                      // �þ߰�(FOV)
	float	rotateSpeed;             // ȸ�� �ӵ�
	float	moveSpeed;               // �̵� �ӵ�

	FPOINT	cameraPos;               // ī�޶� ��ġ
	FPOINT	cameraDir;               // ī�޶� ����
	FPOINT	cameraXDir;              // x�� ī�޶� ����
	FPOINT	plane;                   // ī�޶� ��� (�þ� ���� ����)

	FPOINT	move;                    // ����/���� �̵� ����
	FPOINT	x_move;                  // �¿� �̵� ����
	FPOINT	rotate;                  // ȸ�� ����

	static int map[MAP_ROW * MAP_COLUME];   // ���� �� ������
	float	camera_x[WINSIZE_X];            // ȭ�� ���� ���� ī�޶� X ��ǥ
	float	depth[WINSIZE_X];               // �Ÿ� ���� (z-buffer)
	float	sf_dist[WINSIZE_Y];             // õ��/�ٴ� ���� �Ÿ�
 
	void KeyInput(void);                            // Ű �Է� ó��
	void MoveCamera(float deltaTime);               // ����/���� �̵�
	void MoveSideCamera(float deltaTime);           // �¿� �̵�
	void RotateCamera(float deltaTime);             // ī�޶� ȸ��

	Ray RayCast(int colume);                        // �ϳ��� ������ ���� �߻�
	void RenderWall(Ray& ray, int colume);          // �� ������
	void RenderCeilingFloor(Ray& ray, int colume);  // õ��/�ٴ� ������
	void RenderCeilingFloor(Ray& ray, int colume, COLORREF ceiling, COLORREF floor); // ���� ���� õ��/�ٴ� ������
	void RenderPixel(FPOINT pixel, int color);      // ȭ�� �ȼ� �׸���

	COLORREF GetDistanceShadeColor(int tile, FPOINT texturePixel, float distance); // �Ÿ� ��� ���̵� (Ÿ�Ͽ�)
	COLORREF GetDistanceShadeColor(COLORREF color, float distance);               // �Ÿ� ��� ���̵� (����)

	void LoadTextureTiles(LPCWCH path);             // �ؽ�ó �ε�
	int GetRenderScaleBasedOnFPS(void);             // FPS ��� ������ ����

public:
	virtual HRESULT Init(void) override;
	virtual void Release(void) override;
	virtual void Update(void) override;
	virtual void Render(HDC hdc) override;

	// ��ƿ��Ƽ
	void FillScreen(DWORD start, DWORD end); // Ư�� ���� ���� ä���
};

