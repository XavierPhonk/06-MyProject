#include <windows.h>
//#include <mmsystem.h>
//#pragma comment(lib, "winmm.lib")

#include <algorithm>
#include "debug.h"
#include "AssetIDs.h"


#include "Player.h"
#include "Game.h"
#include "Platform.h"
#include "Collision.h"
#include "Brick.h"
#include "CheckPoint.h"
#include "SampleKeyEventHandler.h"
#include "RowOfBrick.h"
#include "JumpGem.h"
#include "Portal.h"
#include "SpringPad.h"


void CPlayer::Update(DWORD dt, vector<LPGAMEOBJECT>*coObjects) {
	//DebugOut(L"[GRAVITY] before: %.6f\n", GravityScale);

	//Reset collisions flags
	//ResetCollisionFlags();

	//process collisions first
	if (isDeleted) return;

	CCollision::GetInstance()->Process(this, dt, coObjects);


	// Check if player is grounded by checking objects below
	CheckGroundBelow(coObjects);

	
	// You can check whether player collided only with bricks here:
	// 
	// if (CollidedOnlyBrick()) { /* handle special case */ }

	if (IsGrounded) {
		CoyoteCounter = CoyoteTime; // reset coyote counter when grounded
		CurrentDashes = MaxDashes; 
		AirJumpsCurrent = MaxAirJumps;
		AirJumpConsumable = false;
	}
	else {
		if (CoyoteCounter > 0) {
			CoyoteCounter -= (float)dt;
		}
		else {
			CoyoteCounter = 0;
		}

		CanDash = true; 

	}

	if (CoyoteCounter <= 0) {
		AirJumpConsumable = true;
	}

	//if (!isDashing) {Gra}

	//DebugOut(L"[INFO] Coyote counter: %d\n", CoyoteCounter);
	// Update position (displacement)
	if (!IsGrounded && !isDashing && this->state != ID_PLAYER_STATE_DIE) {
		VelocityY += GravityScale * dt;
		if (VelocityY > MaxFallingSpeed) VelocityY = MaxFallingSpeed;
	}
	if (this->state == ID_PLAYER_STATE_DIE) {
		VelocityX = 0;
		VelocityY = 0;
	}

	//Start jumping
	if (JumpBufferCounter > 0) JumpBufferCounter -= (float)dt;

	
	
	if (JumpBufferCounter > 0 && CoyoteCounter > 0 && !AirJumpConsumable) {
		IsJumping = true;
		JumpTimeCounter = MaxJumpTime;
		VelocityY = -JumpStrength;
		CoyoteCounter = 0;
		JumpBufferCounter = 0;
		AirJumpConsumable = true;
	}
	

	//Hold to jump higher
	if (IsJumping && JumpKeyDown) {
		//DebugOut(L"[INFO] JumpTimeCounter: %.6f\n", JumpTimeCounter);

		if (JumpTimeCounter > 0) {
			VelocityY -= HoldForce * ((float)dt/100.0f);
			JumpTimeCounter -= (float)dt;
		}
		else {
			IsJumping = false;
		}
	}

	//stop jumping completetly
	if (JumpKeyReleased) {
		IsJumping = false;
		JumpTimeCounter = 0;
	}

	//===============================================================================================================


	/*if (IsUsingSpringPad == true) {
		UsingSpringpadCounter -= dt;
		if (UsingSpringpadCounter <= 0) {
			IsUsingSpringPad = false;
			UsingSpringpadCounter = 0.0f;
		}
	}
	DebugOut(L"[INFO] UsingSpringPadCounter: %.6f | IsUsingSpringPad: %d\n", UsingSpringpadCounter, IsUsingSpringPad ? 1 : 0);
	DebugOut(L"[INFO] UsingSpringPadTime: %.6f\n", UsingSpringPadTime);*/

	//Dash distance is approximately 60 pixels
	// DASH handling
	if (isDashing)
	{
		
		// while dashing, decrement duration and keep dash speed
		if (DashDurationCounter > 0.0f)
		{
			DashDurationCounter -= (float)dt;
			VelocityX = DashSpeed * Facing; // velocity is per-ms; DON'T multiply by dt here
			VelocityY = 0;
			//player is allowed to dash into spring pad cuz why not lol
			//GravityScale = 0;
			//DebugOut(L"Dash\n");
		}
		//After dash
		else
		{
			isDashing = false;
			VelocityX = Default_velocityX * Facing;
			//disable dash
			CanDash = false;
			DashCoolDownCounter = DashCoolDown;
			//GravityScale = 0.00065f;
			if (!IsGrounded) CurrentDashes--;
			else if (CurrentDashes <= 0) CurrentDashes = 0;
			//DebugOut(L"End Dash\n");
			//DebugOut(L"Dash duration: %.6f \n", DashDuration);


		}

	//	DebugOut(L"[INFO] Player Dashing - VelocityX: %.6f | DashDurationLeft: %.2f\n", VelocityX, DashDurationCounter);
	}

if (!CanDash)
	{

		if (DashCoolDownCounter > 0.0f)
		{
			DashCoolDownCounter -= (float)dt;
			if (DashCoolDownCounter <= 0.0f)
			{
				CanDash = true;
				DashCoolDownCounter = 0.0f;
				//CurrentDashes = MaxDashes;
	
			}
		}
	//	DebugOut(L"[INFO] Dash Cooldown Counter: %.6f | CanDash: %d\n", DashCoolDownCounter, CanDash ? 1 : 0);
	}

	//kill the fucking player
	if (PositionY > LevelBoundaryY) {
		this->state = ID_PLAYER_STATE_DIE;
		//DebugOut(L"[INFO] Player went into oblivion. \n");
		//DebugOut(L"[INFO] Here's your corpse, Press R to respawn \n");
		this->PositionX = CurrentSpawn.x;
		this->PositionY = CurrentSpawn.y;
	}


	//DebugOut(L"[INFO] VelocityY: %.6f\n", VelocityY);
	//DebugOut(L"[INFO] JumpTimeCounter: %.6f\n", JumpTimeCounter);
	//DebugOut(L"[INFO] JumpPressed: %d\n", JumpKeyDown);
	//DebugOut(L"[INFO] JumpBufferCounter: %f\n", JumpBufferCounter);
	//DebugOut(L"[INFO] deltatime: %d\n", dt);	
	//DebugOut(L"[INFO] Player isOnPlatform? : %d\n", isOnPlatform);
	//DebugOut(L"[INFO] velocityY = %f\n", VelocityY);
	//DebugOut(L"dt = %d | GravityScale = %.6f | velY = %.3f\n", dt, GravityScale, VelocityY);
	//DebugOut(L"[INFO] Grounded? : %d\n", IsGrounded);
	//DebugOut(L"[INFO] Facing: %d\n", Facing);
	//DebugOut(L"[INFO] Player Position: (%.6f, %.6f)\n", PositionX, PositionY);
	//DebugOut(L"current dashes: %d\n", CurrentDashes);
	//DebugOut(L"current AIRJUMP: %d\n", AirJumpsCurrent);
	//
	//DebugOut(L"Air jump: %d\n", AirJumpsCurrent);	
	//DebugOut(L"CoyoteTime: %.6f\n", CoyoteCounter);
	//DebugOut(L"Collide fall: %d\n", Colide_fall);
	//DebugOut(L"Collide Spike: %d\n", Colide_spike);
}

