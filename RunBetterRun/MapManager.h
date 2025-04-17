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
	
	bool CreateMazeMap(int width, int height); //�̷� �� ����
	bool CreateNewMap(int width, int height); //���ο� �� ����
	void SetTile(int x, int y, RoomType tileType, int index); //Ư��Ÿ���� Ÿ�� ����

	MapData* GetMapData();
};