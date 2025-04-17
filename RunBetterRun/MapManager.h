#pragma once
#include "Singleton.h"
#include "structs.h"
#include <stack>
#include <algorithm>

#define MAP_COLUME 24
#define MAP_ROW 24

class MapManager : public Singleton<MapManager>
{
private:
	MapData mapData;
	void GenerateMaze(int startX, int startY, int width, int height); //�̷� �����Ҷ� �� �Լ�
	void ShuffleDirections(int directions[4]);
public:
	HRESULT Init();					//�⺻	   �� ����
	HRESULT Init(LPCWCH filePath);  //���ϰ�� �� �ε�
	void Release();

	bool LoadMap(const LPCWCH filePath); 
	bool SaveMap(const LPCWCH filePath);
	
	bool CreateNewMap(int width, int height); //���ο� �� ����
	bool CreateMazeMap(int width, int height); //�̷� �� ����
	bool CreateSimpleMazeMap(int width, int height);
	bool CreateFpsStyleMap(int width, int height);  // FPS ��Ÿ�� �� ����
	void SetTile(int x, int y, RoomType tileType, int index); //Ư��Ÿ���� Ÿ�� ����

	MapData* GetMapData();
};