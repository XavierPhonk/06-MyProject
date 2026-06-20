#pragma once

#include "GameObject.h"
#include "Sprite.h"
#include "AssetIDs.h"
#include "Player.h"

class CSpike : public CGameObject
{
public:
	float Width = 14.0f;
	float Height = 4.0f;
	float OffsetX = 0.0f;
	float OffsetY = 1.0f;
	bool Vertical = false;
	int RotationDegree_IN_INT = 0;
	int length = 1;
	
//	CSpike(float x, float y) : CGameObject(x, y) {}
	CSpike(float x, float y, int length = 1) : CGameObject(x, y) {
		this->length = length;
	}

	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
	void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithPlayer(LPCOLLISIONEVENT e);


};
