#pragma once
#include "Singleton.h"
#include "CommonTypes.h"
#include <string>
#include <vector>
#include <map>

// �� ������ �����ϴ� ����ü
typedef struct tagMapData
{
    string name;           
    int width;                 
    int height;                
    vector<int> tiles;     
    map<LPCWCH, Texture> textureMap; // �� �ؽ�ó ��
} MapData;

class MapManager : public Singleton<MapManager>
{
private:
    map<string, MapData> mapDataList;  // �پ��� ���� ����
    string currentMapName;                 // ���� Ȱ��ȭ�� �� �̸�

public:
    HRESULT Init();
    void Release();

    // �� �ε�/���� �Լ�
    bool LoadMapFromFile(const std::string& mapName, const wchar_t* filePath);
    bool SaveMapToFile(const std::string& mapName, const wchar_t* filePath);

    // �� ���� �Լ�
    bool CreateEmptyMap(const std::string& mapName, int width, int height);
    bool SetCurrentMap(const std::string& mapName);

    // �� ���� �Լ�
    const MapData* GetMapData(const std::string& mapName) const;
    const MapData* GetCurrentMapData() const;
    int* GetMapTiles(const std::string& mapName); // RayCasting���� ȣȯ�� ���� �Լ�
    std::string GetCurrentMapName() const { return currentMapName; }

    // �� �Ӽ� ���� �Լ�
    int GetMapWidth(const std::string& mapName) const;
    int GetMapHeight(const std::string& mapName) const;
};