void CPlayer::Render() {
	CAnimations* animations = CAnimations::GetInstance();
	CSprites* sprite = CSprites::GetInstance();
	int spriteId = ID_SPRITE_PLAYER_IDLE;
	int aniId = ID_ANI_PLAYER_IDLE;

	this->state = state;

	switch (state)
	{
	case ID_PLAYER_STATE_IDLE:
		aniId = ID_ANI_PLAYER_IDLE;
		break;

	case ID_PLAYER_STATE_WALKING:
		aniId = ID_ANI_PLAYER_WALKING;
		/*if (Facing < 0) sprite->Get(spriteId)->Flip_X()->Draw(PositionX, PositionY);
		else sprite->Get(spriteId)->Draw(PositionX, PositionY); */
		break;
	case ID_PLAYER_STATE_DASH:
		aniId = ID_ANI_PLAYER_WALKING;
		break;
	case ID_PLAYER_STATE_DIE:
		aniId = ID_ANI_PLAYER_DIE;
		break;

	default:
		aniId = ID_ANI_PLAYER_IDLE;
		break;
	}   


	if (VelocityY < 0 && this->state != ID_PLAYER_STATE_DIE) {
		aniId = ID_ANI_PLAYER_JUMP;
	//	DebugOut(L"[INFO] Player Jumping - VelocityY: %.6f\n", VelocityY);
	}

	if (VelocityY > 0 && this->state != ID_PLAYER_STATE_DIE) {
		aniId = ID_ANI_PLAYER_FALL;
	//	DebugOut(L"[INFO] Player Falling - VelocityY: %.6f\n", VelocityY);
	}

	if (Facing < 0) {
		animations->Get(aniId)->Render(PositionX, PositionY, -1);
	}
	else animations->Get(aniId)->Render(PositionX, PositionY, 1);
	//RenderBoundingBox();
}
void CPlayer::SetState(int state) {

	if (state == ID_PLAYER_STATE_DIE) {
		DebugOut(L"[INFO] Player Died\n");
		this->VelocityX = 0;
		this->state = state;
		//PlaySound(L"D:\\schooling\\hkiv\\game dev\\projects\\06-MyProject\\05-SceneManager\\sfx\\wav files\\lose-sfx-365579.wav", NULL, SND_ASYNC | SND_FILENAME);
		//PlaySFX("die");
		return;
	}

	switch (state)
	{
	case ID_PLAYER_STATE_IDLE:
		VelocityX = 0;
		//DebugOut(L"[INFO] Player Idle\n");
		
		break;
	case ID_PLAYER_STATE_WALKING:
		VelocityX = Default_velocityX * Facing;
		//DebugOut(L"[INFO] Player Walking - VelocityX: %.6f\n", VelocityX);
		break;
	case ID_PLAYER_STATE_JUMP:
		if (infiniteJump)
		{
			VelocityY = -JumpStrength - 0.1f;
			break;
		}
		if (AirJumpConsumable && AirJumpsCurrent > 0) {
			VelocityY = -JumpStrength - 0.12f;
			AirJumpConsumable = false;
			AirJumpsCurrent--;
			//DebugOut(L"[INFO] Air Jump used! Air jumps left: %d\n", AirJumpsCurrent);

		}
		else {
			JumpBufferCounter = JumpBufferTime;
		}

		
		//DebugOut(L"[INFO] Player Jump Buffer Counter set to: %f\n", JumpBufferCounter);
		//DebugOut(L"[INFO] Player Jump Buffer Time: %f\n", JumpBufferTime);
		//DebugOut(L"[INFO] Player Jump\n");
		//DebugOut(L"[INFO] VelocityY after jump: %f\n", VelocityY);

		break;
	case ID_PLAYER_STATE_FALL_OFF_PLATFORM:
		if (isOnPlatform) {
			PositionY += 1.0f;
		}//DebugOut(L"[INFO] Player Fall Off Platform\n");
		//DebugOut(L"[INFO] Player isOnPlatform? : %d\n", isOnPlatform);
		break;
	//case ID_PLAYER_STATE_DIE:
	//	// isDeleted = true;
	//	break;

	case ID_PLAYER_STATE_DASH:
		if (CanDash && CurrentDashes > 0) {
			//start dashing
			isDashing = true;
			DashDurationCounter = DashDuration;   
			CanDash = false;                      
			
			//VelocityX = DashSpeed * Facing;     
		}
		break;
	}

	// update stored state for all non-die cases
	this->state = state;
	//DebugOut(L"[INFO] Player state: %d\n", this->state);
};

