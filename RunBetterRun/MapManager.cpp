#include "MapManager.h"

HRESULT MapManager::Init()
{
	// 24x24 ũ�� �⺻ �� ����
	//return CreateNewMap(MAP_COLUME, MAP_ROW) ? S_OK : E_FAIL;
    return CreateMazeMap(MAP_COLUME, MAP_ROW) ? S_OK : E_FAIL;
}

HRESULT MapManager::Init(LPCWCH filePath)
{
    if (LoadMap(filePath))
    {
        return S_OK;  
    }

    // �ε� ���� �� �⺻ �� ����
    //return CreateNewMap(MAP_COLUME, MAP_ROW) ? S_OK : E_FAIL;
    return CreateMazeMap(MAP_COLUME, MAP_ROW) ? S_OK : E_FAIL;
}

void MapManager::Release()
{
	mapData.tiles.clear();
}

bool MapManager::LoadMap(const LPCWCH filePath)
{
    HANDLE hFile = CreateFile(
        filePath,           
        GENERIC_READ,       
        0,                  
        NULL,               
        OPEN_EXISTING,     
        FILE_ATTRIBUTE_NORMAL, 
        NULL                
    );

    if (hFile == INVALID_HANDLE_VALUE)
    {
        return false;  // ���� ���� ����
    }

    DWORD bytesRead = 0;

    // �� �ʺ� �б�
    int width;
    ReadFile(hFile, &width, sizeof(int), &bytesRead, NULL);
    if (bytesRead != sizeof(int))
    {
        CloseHandle(hFile);
        return false;
    }

    // �� ���� �б�
    int height;
    ReadFile(hFile, &height, sizeof(int), &bytesRead, NULL);
    if (bytesRead != sizeof(int))
    {
        CloseHandle(hFile);
        return false;
    }

    // Ÿ�� ���� �б�
    int tileCount;
    ReadFile(hFile, &tileCount, sizeof(int), &bytesRead, NULL);
    if (bytesRead != sizeof(int))
    {
        CloseHandle(hFile);
        return false;
    }

    // ������ ��ȿ�� �˻�
    if (width <= 0 || height <= 0 || tileCount != width * height)
    {
        CloseHandle(hFile);
        return false;  
    }

    // �� ������ �ʱ�ȭ
    mapData.width = width;
    mapData.height = height;
    mapData.tiles.resize(tileCount);

    // Ÿ�� ������ �б�
    ReadFile(hFile, mapData.tiles.data(), sizeof(Room) * tileCount, &bytesRead, NULL);
    if (bytesRead != sizeof(Room) * tileCount)
    {
        CloseHandle(hFile);
        return false;
    }

    CloseHandle(hFile);
    return true;  
}

bool MapManager::SaveMap(const LPCWCH filePath)
{
    if (mapData.tiles.empty())
    {
        return false; 
    }

    // ���� ����
    HANDLE hFile = CreateFile(
        filePath,           
        GENERIC_WRITE,     
        0,                  
        NULL,              
        CREATE_ALWAYS,      
        FILE_ATTRIBUTE_NORMAL, 
        NULL                
    );

    // ���� ���� ���� Ȯ��
    if (hFile == INVALID_HANDLE_VALUE)
    {
        return false;  
    }

    DWORD bytesWritten = 0;

    // �� �ʺ� ����
    WriteFile(hFile, &mapData.width, sizeof(int), &bytesWritten, NULL);

    // �� ���� ����
    WriteFile(hFile, &mapData.height, sizeof(int), &bytesWritten, NULL);

    // Ÿ�� ���� ����
    int tileCount = mapData.tiles.size();
    WriteFile(hFile, &tileCount, sizeof(int), &bytesWritten, NULL);

    // Ÿ�� ������ ����
    WriteFile(hFile, mapData.tiles.data(), sizeof(Room) * tileCount, &bytesWritten, NULL);

    // ���� �ڵ� �ݱ�
    CloseHandle(hFile);
    return true; 
}

