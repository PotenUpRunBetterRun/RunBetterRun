#include "MonsterManager.h"
#include "SpriteManager.h"
#include "Player.h"
#include "MapManager.h"
#include <cmath>

HRESULT MonsterManager::Init()
{
	playerPos = Player::GetInstance()->GetCameraPos();
	mapData = MapManager::GetInstance()->GetMapData();
	isCatchPlayer = false;

	Texture monsterTexture;
	SpriteManager::GetInstance()->LoadTexture(TEXT("Image/Rocket.bmp"), monsterTexture);

	CreateMonster({ 10.0f, 10.0f }, 30.0f);
	CreateMonster({ 20.0f,  10.0f}, 30.0f);

	// ��� ��������Ʈ ���
	for (auto& monster : monsters) {
		if (monster->GetIsActive()) {
			SpriteManager::GetInstance()->
				PutSprite(
					TEXT("Image/Rocket.bmp"), 
					monster->GetPostion()
				);
		}
	}

	return S_OK;
}

void MonsterManager::Release()
{
	for (auto& monster : monsters)
	{
		monster->Release();
		delete monster;
	}
	monsters.clear();
}

void MonsterManager::Update()
{
    // �÷��̾� ��ġ ������Ʈ
    playerPos = Player::GetInstance()->GetCameraPos();
    float deltaTime = TimerManager::GetInstance()->GetDeltaTime();

    static float pathUpdateTimer = 0.0f;
    pathUpdateTimer += deltaTime;

    if (pathUpdateTimer >= 0.2f) {
        pathUpdateTimer = 0.0f;

        // �� ���� ������Ʈ
        for (auto& monster : monsters)
        {
            if (!monster->GetIsActive()) continue;


            // �� ������ ��� ���
            for (auto& monster : monsters) {
                if (!monster->GetIsActive()) continue;

                FPOINT monsterPos = monster->GetPostion();
                vector<FPOINT> path = FindPath(monsterPos, playerPos);

                if (path.size() >= 2) {
                    // ���� ��ġ�� ��ǥ ��ġ�� ����
                    monster->SetTargetPosition(path[1]);
                }
            }
        }

        // �� ���� �̵� ���� ������Ʈ
        for (auto& monster : monsters) {
            if (!monster->GetIsActive() || !monster->IsMoving()) continue;

            FPOINT currentPos = monster->GetPostion();
            FPOINT targetPos = monster->GetTargetPosition();

            // ���� ��ġ�� ��ǥ ��ġ ���� �Ÿ�
            float dx = targetPos.x - currentPos.x;
            float dy = targetPos.y - currentPos.y;
            float distance = sqrt(dx * dx + dy * dy);

            if (distance < 0.05f) {
                // ��ǥ�� ���� ���������� ��Ȯ�� ��ġ�� ����
                monster->SetPosition(targetPos);
                monster->SetMoving(false);
            }
            else {
                // ��ǥ�� ���� �̵�
                float dirX = dx / distance;
                float dirY = dy / distance;

                // �� ��ġ ��� (��Ÿ Ÿ������ �ε巯�� �̵�)
                FPOINT newPos = {
                    currentPos.x + dirX * monster->GetSpeed() * deltaTime,
                    currentPos.y + dirY * monster->GetSpeed() * deltaTime
                };

                // �̵� ���� ���� Ȯ��
                if (CanMoveToPosition(newPos)) {
                    monster->SetPosition(newPos);

                    // ��������Ʈ ������Ʈ
                    SpriteManager::GetInstance()->UpdateMonsterPosition(
                        TEXT("Image/Rocket.bmp"),
                        monster->GetPostion());
                }
                else {
                    // ��ΰ� �������� �̵� ���� �� ���� ������Ʈ���� ��� ����
                    monster->SetMoving(false);
                    pathUpdateTimer = 0.2f; // ��� ��� ����
                }
            }

        }
    }
}

void MonsterManager::FindPlayer(FPOINT monsterPos, FPOINT targetPos, float deltaTime)
{
	for (auto& monster : monsters)
	{
		if (!monster->GetIsActive()) continue;

		FPOINT monsterPos = monster->GetPostion();

		vector<FPOINT> path = FindPath(monsterPos, playerPos);

		if (path.size() >= 2)
		{
			// ���� ��ġ�� ����
			monster->SetPosition(path[1]);

			// ��������Ʈ ������Ʈ
			SpriteManager::GetInstance()->UpdateMonsterPosition(
				TEXT("Image/Rocket.bmp"),
				monster->GetPostion());
		}

	}
}


void MonsterManager::CatchPlayer()
{
}

Monster* MonsterManager::CreateMonster(FPOINT position, float speed)
{
	Monster* monster = new Monster();
	monster->Init(position, speed);
	monsters.push_back(monster);
	return monster;
}

void MonsterManager::RemoveMonster(int index)
{
	//TODO
}

void MonsterManager::RemoveAllMonsters()
{
	//TODO
}

FPOINT MonsterManager::GetDirectionToPlayer(const FPOINT& monsterPos)
{
	FPOINT direction = {
		playerPos.x - monsterPos.x,
		playerPos.y - monsterPos.y
	};

	// ����ȭ (���� ���ͷ� ��ȯ)
	float length = sqrt(direction.x * direction.x + direction.y * direction.y);
	if (length > 0.001f) {  // 0���� ������ ����
		direction.x /= length;
		direction.y /= length;
	}

	return direction;
}