void CPlayer::OnNoCollision(DWORD dt) {
	// move normally if no collision; clear ground flag
	//if (this->state == ID_PLAYER_STATE_DIE) return;

	PositionX += VelocityX * dt;
	PositionY += VelocityY * dt;

	/*isOnPlatform = false;
	IsGrounded = false;*/
};

void CPlayer::OnCollisionWith(LPCOLLISIONEVENT e)
{

	// mark what we collided with this frame
	//if (dynamic_cast<CBrick*>(e->obj)) collidingWithBrick = true;
	//if (dynamic_cast<CPlatform*>(e->obj)) collidingWithPlatform = true;
	if (dynamic_cast<CCheckPoint*>(e->obj)) OnCollisionWithCheckPoint(e);
	if (dynamic_cast<CJumpGem*>(e->obj)) OnCollisionWithJumpGem(e);
	if (dynamic_cast<CPortal*>(e->obj)) OnCollisionWithPortal(e);
	//if (dynamic_cast<CSpringPad*>(e->obj)) OnCollisionWithSpringPad(e);

	if (Colide_fall) OnCollisionWithFallingBrick(e);
	// vertical collisions
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		// stop vertical motion
		VelocityY = 0;
		// landed on top
		//IsGrounded = true;

		//DebugOut(L"[INFO] Player collided vertically with object\n");
		//DebugOut(L"[INFO] Player grounded? : %d\n", IsGrounded);
		//DebugOut(L"[INFO] VelocityY = %.6f\n", VelocityY);
		//OnCollisionWithFallingBrick(e);	
		if (e->ny < 0)
		{
			OnCollisionWithPlatform(e);
			OnCollisionWithSpringPad(e);
			OnCollisionWithFallingBrick(e);
		}
	}


	// horizontal collisions
	if (e->nx != 0 && e->obj->IsBlocking())
	{
		VelocityX = 0;
		OnCollisionWithSpringPad(e);
		//OnCollisionWithFallingBrick(e);

	}

	/*if (dynamic_cast<CPlatform*>(e->obj))
		OnCollisionWithPlatform(e);*/


}
void CPlayer::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	float halfW = PLAYER_BBOX_WIDTH / 2.0f;
	float halfH = PLAYER_BBOX_HEIGHT / 2.0f;
 
	// inset the hitbox by the margins
	float insetX = (float)PLAYER_BBOX_MARGIN_X;
	float insetY = (float)PLAYER_BBOX_MARGIN_Y;

	left = PositionX - halfW + insetX;
	top = PositionY - halfH + insetY;
	right = PositionX + halfW - insetX;
	//bottom = PositionY + halfH - insetY + 1.0f;
	bottom = PositionY + halfH - insetY + 1.0f;
}

