#pragma once

#include "Singleton.h"
#include "config.h"
#include <vector>

using namespace std;
class Item;
class Sprite;
class ItemManager : public Singleton<ItemManager>
{
private:
	int itemCount;
	vector<Item*> vecitem;
	map<LPCWCH, Sprite>	itemMap;
public:
	// TODO: item ��������?
	// TODO: item ��������Ʈ ����
	// TODO: item ������ ī��Ʈ ����
	// TODO:
};

