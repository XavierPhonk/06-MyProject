#include "CheckPoint.h"

void CCheckPoint::Render()
{
	CSprites* sprites = CSprites::GetInstance();
	sprites->Get(ID_SPRITE_CHECKPOINT)->Draw(PositionX, PositionY);
	//RenderBoundingBox();
}

void CCheckPoint::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	float baseLeft = PositionX - (CHECKPOINT_BBOX_WIDTH / 2.0f);
	float baseTop = PositionY - (CHECKPOINT_BBOX_HEIGHT / 2.0f);

	l = baseLeft + bboxInsetX;
	t = baseTop + bboxInsetY;
	r = l + CHECKPOINT_BBOX_WIDTH - bboxInsetX + 20.0f;
	b = t + CHECKPOINT_BBOX_HEIGHT - bboxInsetY + 50.0f;


}