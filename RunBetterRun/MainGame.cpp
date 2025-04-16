#include "MainGame.h"
#include "CommonFunction.h"
#include "Image.h"
#include "Timer.h"
#include "TilemapTool.h"
#include "LoadingScene.h"
#include "RayCasting.h"
#include "SpriteManager.h"
#include "MapManager.h"

HRESULT MainGame::Init()
{
	ImageManager::GetInstance()->Init();
	KeyManager::GetInstance()->Init();
	SceneManager::GetInstance()->Init();
	SpriteManager::GetInstance()->Init();
	MapManager::GetInstance()->Init();

	if (!MapManager::GetInstance()->LoadMap("defaultMap", L"Maps/defaultMap.dat"))
	{
		// �⺻ �� ����
		MapManager::GetInstance()->CreateEmptyMap("defaultMap", MAP_COLUME, MAP_ROW);

		// ���⼭ �⺻ �� �����͸� ������ �� �ֽ��ϴ�
		// ����: �׽�Ʈ �� ����
		for (int y = 0; y < MAP_ROW; y++) {
			for (int x = 0; x < MAP_COLUME; x++) {
				// �����ڸ��� �� ��ġ
				if (x == 0 || y == 0 || x == MAP_COLUME - 1 || y == MAP_ROW - 1) {
					MapManager::GetInstance()->SetTile("defaultMap", x, y, 1);
				}
			}
		}

		// �� ���� ��ֹ� �߰�
		for (int i = 5; i < 10; i++) {
			MapManager::GetInstance()->SetTile("defaultMap", i, 5, 2);
		}

		// �� ����
		MapManager::GetInstance()->SaveMap("defaultMap", L"Maps/defaultMap.dat");
	}

	// ���� ������ ����
	MapManager::GetInstance()->SetCurrentMap("defaultMap");
	SceneManager::GetInstance()->AddScene("Ÿ�ϸ���", new TilemapTool());
	SceneManager::GetInstance()->AddLoadingScene("�ε�_1", new LoadingScene());
	SceneManager::GetInstance()->AddScene("RayCasting", new RayCasting());
	SceneManager::GetInstance()->ChangeScene("RayCasting");

	hdc = GetDC(g_hWnd);

	backBuffer = new Image();
	if (FAILED(backBuffer->Init(TILEMAPTOOL_X, TILEMAPTOOL_Y)))
	{
		MessageBox(g_hWnd, 
			TEXT("����� ���� ����"), TEXT("���"), MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

void MainGame::Release()
{
	if (backBuffer)
	{
		backBuffer->Release();
		delete backBuffer;
		backBuffer = nullptr;
	}

	ReleaseDC(g_hWnd, hdc);

	SpriteManager::GetInstance()->Release();
	SceneManager::GetInstance()->Release();
	KeyManager::GetInstance()->Release();
	ImageManager::GetInstance()->Release();
	MapManager::GetInstance()->Release();
}

void MainGame::Update()
{
	SceneManager::GetInstance()->Update();
	InvalidateRect(g_hWnd, NULL, false);
}

void MainGame::Render()
{
	// ����ۿ� ���� ����
	HDC hBackBufferDC = backBuffer->GetMemDC();

	SceneManager::GetInstance()->Render(hBackBufferDC);

	TimerManager::GetInstance()->Render(hBackBufferDC);
	/*wsprintf(szText, TEXT("Mouse X : %d, Y : %d"), g_ptMouse.x, g_ptMouse.y);
	TextOut(hBackBufferDC, 20, 60, szText, wcslen(szText));*/

	// ����ۿ� �ִ� ������ ���� hdc�� ����
	backBuffer->Render(hdc);
}

LRESULT MainGame::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_KEYDOWN:
		break;
	case WM_LBUTTONDOWN:
		g_ptMouse.x = LOWORD(lParam);
		g_ptMouse.y = HIWORD(lParam);
		break;
	case WM_LBUTTONUP:
		break;
	case WM_MOUSEMOVE:
		g_ptMouse.x = LOWORD(lParam);
		g_ptMouse.y = HIWORD(lParam);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}

MainGame::MainGame()
{
}

MainGame::~MainGame()
{
}
