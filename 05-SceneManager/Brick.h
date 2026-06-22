#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "Debug.h"
#include "SpringPad.h"
#include "Player.h"

#define ID_ANI_BRICK 10000
#define ID_ANI_FALLING_BRICK 10001
#define BRICK_WIDTH 14
#define BRICK_BBOX_WIDTH 14
//#define BRICK_BBOX_HEIGHT 14

class CBrick : public CGameObject {
public:
	// 0 for normal brick
	// 1 falling brick
	// 2 for falling brick with springpad
	int type = 0; 
	CSpringPad* springPad = nullptr;
	bool SpringPadOnTop = false;

	float Sensor_Width = 14.0f;
	float Sensor_Height = 3.0f;
	float OffsetX = 0.0f;
	float OffsetY = 0.0f;


	//Before activation (this applies to non-normal bricks)===========================================
	bool Activate = false;
	float PreActiveTime = 500.0f;
	float PreActiveCounter = 0.0f;
	bool Action = false;

	//For falling brick=======================================
	//Is falling
	bool isFalling = false;
	float FallingSpeed = 0.007f;
	float GravityScale = 0.00065f;
	float SavedPositionX = PositionX;
	float SavedPositionY = PositionY;
	float RespawnTime = 1500.0f;
	float Fall_RespawnCounter = 0.0f;


	//For Spike Brick========================================



	CBrick(float x, float y, int type) : CGameObject(x, y) {
		if (type == 1 || type == 2) {
			SavedPositionX = PositionX;
			SavedPositionY = PositionY;

			if (type == 2) {
				springPad = new CSpringPad(SavedPositionX, SavedPositionY);
				SpringPadOnTop = true;
			//	DebugOut(L"Spring pad created at (%.2f, %.2f)\n", PositionX, PositionY);
			}
		}
		//type 2 has springpad on top of it

		this->type = type;
		DebugOut(L"type: %d\n", type);
	}
	void Render();	
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	//void GetBoundingBoxSensor(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 1; }
	//int IsDirectionColliable(float nx, float ny);

	void ActivateBrick() {
		Activate = true;
		PreActiveCounter = PreActiveTime;
		//DebugOut(L"Active: %d \n", Activate);
	}
};