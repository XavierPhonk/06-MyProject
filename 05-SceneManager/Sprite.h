#pragma once

#include "Texture.h"

class CSprite
{
	int id;				// Sprite ID in the sprite database

	int left;
	int top;
	int right;
	int bottom;

	LPTEXTURE texture;
	D3DX10_SPRITE sprite;
	D3DXMATRIX matScaling;

	D3DXMATRIX matRotation;     // Add this
	float rotationAngle;        // in radians

public:
	CSprite(int id, int left, int top, int right, int bottom, LPTEXTURE tex);

	void Draw(float x, float y);
	void SetScale(float scaleX, float scaleY) { D3DXMatrixScaling(&this->matScaling, scaleX, scaleY, 1.0f); }
	CSprite* Flip_X();
	CSprite* Flip_Y();
	CSprite* CopySprite();
	void FixScaling();

	D3DXMATRIX GetScalingMatrix() { return this->matScaling; }

	void SetRotation(float angleDegrees);   // Add this method
	float GetRotation() const;

};

typedef CSprite* LPSPRITE;