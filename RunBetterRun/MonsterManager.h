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
	FPOINT playerPos;
	vector<Monster*> monsters;
	bool isCatchPlayer;
	MapData* mapData;

public:
	HRESULT Init();
	void Release();
	void Update();
	//TODO: �� ã�� �˰���
	void FindPlayer();


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
};

