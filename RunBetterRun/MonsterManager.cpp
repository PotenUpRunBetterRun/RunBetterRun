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

	CreateMonster({ playerPos.x + 2.0f, playerPos.y }, 0.5f);
	CreateMonster({ playerPos.x, playerPos.y + 2.0f }, 0.3f);

	// ��� ��������Ʈ ���
	for (auto& monster : monsters) {
		if (monster->GetIsActive()) {
			SpriteManager::GetInstance()->PutSprite(TEXT("Image/Rocket.bmp"), monster->GetPostion());
			// ������ �α�
			FPOINT pos = monster->GetPostion();
			WCHAR buffer[100];
			swprintf_s(buffer, L"Monster created at: %.2f, %.2f\n", pos.x, pos.y);
			OutputDebugString(buffer);
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

	// �� ���� ������Ʈ
	for (auto& monster : monsters)
	{
		if (!monster->GetIsActive()) continue;

		// �÷��̾� ���� ���
		FPOINT monsterPos = monster->GetPostion();
		FPOINT direction = GetDirectionToPlayer(monsterPos);

		// ���� ��ġ ���
		FPOINT nextPos = {
			monsterPos.x + direction.x * monster->GetSpeed() * deltaTime,
			monsterPos.y + direction.y * monster->GetSpeed() * deltaTime
		};

		// �̵� ���� ���� Ȯ��
		if (CanMoveToPosition(nextPos))
		{
			monster->SetPosition(nextPos);
		}
	}
	
	
	// �̵� �� ��������Ʈ ������Ʈ
	// �� �����Ӹ��� ��������Ʈ�� ����� �ٽ� ����
	//SpriteManager::GetInstance()->ClearSprites();

	//// ���� ��������Ʈ ���
	//for (auto& monster : monsters) {
	//	if (monster->GetIsActive()) {
	//		SpriteManager::GetInstance()->PutSprite(TEXT("Image/Rocket.bmp"), monster->GetPostion());
	//	}
	//}
	

	// ��������Ʈ �ʱ�ȭ�� ù �����ӿ��� �� �� ����
	static bool isFirstFrame = true;
	if (isFirstFrame) {
		// ��������Ʈ �ʱ� ���
		for (auto& monster : monsters) {
			SpriteManager::GetInstance()->PutSprite(TEXT("Image/Rocket.bmp"), monster->GetPostion());
		}
		isFirstFrame = false;
	}
	else {
		//// ���Ŀ��� ��ġ�� ������Ʈ
		//// ��������Ʈ �Ŵ����� ���� ��� �߰� �ʿ�
		//SpriteManager::GetInstance()->ClearSprites();
		//for (auto& monster : monsters) {
		//	SpriteManager::GetInstance()->PutSprite(TEXT("Image/Rocket.bmp"), monster->GetPostion());
		//}
	}
}

void MonsterManager::FindPlayer()
{
}


void MonsterManager::CatchPlayer()
{
}

Monster* MonsterManager::CreateMonster(FPOINT position, float speed)
{
	Monster* monster = new Monster();
	monster->Init(position);
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
