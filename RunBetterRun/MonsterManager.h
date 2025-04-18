#pragma once
#include "Singleton.h"
#include "config.h"
#include "Monster.h"
#include "MapManager.h"
#include <vector>

using namespace std;

class MonsterManager : public Singleton<MonsterManager>
{
private:
	// A* �˰����� ���� ��� ����ü
	struct PathNode {
		int x, y;           // ��ġ
		int g;              // ���������� ���� �������� ���
		int h;              // ���� ��忡�� ��ǥ������ ���� ���
		int f;              // f = g + h
		PathNode* parent;   // �θ� ���

		PathNode(int x, int y) : x(x), y(y), g(0), h(0), f(0), parent(nullptr) {}
	};

private:
	FPOINT playerPos;
	vector<Monster*> monsters;
	bool isCatchPlayer;
	MapData* mapData;

public:
	HRESULT Init();
	void Release();
	void Update();
	//TODO: �� ã�� �˰���
	void FindPlayer(FPOINT monsterPos, FPOINT targetPos, float deletaTime);


	//TODO: ���� �÷��̾ ����� ���� �޼ҵ�
	void CatchPlayer();

	//TODO:
	void SetisCatchPlayer(bool SetisCatchPlayer) { this->isCatchPlayer = isCatchPlayer; }
	bool GetisCatchPlayer() { return this->isCatchPlayer; }

	// ���� ���� �� ����
	Monster* CreateMonster(FPOINT position, float speed = 0.5f);
	void RemoveMonster(int index);
	void RemoveAllMonsters();

	// ������ ��� ã�� (���� �̵�)
	FPOINT GetDirectionToPlayer(const FPOINT& monsterPos);
	bool CanMoveToPosition(const FPOINT& pos);

	// TODO: Astar
	vector<FPOINT> FindPath(FPOINT start, FPOINT end);
	int CalculateHeuristic(int x1, int y1, int x2, int y2);
	bool IsNodeInList(const vector<PathNode*>& list, int x, int y);
	PathNode* GetNodeFromList(vector<PathNode*>& list, int x, int y);
};

