#include "RowOfBrick.h"

#include "Sprite.h"
#include "Sprites.h"

#include "Textures.h"
#include "Game.h"
 
void CRowOfBrick::RenderBoundingBox()
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
	// Calculate center position of the bounding box
	float drawX, drawY;

	if (!isVertical)
	{
		// Horizontal - original logic improved
		drawX = PositionX - this->cellWidth / 2.0f + (rect.right / 2.0f);
		drawY = PositionY;
	}
	else
	{
		// Vertical - anchored at bottom (as we set in previous Render())
		drawX = PositionX;
		drawY = PositionY - (this->cellHeight * (this->length - 1)) / 2.0f;
		// This centers the bbox vertically on the whole column
	}

	CGame::GetInstance()->Draw(
		drawX - cx,
		drawY - cy,
		bbox,
		nullptr,
		BBOX_ALPHA,
		rect.right - 1,
		rect.bottom - 1
	);


	float xx = PositionX - this->cellWidth / 2 + rect.right / 2;

	//CGame::GetInstance()->Draw(xx - cx, PositionY - cy, bbox, nullptr, BBOX_ALPHA, rect.right - 1, rect.bottom - 1);



}

void CRowOfBrick::Render()
{
	if (this->length <= 0) return;
	//float xx = PositionX - this->cellWidth / 2;
	float Sprite_Center_PositionX = PositionX;
	CSprites* s = CSprites::GetInstance();
	float posX = PositionX;
	float posY = PositionY;


	if (!isVertical) {
		s->Get(this->spriteIdBegin)->Draw(Sprite_Center_PositionX, PositionY);
		Sprite_Center_PositionX += this->cellWidth;
		for (int i = 1; i < this->length - 1; i++)
		{
			s->Get(this->spriteIdMiddle)->Draw(Sprite_Center_PositionX, PositionY);
			Sprite_Center_PositionX += this->cellWidth;
		}
		if (length > 1)
			s->Get(this->spriteIdEnd)->Draw(Sprite_Center_PositionX, PositionY);
	}

	else {
		//float currentY = PositionY;                    // Start at bottom

		//// Draw Bottom piece first
		//s->Get(this->spriteIdBottom != -1 ? this->spriteIdBottom : this->spriteIdEnd)
		//	->Draw(posX, currentY);

		//currentY -= this->cellHeight;   // Move up
		//float offset = 4.0f;

		//// Draw middle pieces
		//for (int i = 1; i < this->length - 1; i++)
		//{
		//	s->Get(this->spriteIdMiddle)->Draw(posX, currentY);
		//	currentY -= this->cellHeight - offset;
		//}
		//// Draw Top piece last
		//if (length > 1)
		//	s->Get(this->spriteIdTop != -1 ? this->spriteIdTop : this->spriteIdBegin)
		//	->Draw(posX, currentY + offset);

		//float offset = 4.0f;	
		s->Get(this->spriteIdBegin)->Draw(posX, posY);
		posY -= this->cellHeight;

		for (int i = 1; i < this->length - 1; i++)
		{
			s->Get(this->spriteIdMiddle)->Draw(posX, posY);
			posY -= this->cellHeight;
		}
		if (length > 1){
			s->Get(this->spriteIdEnd)->Draw(posX, posY);
		}

		//RenderBoundingBox();

	}
		//RenderBoundingBox();
}

void CRowOfBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	// base box
	float baseLeft = PositionX - (this->cellWidth) / 2.0f;
	float baseTop = PositionY - (this->cellHeight) / 2.0f;

	SetHitboxInset(1.0f, -1.0f);
	// this modifies the bboxInsetX and bboxInsetY

	if (!isVertical)
	{
		// Horizontal
		l = baseLeft + bboxInsetX;
		t = baseTop + bboxInsetY;
		r = baseLeft + cellWidth * length - bboxInsetX;
		b = baseTop + cellHeight - bboxInsetY;
	}
	else
	{
		SetHitboxInset(0.0f, 0.5f);
		// Vertical - growing upward
		float baseLeft = PositionX - cellWidth / 2.0f;
		float baseBottom = PositionY + cellHeight / 2.0f;   // PositionY is now the bottom

		l = baseLeft + bboxInsetX;
		t = baseBottom - cellHeight * length + bboxInsetY;
		r = baseLeft + cellWidth - bboxInsetX;
		b = baseBottom - bboxInsetY;
	
	}
}

//int CRowOfBrick::IsDirectionColliable(float nx, float ny)
//{
//	if (nx == 0 && ny == -1) return 1;
//	else return 0;
// 
//}

