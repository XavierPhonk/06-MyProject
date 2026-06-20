#include "SpringPad.h"
#include "Debug.h"

void CSpringPad::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

	// Spring pad doesn't move so no need to update position
	// Just check if it should be deactivated after being used
	if (CanBeUsed == false) {
		CoolDownCounter -= dt;
		Active = true;
	}
	
	if (CoolDownCounter <= 0) {
		CanBeUsed = true;
		Active = false;
		CoolDownCounter = 0.0f;
		
	}
	//DebugOut(L"[INFO] Spring Pad Cooldown Counter: %.6f\n", CoolDownCounter);
}
void CSpringPad::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	CSprites* sprites = CSprites::GetInstance();

	if (Active) {
		animations->Get(ID_ANI_SPRING_PAD)->Render(PositionX, PositionY - 0.2f);
		// Active = false;
	}
	else {
		sprites->Get(ID_SPRITE_SPRING_PAD_1)->Draw(PositionX, PositionY);
		//Active = false;

	}
	//RenderBoundingBox();
}

void CSpringPad::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = PositionX - BRICK_BBOX_WIDTH / 2;
	t = PositionY - BRICK_BBOX_HEIGHT / 2;
	r = l + BRICK_BBOX_WIDTH;
	b = t + BRICK_BBOX_HEIGHT;
}

int CSpringPad::IsDirectionColliable(float nx, float ny)
{
	/*if (nx == 0 && ny == -1) return 1;
	else return 0;*/
	if (ny == 1) return 0;
	return 1;
}