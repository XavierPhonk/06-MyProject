#pragma once

#include "GameObject.h"

#include "Animation.h"
#include "Animations.h"

#include "debug.h"
#include "SampleKeyEventHandler.h"

#pragma region ANIMATION_ID

#define ID_ANI_PLAYER_IDLE 1231
#define ID_ANI_PLAYER_WALKING 1232
#define	ID_ANI_PLAYER_JUMP 1233
#define ID_ANI_PLAYER_FALL 1234
#define ID_ANI_PLAYER_DIE 1235
#pragma endregion

#pragma region STATE_ID_PLAYER
#define ID_PLAYER_STATE_IDLE 0
#define ID_PLAYER_STATE_WALKING 1
#define ID_PLAYER_STATE_JUMP 2
#define ID_PLAYER_STATE_DIE 3
#define ID_PLAYER_STATE_FALL_OFF_PLATFORM 4
#define ID_PLAYER_STATE_FALL 5
#define ID_PLAYER_STATE_DASH 6

#pragma endregion

#pragma region PLAYER_BBOX
#define PLAYER_BBOX_WIDTH 14
#define PLAYER_BBOX_HEIGHT 10
#define PLAYER_BBOX_MARGIN_X 2
#define PLAYER_BBOX_MARGIN_Y 2
#pragma endregion

struct SpawnPoint
{
	float x;
	float y;
	SpawnPoint(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}
	
};

class CPlayer : public CGameObject
{
public:
	float GravityScale = 0.00065f;
	float MaxFallingSpeed= 0.4f;
	float Default_velocityX = 0.075f;
	float Default_velocityY = 0.0f;
	
	
	float GravSpeed = VelocityY;
	bool IsGrounded = 0;
	//float TotalFallingSpeed = 0.0f;

	bool isOnPlatform = false;
	int Facing = 1;
	bool CanFallThroughPlatform = false;


	//Jump Functionality
	float JumpStrength = 0.1f;

	//Jump buffering
	float JumpBufferTime = 150.0f;
	float JumpBufferCounter = 0.0f;
	bool JumpKeyReleased = false;
	bool JumpKeyDown = false;

	//Hold jump to jump higher
	float MaxJumpTime = 120.0f; // how long the player can hold the jump key to increase jump height (in ms)
	float JumpTimeCounter = 0.0f;
	float HoldForce = 0.1f;
	bool IsJumping = false;

	//Jump Coyote time
	float CoyoteTime = 120.0f; // how long the player can still jump after leaving a platform (in ms)
	float CoyoteCounter = 0.0f;
	 
	//Air jump
	int MaxAirJumps = 1;
	int AirJumpsCurrent = 0;
	bool CanAirJump = false;
	bool AirJumpConsumable = false;

	// CHEATS
	bool infiniteJump = false;

	/*bool collidingWithPlatform = false;
	bool collidingWithBrick = false;*/

	//implement dash
	bool isDashing = false;
	float DashSpeed = 0.65f;
	bool CanDash = true;
	float DashCoolDown = 500.0f; // in ms
	float DashCoolDownCounter = 0.0f;
	float DashDuration = 80.0f;
	float DashDurationCounter = 0.0f;
//	bool AirBorneCoolDownSkip = false;
	int MaxDashes = 1;
	int CurrentDashes = 0;

	//is using spring pad	
	//bool IsUsingSpringPad = false;
	//float UsingSpringpadCounter = 0.0f;
	//float UsingSpringPadTime = 500.0f; // in ms

	bool Colide_fall = false;
	bool Colide_spike = false;

	float LevelBoundaryY = 1000.0f;

	//Respawn system
	SpawnPoint CurrentSpawn;


	CPlayer(float x, float y) : CGameObject(x, y)
	{
		VelocityX = 0.0f;
		VelocityY = 0.0f;
		GravityScale = 0.00065f;
		MaxFallingSpeed = 0.35f;
		CurrentSpawn = SpawnPoint(x, y);

		//DebugOut(L"[PLAYER CONSTRUCTOR] SUCCESS - GravityScale = %.6f\n", GravityScale);
		//DebugOut(L"[PLAYER CONSTRUCTOR] SUCCESS - JumpStrength = %.6f\n", JumpStrength);
		//DebugOut(L"[PLAYER CONSTRUCTOR] SUCCESS - MaxFallingSpeed = %.6f\n", MaxFallingSpeed);
		//DebugOut(L"[PLAYER CONSTRUCTOR] SUCCESS - GravSpeed = %.6f\n", GravSpeed);
		DebugOut(L"[PLAYER CONSTRUCTOR] SUCCESS - PositionX = %.6f\n", x);
		DebugOut(L"[PLAYER CONSTRUCTOR] SUCCESS - PositionY = %.6f\n", y);
	}

	

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();
	void SetState(int state);
	int GetState() { return this->state; }

	//collision interactions
	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	//Collision framework
	virtual int isblocking() { return 1; } // by default, player is blocking other objects
	virtual int IsCollidable() { return 1; } // player is collidable

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	//Velocities
	float GetVelocityX() { return this->VelocityX; }
	float GetVelocityY() { return this->VelocityY; }
	void SetVelocityX(float vx) { this->VelocityX = vx; }
	void SetVelocityY(float vy) { this->VelocityY = vy; }

	//position
	float GetPositionX() { return this->PositionX; }
	float GetPositionY() { return this->PositionY; }
	float SetPositionX(float x) { return this->PositionX = x; }
	float SetPositionY(float y) { return this->PositionY = y; }
	void SetFacing(int f) { this->Facing = f; }
	//Apply gravity to player
	/*void ApplyGravity(DWORD dt) {
		
		if (GravityScale != 0)
		{
			VelocityY += GravityScale * dt;
			if (VelocityY > MaxFallingSpeed) VelocityY = MaxFallingSpeed;
		}
	}	*/
	/*float GravitySpeed(DWORD dt) {

		if (GravityScale != 0)
		{
			VelocityY += GravityScale * dt;
			if (VelocityY > MaxFallingSpeed) VelocityY = MaxFallingSpeed;
		}
		return VelocityY;
	}*/

	// Object interactions via collision
	void OnCollisionWithPlatform(LPCOLLISIONEVENT e);
	//void OnCollisionWithBrick(LPCOLLISIONEVENT e);
	void OnCollisionWithCheckPoint(LPCOLLISIONEVENT e);
	void OnCollisionWithJumpGem(LPCOLLISIONEVENT e);
	void OnCollisionWithPortal(LPCOLLISIONEVENT e);
	void OnCollisionWithSpringPad(LPCOLLISIONEVENT e);
	void OnCollisionWithSpikeBrick(LPCOLLISIONEVENT e);
	void OnCollisionWithFallingBrick(LPCOLLISIONEVENT e);

	void ResetCollisionFlags() { Colide_fall = Colide_spike = false; }
	//bool CollidedWithBrick() const { return collidingWithBrick && !collidingWithPlatform; }

	// Ground-check raycast: checks objects under player's bottom within checkDistance (in pixels)
	void CheckGroundBelow(vector<LPGAMEOBJECT>* coObjects, float checkDistance = 1.0f);
	//void Delete() { CGameObject::Delete(); }
	void RespawnPlayer();
	void SetSpawnPoint(float x, float y) { CurrentSpawn = SpawnPoint(x, y); }
	void RespawnPlayer_AT(float PosX, float PosY);     //respawn at a coordinate


	void SetLevel(int l);
};
