#include "Sprite.h"

#include "Game.h"

CSprite::CSprite(int id, int left, int top, int right, int bottom, LPTEXTURE tex)
{
	this->id = id;
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
	this->texture = tex;

	float texWidth = (float)tex->getWidth();
	float texHeight = (float)tex->getHeight();

	// Set the sprite’s shader resource view
	sprite.pTexture = tex->getShaderResourceView();

	sprite.TexCoord.x = this->left / texWidth;
	sprite.TexCoord.y = this->top / texHeight;

	int spriteWidth = (this->right - this->left + 1);
	int spriteHeight = (this->bottom - this->top + 1);

	sprite.TexSize.x = spriteWidth / texWidth;
	sprite.TexSize.y = spriteHeight / texHeight;

	sprite.ColorModulate = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	sprite.TextureIndex = 0;

	D3DXMatrixScaling(&this->matScaling, (FLOAT)spriteWidth, (FLOAT)spriteHeight, 1.0f);

	D3DXMatrixIdentity(&this->matRotation);   
	this->rotationAngle = 0.0f;
}

LPSPRITE CSprite::Flip_X() {
	// Create flipped sprite by swapping left and right in the constructor arguments

	// "this->left - 1" is becuz it seems to lose 1 pixel when flipping, so there should be an offset  
	CSprite* flippedSprite = new CSprite(this->id, this->right, this->top, this->left - 1, this->bottom, this->texture);

	// Copy scaling from original to flipped sprite
	float scaleX = this->GetScalingMatrix()._11;
	float scaleY = this->GetScalingMatrix()._22;
	flippedSprite->SetScale(scaleX, scaleY);

	return flippedSprite;
}

CSprite* CSprite::Flip_Y()
{
	// Swap top and bottom, with -1 offset on the new bottom (mirroring your Flip_X approach)
	CSprite* flippedSprite = new CSprite(this->id,
		this->left,
		this->bottom,      // new top = original bottom
		this->right,
		this->top,     // new bottom = original top - 1
		this->texture);

	// Copy scaling from original to flipped sprite
	float scaleX = this->GetScalingMatrix()._11;
	float scaleY = this->GetScalingMatrix()._22;
	flippedSprite->SetScale(scaleX, scaleY);

	return flippedSprite;
}

CSprite* CSprite::CopySprite() {
	CSprite* CopySprite = new CSprite(this->id,
		this->left,
		this->top,      
		this->right,
		this->bottom,    
		this->texture);

	float scaleX = this->GetScalingMatrix()._11;
	float scaleY = this->GetScalingMatrix()._22;
	CopySprite->SetScale(scaleX, scaleY);

	return CopySprite;
}

void CSprite::FixScaling() {
	float scaleX = this->GetScalingMatrix()._11;
	float scaleY = this->GetScalingMatrix()._22;
	this->SetScale(scaleX, scaleY);
}

void CSprite::Draw(float x, float y)
{
	CGame* g = CGame::GetInstance();
	float cx, cy;
	g->GetCamPos(cx, cy);

	cx = (FLOAT)floor(cx);
	cy = (FLOAT)floor(cy);

	D3DXMATRIX matTranslation;

	
	x = (FLOAT)floor(x);
	y = (FLOAT)floor(y);

	//choose the center of the sprite 

	D3DXMatrixTranslation(&matTranslation, x - cx, g->GetBackBufferHeight() - y + cy, 0.1f);

	//this->sprite.matWorld = (this->matScaling * matTranslation);
	this->sprite.matWorld = (this->matScaling * this->matRotation * matTranslation);
	//FixScaling();
	g->GetSpriteHandler()->DrawSpritesImmediate(&sprite, 1, 0, 0);
}

void CSprite::SetRotation(float angleDegrees)
{
	this->rotationAngle = D3DXToRadian(angleDegrees);
	D3DXMatrixRotationZ(&this->matRotation, this->rotationAngle);
}

float CSprite::GetRotation() const
{
	return D3DXToDegree(this->rotationAngle);
}

