#pragma once
#include "Singleton.h"
#include "config.h"
#include <vector>

using namespace std;
class Texture;
class Player;
class Monster;
class MonsterManager : public Singleton<MonsterManager>
{
private:
	Texture* monsterTexture;
	Player* playerPos;
	vector<Monster*> monsters;
	bool isCatchPlayer;

public:
	//TODO: �� ã�� �˰���
	void FindPlayer(Player* playerPos);

	//TODO: �ؽ�ó ����?
	void SetMonsterTexture(vector<Monster*> monsters);

	//TODO: ���� �÷��̾ ����� ���� �޼ҵ�
	void CatchPlayer();

	//TODO:
	void SetisCatchPlayer(bool SetisCatchPlayer) { this->isCatchPlayer = isCatchPlayer; }
	bool GetisCatchPlayer() { return this->isCatchPlayer; }
};