void MapManager::GenerateMaze(int startX, int startY, int width, int height)
{
    // �湮 ���θ� �����ϴ� �ӽ� �迭
    vector<vector<bool>> visited(height, vector<bool>(width, false));

    // ���� ���� (��, ��, ��, ��)
    int dx[] = { 0, 0, -2, 2 };
    int dy[] = { -2, 2, 0, 0 };

    // ������ �̿��� ���� �켱 Ž��
    stack<pair<int, int>> cellStack;

    // ������
    SetTile(startX, startY, RoomType::FLOOR, 10);
    visited[startY][startX] = true;
    cellStack.push(make_pair(startX, startY));

    // ���� �õ� ����
    srand(static_cast<unsigned int>(time(NULL)));

    // �̷� ����
    while (!cellStack.empty())
    {
        // ���� ��ġ
        int x = cellStack.top().first;
        int y = cellStack.top().second;

        // ���� �迭
        int directions[4] = { 0, 1, 2, 3 };
        ShuffleDirections(directions);

        bool foundNext = false;

        // 4���� �õ�
        for (int i = 0; i < 4; i++)
        {
            int dir = directions[i];
            int nx = x + dx[dir];
            int ny = y + dy[dir];

            // �� ��� �� �湮 ���� Ȯ��
            if (nx > 0 && nx < width - 1 && ny > 0 && ny < height - 1 && !visited[ny][nx])
            {
                // �߰� �� ���� (�� �����)
                SetTile(x + dx[dir] / 2, y + dy[dir] / 2, RoomType::FLOOR, 10);

                // ���� ��ġ�� �̵�
                SetTile(nx, ny, RoomType::FLOOR, 10);
                visited[ny][nx] = true;

                // ���ÿ� �߰�
                cellStack.push(make_pair(nx, ny));
                foundNext = true;
                break;
            }
        }

        // �� �̻� �̵��� �� ������ ���ÿ��� ����
        if (!foundNext)
        {
            cellStack.pop();
        }
    }
}

void MapManager::ShuffleDirections(int directions[4])
{
    // Fisher-Yates ���� �˰���
    for (int i = 3; i > 0; i--)
    {
        int j = rand() % (i + 1);

        int temp = directions[i];
        directions[i] = directions[j];
        directions[j] = temp;
    }
}

bool MapManager::CreateMazeMap(int width, int height)
{
    // ���� ���� ũ�Ⱑ Ȧ������ �̷ΰ� �� �� ������
    if (width % 2 == 0) width++;
    if (height % 2 == 0) height++;

    // �� ũ��� �ּ� 5x5
    if (width < 5) width = 5;
    if (height < 5) height = 5;

    // �� ���� (��� Ÿ���� ������ ����)
    CreateNewMap(width, height);

    // ��� Ÿ���� ������ ����
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            mapData.tiles[y * width + x].roomType = RoomType::WALL;
            mapData.tiles[y * width + x].tilePos = 4;
        }
    }

    // �̷� ���� ���� (1, 1���� ����)
    GenerateMaze(1, 1, width, height);

    // ���� ��ġ ���� (�̷� �߾� ��ó)
    SetTile(width / 2, height / 2, RoomType::START, 3);

    return true;
}

bool MapManager::CreateNewMap(int width, int height)
{
    if (width <= 0 || height <= 0)
    {
        return false;  
    }

    // �� ������ �ʱ�ȭ
    mapData.width = width;
    mapData.height = height;
    mapData.tiles.resize(width * height);

    // �� Ÿ�� �ʱ�ȭ
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            // �� �����ڸ��� ������ ����
            if (x == 0 || x == width - 1 || y == 0 || y == height - 1)
            {
                // �� Ÿ�� (�ε��� 4)
                mapData.tiles[y * width + x].roomType = RoomType::WALL;
                mapData.tiles[y * width + x].tilePos = 4;
            }
            else
            {
                // ���δ� �ٴ����� ���� (�ε��� 10)
                mapData.tiles[y * width + x].roomType = RoomType::FLOOR;
                mapData.tiles[y * width + x].tilePos = 10;
            }
        }
    }

    // ���� ��ġ ���� (�߾� ��ó)
    int startX = width / 2;
    int startY = height / 2;
    mapData.tiles[startY * width + startX].roomType = RoomType::START;
    mapData.tiles[startY * width + startX].tilePos = 3;

    return true;  
}

MapData* MapManager::GetMapData()
{
	return &mapData;
}

void MapManager::SetTile(int x, int y, RoomType tileType, int index)
{
    if (x >= 0 && x < mapData.width && y >= 0 && y < mapData.height)
    {
        // Ÿ�� ���� ����
        mapData.tiles[y * mapData.width + x].roomType = tileType;
        mapData.tiles[y * mapData.width + x].tilePos = index;
    }
}