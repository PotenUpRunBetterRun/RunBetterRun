#pragma once
#include "GameObject.h"
#include "structs.h"

class UIUnit : public GameObject
{
public:
	virtual HRESULT Init() override;
	virtual HRESULT Init(UIType type, FPOINT pos, FPOINT size, INT layer);
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

	UIType GetUIType() { return uiType; }
	void SetActive(bool isActive) { this->isActive = isActive; }
	void ToggleActive() { isActive = !isActive; }

protected:
	UIType uiType;
	//vector<UIUnit*> uiUnits;
	FPOINT pos;
	FPOINT size;
	INT layer;

	bool isActive;
};

