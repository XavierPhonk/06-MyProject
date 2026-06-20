#include "Spike.h"

void CSpike::Render()
{
	//DebugOut(L"[INFO] Spike height: %.6f\n", Height);
	CSprites* sprites = CSprites::GetInstance();
	//CSprites* rotated_sprite = NULL;

	
	/*CSprite* rotated_sprite = sprites->Get(ID_SPRITE_SPIKE)->CopySprite();
	rotated_sprite->SetRotation(90.0f * RotationDegree_IN_INT);
	rotated_sprite->Draw(PositionX, PositionY);*/


	// Assuming a standard grid tile size is 16 pixels. Adjust if yours is different!
	float cellSize = 16.0f;

	for (int i = 0; i < length; i++)
	{
		CSprite* rotated_sprite = sprites->Get(ID_SPRITE_SPIKE)->CopySprite();
		rotated_sprite->SetRotation(90.0f * RotationDegree_IN_INT);

		float drawX = PositionX;
		float drawY = PositionY;

		// Offset the drawing position for each subsequent spike
		if (!Vertical) {
			drawX += i * cellSize; // Extend to the right
		}
		else {
			drawY += i * cellSize; // Extend downwards
		}

		rotated_sprite->Draw(drawX, drawY);

		// PREVENT MEMORY LEAK: Destroy the copy after drawing it!
		delete rotated_sprite;
	}

	//RenderBoundingBox();

}

void CSpike::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	//l = PositionX - Width / 2;
	//t = PositionY - Height / 2;
	/*l = PositionX;
	t = PositionY;
	r = l + Width;
	b = t + Height;*/

	float cellSize = 16.0f; // Must match the spacing in Render()

	float halfW = Width / 2.0f;
	float halfH = Height / 2.0f;

	if (Vertical) {
		float temp = halfW;
		halfW = halfH;
		halfH = temp;
	}

	float centerX = PositionX + OffsetX;
	float centerY = PositionY + OffsetY; // match Render's Y shift

	l = centerX - halfW;
	t = centerY - halfH;

	// Expand the Right or Bottom edge to cover the entire length
	if (!Vertical) {
		r = l + (halfW * 2.0f) + ((length - 1) * cellSize);
		b = t + (halfH * 2.0f);
	}
	else {
		r = l + (halfW * 2.0f);
		b = t + (halfH * 2.0f) + ((length - 1) * cellSize);
	}

	/*l = centerX - halfW;
	t = centerY - halfH; 
	r = centerX + halfW;
	b = centerY + halfH;*/
}

void CSpike::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Check collision with player
	if (coObjects)
	{
		for (LPGAMEOBJECT obj : *coObjects)
		{
			if (dynamic_cast<CPlayer*>(obj))
			{
				CPlayer* player = dynamic_cast<CPlayer*>(obj);
				float pl, pt, pr, pb;
				player->GetBoundingBox(pl, pt, pr, pb);
				float sl, st, sr, sb;
				GetBoundingBox(sl, st, sr, sb);
				if (CCollision::AABBCheck(pl, pt, pr, pb, sl, st, sr, sb))
				{
					// Correct constructor usage: (t, nx, ny, dx, dy, obj, src_obj)
					LPCOLLISIONEVENT e = new CCollisionEvent(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, obj, this);
					OnCollisionWithPlayer(e);

					// ... after player->GetBoundingBox(...) and GetBoundingBox(...)
					bool horizA = (sl < pr);          // spike.left < player.right
					bool horizB = (sr > pl);          // spike.right > player.left
					bool vertA = (st < pb);          // spike.top < player.bottom
					bool vertB = (sb > pt);          // spike.bottom > player.top
					bool result = CCollision::AABBCheck(pl, pt, pr, pb, sl, st, sr, sb);
					bool result_swapped = CCollision::AABBCheck(sl, st, sr, sb, pl, pt, pr, pb);

					//DebugOut(L"[DBG] Spike LTRB=%.2f,%.2f,%.2f,%.2f | Player LTRB=%.2f,%.2f,%.2f,%.2f\n",
					//	sl, st, sr, sb, pl, pt, pr, pb);
					//DebugOut(L"[DBG] comps: sl<pr=%d  sr>pl=%d  st<pb=%d  sb>pt=%d\n",
					//	horizA ? 1 : 0, horizB ? 1 : 0, vertA ? 1 : 0, vertB ? 1 : 0);

					//DebugOut(L"[DBG] AABBCheck(params player,spike) => %d\n", result ? 1 : 0);
					//// Also test swapped order in case of param-order confusion:
					//DebugOut(L"[DBG] AABBCheck(params spike,player) => %d\n", result_swapped ? 1 : 0);
					delete e; // Clean up the event after handling
				}
			}
		}
	}

}


void CSpike::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CPlayer*>(e->obj))
	{
		OnCollisionWithPlayer(e);
	}
}

void CSpike::OnCollisionWithPlayer(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CPlayer*>(e->obj))
	{
		CPlayer* player = dynamic_cast<CPlayer*>(e->obj);
		if (player->GetState() == ID_PLAYER_STATE_DIE) return;
		DebugOut(L"[INFO] Player died at positionX: %.6f, positionY: %.6f\n", player->GetPositionX(), player->GetPositionY());
		player->SetState(ID_PLAYER_STATE_DIE);
		//DebugOut(L"[INFO] Player hit spike and died. \n");
		// DebugOut(L"[INFO] Here's your corpse, Press R to respawn \n");
	}
}