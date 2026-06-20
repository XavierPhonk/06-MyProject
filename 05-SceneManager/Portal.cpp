#include "Portal.h"
#include "Game.h"
#include "Textures.h"

CPortal::CPortal(float l, float t, float r, float b, int scene_id )
{
	this->scene_id = scene_id;
	PositionX = l; 
	PositionY = t;
	width = r - l;
	height = b - t;
}

void CPortal::RenderBoundingBox()
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

	CGame::GetInstance()->Draw(PositionX - cx, PositionY - cy, bbox, nullptr, BBOX_ALPHA, rect.right - 1, rect.bottom - 1);
}

void CPortal::Render()
{
	CSprites* sprites = CSprites::GetInstance();
	if (allow_quit == true) sprites->Get(ID_SPRITE_PORTAL_QUIT)->Draw(PositionX, PositionY);
	else sprites->Get(ID_SPRITE_PORTAL)->Draw(PositionX, PositionY);
	//RenderBoundingBox();
}

void CPortal::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = PositionX - width/2;
	t = PositionY - height/2;
	r = PositionX + width/2;
	b = PositionY + height/2;
}