void CPlayer::OnCollisionWithPlatform(LPCOLLISIONEVENT e)
{
	isOnPlatform = true;
	//DebugOut(L"[INFO] Player grounded on platform? : %d\n", IsGrounded);
	//DebugOut(L"[INFO] Player isOnPlatform? : %d\n", isOnPlatform);
	//DebugOut(L"[INFO] VelocityY = %.6f\n", VelocityY);
}

void CPlayer::CheckGroundBelow(vector<LPGAMEOBJECT>* coObjects, float checkDistance)
{
	// default: not grounded unless we find something
	isOnPlatform = false;
	IsGrounded = false;
	//collidingWithBrick = false;
	//collidingWithPlatform = false;
	/*Colide_fall = false;
	Colide_spike = false;*/

	// get player's bounding box
	float l, t, r, b;
	GetBoundingBox(l, t, r, b);;

	// sample a small vertical band under the player's bottom
	float probeTop = b;
	float probeBottom = b + checkDistance;

	// horizontal extents
	float playerLeft = l;
	float playerRight = r;

	for (auto obj : *coObjects)
	{
		if (obj == this) continue;
		if (obj->IsDeleted()) continue;

		// consider only blocking (solid) objects for ground check
		if (!obj->IsBlocking()) continue;

		float obj_l, obj_t, obj_r, obj_b;
		obj->GetBoundingBox(obj_l, obj_t, obj_r, obj_b);

		// check if object's top sits within the probe band (object top between probeTop and probeBottom)
		bool topInBand = (obj_t >= probeTop && obj_t <= probeBottom);	
		// check horizontal overlap
		bool horizOverlap = (playerRight > obj_l) && (playerLeft < obj_r);

		if (topInBand && horizOverlap)
		{
			// found candidate ground directly under the player
			if (dynamic_cast<CPlatform*>(obj))
			{
				isOnPlatform = true;
				IsGrounded = true;
				//collidingWithPlatform = true;
				return; // prefer platform if found; exit early
			}
			else if (dynamic_cast<CBrick*>(obj) || dynamic_cast<CRowOfBrick*>(obj))
			{
				isOnPlatform = false;
				IsGrounded = true;
			//	collidingWithBrick = true;
				//if (dynamic_cast<CBrick*>(obj)->type == 1) {
				//	//DebugOut(L"Acti\n");
				//	Colide_fall = true;
				//}
				//if (dynamic_cast<CBrick*>(obj)->type == 2) {
				//	Colide_spike = true;

				//}
				// continue search; we might prefer platform if also present
				// but it's fine to return here if bricks should take precedence:
				return;
			}
			else
			{
				// other blocking object: treat as ground
				IsGrounded = true;
				return;
			}
		}
	}

	// nothing detected under feet within probe distance: remain not grounded
	IsGrounded = false;
	isOnPlatform = false;
	//Colide_fall = Colide_spike = false;
}

