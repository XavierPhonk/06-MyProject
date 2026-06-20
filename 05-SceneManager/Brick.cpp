#include "Brick.h"


void CBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (type == 0) return;
//	if (Activate == false) return;

	if (springPad) {
		//springPad->SetPosition(PositionX, PositionY);
		springPad->SetPosition(PositionX, PositionY - 10.0f);
	}


	// === DETECT PLAYER ON SPRING PAD (for type 2) ===
	if (type == 2 && !isFalling && !Activate && springPad)
	{
		for (auto& obj : *coObjects)
		{
			CPlayer* player = dynamic_cast<CPlayer*>(obj);
			if (player)
			{
				float pl, pt, pr, pb;
				player->GetBoundingBox(pl, pt, pr, pb);

				//float sl, st, sr, sb;
				// Use spring pad's bounding box (approximate)
				float sensorWidth = BRICK_BBOX_WIDTH * 1.0f;   
				float sensorHeight = BRICK_BBOX_HEIGHT * 1.4f;

				float sl = PositionX - sensorWidth / 2.0f;
				float sr = PositionX + sensorWidth / 2.0f;
				float st = (PositionY - 10.0f) - sensorHeight / 2.0f;   // slightly above spring
				float sb = st + sensorHeight + 6.0f;   // extra tolerance below

				if (pr > sl && pl < sr && pb > st && pt < sb)
				{
					float py = player->GetPositionY();
					if (py < PositionY - 5.0f)   // player is above the brick
					{
						ActivateBrick();     // Trigger the falling sequence
						DebugOut(L"[EVENT] Player on spring pad: Spike Brick activated!\n");
						break;
					}
				}
			}
		}
	}


// Pre-activation delay
	if (Activate == true)
	{
		PreActiveCounter -= dt;
		if (PreActiveCounter <= 0)
		{
			Activate = false;
			Action = true;
			PreActiveCounter = PreActiveTime;
		}
	}

// Trigger falling
	if ( (type == 1 || type == 2) && Action)
	{
		isFalling = true;
		Action = false;
		Fall_RespawnCounter = RespawnTime;     // Start timer ONCE when falling begins
		VelocityY = 0.0f;                     // optional
	}
	//if (type == 1 && isFalling) {
	//	Fall_RepawnCounter -= dt;
	//	if (Fall_RepawnCounter <= 0) {
	//		Fall_RepawnCounter = RespawnTime;
	//		isFalling = false;
	//		PositionX = SavedPositionX;
	//		PositionY = SavedPositionY;
	//		VelocityY = 0;
	//		GravityScale = 0;
	//		isFalling = false;
	//	}
	//}

	//this thing doesnt need max falling speed
	if (isFalling) {
		GravityScale = 0.00065f;
		VelocityY += GravityScale * dt; // apply gravity
		SetSpeed(0, VelocityY);
		Action = false; // reset action state
		Fall_RespawnCounter -= dt;

		if (Fall_RespawnCounter <= 0)
		{
			PositionX = SavedPositionX;
			PositionY = SavedPositionY;
			VelocityY = 0;
			GravityScale = 0;
			isFalling = false;
			Fall_RespawnCounter = 0.0f;
		}
	}


	
	//DebugOut(L"activate in: %.6d\n", PreActiveCounter);
	PositionY += VelocityY * dt;

	/*if (PositionY > 1000.0f) {
		PositionX = SavedPositionX;
		PositionY = SavedPositionY;
		VelocityY = 0;
		GravityScale = 0;
		isFalling = false;
		Fall_RepawnCounter = RespawnTime;     // Start timer ONCE when falling begins

	};*/

	//DebugOut(L"Repsawn time: %.6f\n", Fall_RepawnCounter);
}

void CBrick::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if (type == 1 || type == 2) animations->Get(ID_ANI_FALLING_BRICK)->Render(PositionX, PositionY);

	else animations->Get(ID_ANI_BRICK)->Render(PositionX, PositionY);

	//RenderBoundingBox();
}

void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	
	l = PositionX - BRICK_BBOX_WIDTH/2;
	t = PositionY - BRICK_BBOX_HEIGHT/2;
	r = l + BRICK_BBOX_WIDTH;
	b = t + BRICK_BBOX_HEIGHT;
}

//void CBrick::GetBoundingBoxSensor(float& l, float& t, float& r, float& b) {
//	//if (type == 0) return;
//
//	float halfW = Sensor_Width / 2.0f;
//	float halfH = Sensor_Height / 2.0f;
//
//	float centerX = PositionX + OffsetX;
//	float centerY = (PositionY) + OffsetY; // match Render's Y shift
//	l = centerX - halfW;
//	t = centerY - halfH; // match Render's Y shift
//	r = centerX + halfW;
//	b = centerY + halfH;
//	//GetBoundingBox(l, t, r, b);
//
//}

//int CBrick::IsDirectionColliable(float nx, float ny) {
//	if (nx == 0 && ny == -1) return 1;
//	else return 0;
//}
