#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "AssetIDs.h"


class CJumpGem : public CGameObject {
public:
	float Width = 16;
	float Height = 16;
	bool Taken = false;

	float RespawnCounter = 0.0f;
	float RespawnTime = 2000.0f; // in ms
	int type = 0;
	//0 for jump gem
	//1 for dash gem

	CJumpGem(float x, float y) : CGameObject(x, y) {}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }

	void SetType(int t) { type = t; }
	int GetType() const {return type;}
};