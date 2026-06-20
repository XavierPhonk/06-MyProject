
#include "JumpGem.h"
#include "debug.h"

void CJumpGem::Update(DWORD dt, vector<LPGAMEOBJECT>*coObjects)
{
	if (Taken) {
		RespawnCounter -= (float)dt;
		// optional debug:
		// DebugOut(L"[DBG] JumpGem RespawnCounter=%.2f Taken=%d\n", RespawnCounter, Taken ? 1 : 0);
	}
	if (RespawnCounter <= 0.0f && Taken) {
		Taken = false;
		RespawnCounter = RespawnTime;
	//	DebugOut(L"[INFO] JumpGem respawned at (%.2f, %.2f)\n", PositionX, PositionY);
	}
}
void CJumpGem::Render()
{
	CSprites* Frame_sprites = CSprites::GetInstance();
	int Frame_sprites_ID = ID_SPRITE_JUMP_GEM_FRAME;

	switch (type)
	{
	case 0:
		Frame_sprites_ID = ID_SPRITE_JUMP_GEM_FRAME;
		break;
	case 1:
		Frame_sprites_ID = ID_SPRITE_DASH_GEM_FRAME;
		//DebugOut(L"use dash gem frame \n");
		break;
	default:
		break;
	}

	Frame_sprites->Get(Frame_sprites_ID)->FixScaling();
	Frame_sprites->Get(Frame_sprites_ID)->Draw(PositionX, PositionY);


	if (Taken == true) return;
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = ID_ANI_DASH_GEM;

	switch (type)
	{
		case 0:
			aniId = ID_ANI_JUMP_GEM;
			break;	
		case 1:
			aniId = ID_ANI_DASH_GEM;
			break;
	default:
		break;
	}

	animations->Get(aniId)->Render(PositionX, PositionY);
	//RenderBoundingBox();
}

void CJumpGem::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = PositionX - Width / 2;
	t = PositionY - Height / 2;
	r = l + Width;
	b = t + Height;
}