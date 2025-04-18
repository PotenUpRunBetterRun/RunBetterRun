#pragma comment(lib, "Msimg32.lib")
#include "PhoneUI.h"
#include "MapManager.h"
#include "Player.h"

HRESULT PhoneUI::Init(UIType type, FPOINT pos, FPOINT size, INT layer)
{
	isActive = true;

	this->uiType = type;
	this->pos = pos;
	this->size = size;
	this->layer = layer;
    return E_NOTIMPL;
}

void PhoneUI::Release()
{
}

void PhoneUI::Update()
{
}

void PhoneUI::Render(HDC hdc)
{
    // 1. �̴ϸ��� �׸� �޸� DC�� ��Ʈ�� ����
    HDC memDC = CreateCompatibleDC(hdc);
    HBITMAP hBmp = CreateCompatibleBitmap(hdc, size.x, size.y);
    HBITMAP oldBmp = (HBITMAP)SelectObject(memDC, hBmp);


    HBRUSH bgBrush = CreateSolidBrush(RGB(255, 0, 255));
    RECT rect = { 0, 0, size.x, size.y };
    FillRect(memDC, &rect, bgBrush);
    DeleteObject(bgBrush);

    // 2. �̴ϸ��� �׸���
    DrawMiniMapToDC(memDC);

	// 3. �޸� DC�� ���� DC�� ����
    auto dir = Player::GetInstance()->GetCameraVerDir();
    float angle = atan2f(dir.y, dir.x); // ���� ����
    GDITransformRotate(hdc, memDC, DEG_TO_RAD(-90) + angle);

    // 4. ����
    SelectObject(memDC, oldBmp);
    DeleteObject(hBmp);
    DeleteDC(memDC);
}

void PhoneUI::DrawMiniMapToDC(HDC hdc)
{
    if (!isActive)
        return;

    int tileSize = 15;
    int halfTile = tileSize / 2;

    FPOINT pPos = Player::GetInstance()->GetCameraPos();
    auto mapData = MapManager::GetInstance()->GetMapData();

    // ȭ�� ũ�� �������� �׸� radius ���
    float tilesVisibleX = size.x / (float)tileSize;
    float tilesVisibleY = size.y / (float)tileSize;

    int radiusX = static_cast<int>(ceil(tilesVisibleX / 2.0f));
    int radiusY = static_cast<int>(ceil(tilesVisibleY / 2.0f));

    HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));

    for (int dy = -radiusY; dy <= radiusY; ++dy)
    {
        for (int dx = -radiusX; dx <= radiusX; ++dx)
        {
            int mapX = static_cast<int>(pPos.x) + dx;
            int mapY = static_cast<int>(pPos.y) + dy;

            if (mapX < 0 || mapX >= mapData->width || mapY < 0 || mapY >= mapData->height)
                continue;

            const auto& tile = mapData->tiles[mapY * mapData->width + mapX];

            if (tile.roomType == RoomType::FLOOR)
            {
                float drawX = (dx - (pPos.x - static_cast<int>(pPos.x))) * tileSize + size.x / 2.0f;
                float drawY = (-dy + (pPos.y - static_cast<int>(pPos.y))-1) * tileSize + size.y / 2.0f;

                RECT rect = {
                    static_cast<int>(drawX),
                    static_cast<int>(drawY),
                    static_cast<int>(drawX + tileSize),
                    static_cast<int>(drawY + tileSize)
                };

                FillRect(hdc, &rect, brush);
            }
        }
    }

    DeleteObject(brush);

    // �÷��̾� �߽� �׸��� (�׻� �߾�)
    float playerDrawX = size.x / 2.0f;
    float playerDrawY = size.y / 2.0f;

    Ellipse(hdc,
        static_cast<int>(playerDrawX - halfTile),
        static_cast<int>(playerDrawY - halfTile),
        static_cast<int>(playerDrawX + halfTile),
        static_cast<int>(playerDrawY + halfTile));
}

void PhoneUI::GDITransformRotate(HDC hdc, HDC memDC, float angle)
{
    XFORM oldXform;
    GetWorldTransform(hdc, &oldXform);

    SetGraphicsMode(hdc, GM_ADVANCED);

    // ȸ�� �߽�
    float centerX = pos.x + size.x / 2.0f;
    float centerY = pos.y + size.y / 2.0f;

    // ȸ�� ��ȯ ����: �̵�(-�߽�) �� ȸ�� �� �̵�(+�߽�)
    XFORM xform;

    float cosA = cos(angle);
    float sinA = sin(angle);

    xform.eM11 = cosA;
    xform.eM12 = sinA;
    xform.eM21 = -sinA;
    xform.eM22 = cosA;

    // ȸ�� �߽� ����
    xform.eDx = centerX * (1 - cosA) + centerY * sinA;
    xform.eDy = centerY * (1 - cosA) - centerX * sinA;

    SetWorldTransform(hdc, &xform);

    // �߽� �������� BitBlt ��ġ ����
    int drawX = static_cast<int>(centerX - size.x / 2);
    int drawY = static_cast<int>(centerY - size.y / 2);

    TransparentBlt(hdc, drawX, drawY, size.x, size.y, memDC, 0, 0, size.x, size.y, RGB(255, 0, 255));

    SetWorldTransform(hdc, &oldXform);
}