void CPlayer::OnCollisionWithCheckPoint(LPCOLLISIONEVENT e)
{
	CCheckPoint* checkpoint = dynamic_cast<CCheckPoint*>(e->obj);
	if (checkpoint) {
		if (checkpoint->IsActivated == true) return;
		else {
			checkpoint->Activate();
			SetSpawnPoint(checkpoint->GetX(), checkpoint->GetY());
		//	DebugOut(L"[INFO] Checkpoint activated at (%.6f, %.6f)\n", checkpoint->GetX(), checkpoint->GetY());
		}
	}
}
void CPlayer::OnCollisionWithJumpGem(LPCOLLISIONEVENT e)
{
	CJumpGem* jumpGem = dynamic_cast<CJumpGem*>(e->obj);
	if (!jumpGem->Taken) {
		jumpGem->Taken = true;
		if (jumpGem->type == 0) {
			AirJumpsCurrent++;
			//DebugOut(L"JUmp gem collected \n");
		}

		if (jumpGem->type == 1) {
			CurrentDashes++;
			//DebugOut(L"[INFO] Dash Gem collected! Current dashes: %d\n", CurrentDashes);
		}
		jumpGem->RespawnCounter = jumpGem->RespawnTime;
		//DebugOut(L"[INFO] Gem collected!\n");
	}
}	

void CPlayer::OnCollisionWithPortal(LPCOLLISIONEVENT e)
{
	CPortal* portal = dynamic_cast<CPortal*>(e->obj);
	DebugOut(L"in the portal\n");
	if (portal->allow_quit) {
		exit(0);
		return;
	}
	CGame::GetInstance()->InitiateSwitchScene(portal->GetSceneId());
}

void CPlayer::OnCollisionWithSpringPad(LPCOLLISIONEVENT e)
{
	CSpringPad* springPad = dynamic_cast<CSpringPad*>(e->obj);
	if (springPad && springPad->CanBeUsed) {
		//IsUsingSpringPad = true;
		//UsingSpringpadCounter = UsingSpringPadTime;
		VelocityY = -0.35f; 
		if (isDashing) isDashing = false; 
		//	cancel dash if hitting spring pad
		//	plus, player can dash into the spring pad cuz why not lol
		
		//DebugOut(L"[INFO] Spring Pad activated! VelocityY set to %.6f\n", VelocityY);
		springPad->Active = true;
		springPad->CanBeUsed = false;
		springPad->CoolDownCounter = springPad->CoolDownTime;
		CurrentDashes = MaxDashes; 
		AirJumpsCurrent = MaxAirJumps;
		//DebugOut(L"[INFO] Spring Pad activated! Jump and Dash reset: CurrentDashes = %d, AirJumpsCurrent = %d\n", CurrentDashes, AirJumpsCurrent);	
	}
}

void CPlayer::OnCollisionWithFallingBrick(LPCOLLISIONEVENT e)
{
    CBrick* brick = dynamic_cast<CBrick*>(e->obj);
    if (brick && (brick->type == 1 || brick->type == 2))
    {
        brick->ActivateBrick();
        //DebugOut(L"Falling brick activated!\n");
    }
}

void CPlayer::OnCollisionWithSpikeBrick(LPCOLLISIONEVENT e) {
	CBrick* brick = dynamic_cast<CBrick*>(e->obj);
	if (brick && brick->type == 2) {
		brick->ActivateBrick();
		//DebugOut(L"[INFO] Player hit spike brick! Brick at (%.6f, %.6f) activated.\n");
	}
}


void CPlayer::RespawnPlayer_AT(float PosX, float PosY) {
	SetPosition(PosX, PosY);
	VelocityX = Default_velocityX;
	VelocityY = Default_velocityY;
	SetState(ID_PLAYER_STATE_IDLE);
}

void CPlayer::RespawnPlayer() {
	RespawnPlayer_AT(CurrentSpawn.x, CurrentSpawn.y);
}



void CPlayer::SetLevel(int l) {};