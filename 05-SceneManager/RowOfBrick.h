#pragma once

#include "GameObject.h"

// 
// The most popular type of object in Mario! 
// 
class CRowOfBrick : public CGameObject
{
protected:
	int length;				// Unit: cell 
	float cellWidth;
	float cellHeight;
	int spriteIdBegin, spriteIdMiddle, spriteIdEnd;



	// Hitbox inset (pixels) applied to both sides horizontally and vertically.
	float bboxInsetX = 0.0f;
	float bboxInsetY = 0.0f;

public:
	bool isVertical = false;        // false = horizontal, true = vertical
	int spriteIdTop = -1;           // for vertical
	int spriteIdBottom = -1;

	float offset_vertical = 4.0f;


	CRowOfBrick(float x, float y,
		float cell_width, float cell_height, int length,
		int sprite_id_begin, int sprite_id_middle, int sprite_id_end) :CGameObject(x, y)
	{
		this->length = length;
		this->cellWidth = cell_width;
		this->cellHeight = cell_height;
		this->spriteIdBegin = sprite_id_begin;
		this->spriteIdMiddle = sprite_id_middle;
		this->spriteIdEnd = sprite_id_end;
	}

	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void RenderBoundingBox();

	//int IsDirectionColliable(float nx, float ny);
	void SetHitboxInset(float insetX, float insetY) { bboxInsetX = insetX; bboxInsetY = insetY; }

	int isblocking() { return 1; }

	//void SetFlipStanding(bool flip) { FlipStanding = flip; }
};

typedef CRowOfBrick* LPROWOFBRICK;