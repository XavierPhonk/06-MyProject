#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "Sprite.h"
#include "AssetIDs.h"

#define BRICK_WIDTH 14
#define BRICK_BBOX_WIDTH 14
#define BRICK_BBOX_HEIGHT 5

class CSpringPad : public CGameObject {
public:
	bool Active = false;
	bool CanBeUsed = true;
	float CoolDownTime = 150.0f;
	float CoolDownCounter = 0.0f;
	bool IsOnFallingBrick = false;

	CSpringPad(float x, float y) : CGameObject(x, y) {}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 1; }
	int IsDirectionColliable(float nx, float ny);

};