bool MonsterManager::CanMoveToPosition(const FPOINT& pos)
{
	// �� ��� Ȯ��
	int x = static_cast<int>(pos.x);
	int y = static_cast<int>(pos.y);

	if (x < 0 || x >= MAP_COLUME || y < 0 || y >= MAP_ROW)
		return false;

	// Ÿ�� Ÿ�� Ȯ�� (�ٴ��� ��츸 �̵� ����)
	if (!mapData) return false;

	Room& tile = mapData->tiles[y * mapData->width + x];
	return (tile.roomType == RoomType::FLOOR || tile.roomType == RoomType::START);
}

vector<FPOINT> MonsterManager::FindPath(FPOINT start, FPOINT end)
{
    // ��� ��� ����
    vector<FPOINT> path;

    // �� ������ Ȯ��
    if (!mapData) return path;

    // �������� ��ǥ���� ���� ��ǥ
    int startX = static_cast<int>(start.x);
    int startY = static_cast<int>(start.y);
    int endX = static_cast<int>(end.x);
    int endY = static_cast<int>(end.y);

    // �������� ��ǥ���� �� ������ ������� Ȯ��
    if (startX < 0 || startX >= mapData->width || startY < 0 || startY >= mapData->height ||
        endX < 0 || endX >= mapData->width || endY < 0 || endY >= mapData->height)
        return path;

    // �������̳� ��ǥ���� ���� ��� ó��
    if (mapData->tiles[startY * mapData->width + startX].roomType == RoomType::WALL ||
        mapData->tiles[endY * mapData->width + endX].roomType == RoomType::WALL)
        return path;

    // ���� ��ϰ� ���� ��� �ʱ�ȭ
    vector<PathNode*> openList;
    vector<PathNode*> closedList;

    // ���� ��� ����
    PathNode* startNode = new PathNode(startX, startY);
    startNode->h = CalculateHeuristic(startX, startY, endX, endY);
    startNode->f = startNode->h;

    // ���� ��Ͽ� ���� ��� �߰�
    openList.push_back(startNode);

    // �����¿� �̵� ���� (���� �̵���)
    const int dx[4] = { 0, 1, 0, -1 };
    const int dy[4] = { -1, 0, 1, 0 };

    // A* �˰��� ���� ����
    while (!openList.empty())
    {
        // f���� ���� ���� ��� ã��
        auto it = min_element(openList.begin(), openList.end(), [](PathNode* a, PathNode* b) {
            return a->f < b->f;
            });

        PathNode* current = *it;

        // ��ǥ ���� Ȯ��
        if (current->x == endX && current->y == endY)
        {
            // ��� ������
            PathNode* temp = current;
            while (temp)
            {
                path.push_back({ static_cast<float>(temp->x) + 0.5f, static_cast<float>(temp->y) + 0.5f });
                temp = temp->parent;
            }

            // ��� ���� ������
            reverse(path.begin(), path.end());

            // �޸� ����
            for (auto node : openList)
                delete node;
            for (auto node : closedList)
                delete node;

            // ��� ��ȯ
            return path;
        }

        // ���� ��带 ���� ��Ͽ��� �����ϰ� ���� ��Ͽ� �߰�
        openList.erase(it);
        closedList.push_back(current);

        // �����¿� �̿� ��� ó��
        for (int i = 0; i < 4; i++)
        {
            int nx = current->x + dx[i];
            int ny = current->y + dy[i];

            // �� ��� Ȯ��
            if (nx < 0 || nx >= mapData->width || ny < 0 || ny >= mapData->height)
                continue;

            // �̵� ������ Ÿ������ Ȯ�� (FLOOR �Ǵ� START)
            Room& tile = mapData->tiles[ny * mapData->width + nx];
            if (tile.roomType != RoomType::FLOOR && tile.roomType != RoomType::START)
                continue;

            // �̹� ���� ��Ͽ� �ִ��� Ȯ��
            if (IsNodeInList(closedList, nx, ny))
                continue;

            // �� g�� ��� (���� �̵��� ��� 1)
            int newG = current->g + 1;

            // �̹� ���� ��Ͽ� �ִ��� Ȯ��
            PathNode* neighbor = GetNodeFromList(openList, nx, ny);

            if (!neighbor)
            {
                // �� ��� ����
                neighbor = new PathNode(nx, ny);
                neighbor->g = newG;
                neighbor->h = CalculateHeuristic(nx, ny, endX, endY);
                neighbor->f = neighbor->g + neighbor->h;
                neighbor->parent = current;

                // ���� ��Ͽ� �߰�
                openList.push_back(neighbor);
            }
            else if (newG < neighbor->g)
            {
                // �� ���� ��θ� ã��
                neighbor->g = newG;
                neighbor->f = neighbor->g + neighbor->h;
                neighbor->parent = current;
            }
        }
    }

    // ��θ� ã�� ����, �޸� ����
    for (auto node : openList)
        delete node;
    for (auto node : closedList)
        delete node;

    // �� ��� ��ȯ
    return path;
}

int MonsterManager::CalculateHeuristic(int x1, int y1, int x2, int y2)
{
	return abs(x1 - x2) + abs(y1 - y2);
}

bool MonsterManager::IsNodeInList(const vector<PathNode*>& list, int x, int y)
{
	for (auto node : list)
	{
		if (node->x == x && node->y == y)
			return true;
	}
	return false;
}

MonsterManager::PathNode* MonsterManager::GetNodeFromList(vector<PathNode*>& list, int x, int y)
{
	for (auto node : list)
	{
		if (node->x == x && node->y == y)
			return node;
	}
	return nullptr;
}
