#pragma once

#include "GameObject.h"
#include "Sprite.h"
#include "AssetIDs.h"

#define CHECKPOINT_BBOX_WIDTH 16
#define CHECKPOINT_BBOX_HEIGHT 30

class CCheckPoint : public CGameObject
{
public:
	float bboxInsetX = 0.0f;
	float bboxInsetY = 0.0f;
	bool IsActivated = false; // whether checkpoint has been activated by player

	CCheckPoint(float x, float y) : CGameObject(x, y) {}
	float GetX() { return PositionX; }
	float GetY() { return PositionY; }

	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; } //Checkpoint is just a trigger
	void SetHitboxInset(float insetX, float insetY) { bboxInsetX = insetX; bboxInsetY = insetY; }
	bool Activate(){ 
		if (!IsActivated) {
			IsActivated = true;
			return true; // return true if checkpoint was successfully activated
		}
		return false; // already activated
	}
};
