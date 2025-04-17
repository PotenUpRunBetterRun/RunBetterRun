#pragma once
#include "Singleton.h"
#include "structs.h"

#define MAP_COLUME 24
#define MAP_ROW 24

class MapManager : public Singleton<MapManager>
{
private:
	MapData mapData;

public:
	HRESULT Init();					//�⺻	   �� ����
	HRESULT Init(LPCWCH filePath);  //���ϰ�� �� �ε�
	void Release();

	bool LoadMap(const LPCWCH filePath);
	bool SaveMap(const LPCWCH filePath);
	
	bool CreateNewMap(int width, int height); //���ο� �� ����
	bool CreateMazeMap(int width, int height);
	void SetTile(int x, int y, RoomType tileType, int index); //Ư��Ÿ���� Ÿ�� ����

	MapData* GetMapData();
};