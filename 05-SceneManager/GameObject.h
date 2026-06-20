#pragma once

#include <Windows.h>
#include <d3dx10.h>
#include <vector>

#include "Animation.h"
#include "Animations.h"
#include "Sprites.h"
#include "Collision.h"
#include <string>

using namespace std;

#define ID_TEX_BBOX -100		// special texture to draw object bounding box
#define BBOX_ALPHA 0.25f		// Bounding box transparency

class CGameObject
{
protected:

	float PositionX; 
	float PositionY;

	float VelocityX;
	float VelocityY;

	int nx;	 

	int state;

	bool isDeleted; 

public: 

	std::string sourceLine = ""; // Add this!
	string Name = "";
	void SetPosition(float x, float y) { this->PositionX = x, this->PositionY = y; }
	void SetSpeed(float vx, float vy) { this->VelocityX = vx, this->VelocityY = vy; }
	void GetPosition(float &x, float &y) { x = this->PositionX; y = this->PositionY; }
	void GetSpeed(float &vx, float &vy) { vx = this->VelocityX; vy = this->VelocityY; }
	float GetPositionX() { return this->PositionX; }
	float GetPositionY() { return this->PositionY; }

	string Get_obj_Name() { return Name; }
	void Set_obj_Name(string newName) { Name = newName; }

	int GetState() { return this->state; }
	virtual void Delete() { isDeleted = true;  }
	bool IsDeleted() { return isDeleted; }

	void RenderBoundingBox();

	CGameObject();
	CGameObject(float x, float y) :CGameObject() { this->PositionX = x; this->PositionY = y; }


	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) {};
	virtual void Render() = 0;
	virtual void SetState(int state) { this->state = state; }

	//
	// Collision ON or OFF ? This can change depending on object's state. For example: die
	//
	virtual int IsCollidable() { return 0; };

	// When no collision has been detected (triggered by CCollision::Process)
	virtual void OnNoCollision(DWORD dt) {};

	// When collision with an object has been detected (triggered by CCollision::Process)
	virtual void OnCollisionWith(LPCOLLISIONEVENT e) {};
	
	// Is this object blocking other object? If YES, collision framework will automatically push the other object
	virtual int IsBlocking() { return 1; }

	// Does this object collide with other object at certain direction ( like ColorBox )
	virtual int IsDirectionColliable(float nx, float ny) { return 1; }

	~CGameObject();

	static bool IsDeleted(const LPGAMEOBJECT &o) { return o->isDeleted; }
};
