#include "Platform.h"

#include "Sprite.h"
#include "Sprites.h"

#include "Textures.h"
#include "Game.h"

void CPlatform::RenderBoundingBox()
{
	D3DXVECTOR3 p(PositionX, PositionY, 0);
	RECT rect;

	LPTEXTURE bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	float cx, cy;
	CGame::GetInstance()->GetCamPos(cx, cy);

	float xx = PositionX - this->cellWidth / 2 + rect.right / 2;

	CGame::GetInstance()->Draw(xx - cx, PositionY - cy, bbox, nullptr, BBOX_ALPHA, rect.right - 1, rect.bottom - 1);
}

void CPlatform::Render()
{
	if (this->length <= 0) return; 
	//float xx = PositionX - this->cellWidth / 2;
	float Sprite_Center_PositionX = PositionX;
	CSprites * s = CSprites::GetInstance();

	s->Get(this->spriteIdBegin)->Draw(Sprite_Center_PositionX, PositionY);
	Sprite_Center_PositionX += this->cellWidth;
	for (int i = 1; i < this->length - 1; i++)
	{
		s->Get(this->spriteIdMiddle)->Draw(Sprite_Center_PositionX, PositionY);
		Sprite_Center_PositionX += this->cellWidth ;
	}
	if (length>1)
		s->Get(this->spriteIdEnd)->Draw(Sprite_Center_PositionX, PositionY);
	//RenderBoundingBox();
}

void CPlatform::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	// base box
	float baseLeft = PositionX - (this->cellWidth / 2.0f);
	float baseTop = PositionY - (this->cellHeight / 2.0f);

	// apply insets (reduces width by 2*bboxInsetX and height by 2*bboxInsetY)
	l = baseLeft + bboxInsetX;
	t = baseTop + bboxInsetY;
	r = baseLeft + this->cellWidth * this->length - bboxInsetX;
	b = baseTop + this->cellHeight - bboxInsetY;

}

int CPlatform::IsDirectionColliable(float nx, float ny)
{
	if (nx == 0 && ny == -1) return 1;
	else return 0;
}
