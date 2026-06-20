#include <iostream>
#include <fstream>
#include "AssetIDs.h"

#include "PlayScene.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
//#include "Coin.h"
#include "Platform.h"
#include "Brick.h"
#include "CheckPoint.h"
#include "RowOfBrick.h"
#include "Player.h"
#include "Spike.h"
#include "SampleKeyEventHandler.h"
#include "JumpGem.h"
#include "SpringPad.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath):
	CScene(id, filePath)
{
	player = NULL;
	key_handler = new CSampleKeyHandler(this);
}


#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_ASSETS	1
#define SCENE_SECTION_OBJECTS	2

#define ASSETS_SECTION_UNKNOWN -1
#define ASSETS_SECTION_SPRITES 1
#define ASSETS_SECTION_ANIMATIONS 2

#define MAX_SCENE_LINE 1024

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPTEXTURE tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return; 
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ASSETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	wstring path = ToWSTR(tokens[0]);
	
	LoadAssets(path.c_str());
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i+1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

/*
	Parse a line in section [OBJECTS] 
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	// debug: show the raw line and tokens
	DebugOut(L"[DEBUG] _ParseSection_OBJECTS raw: %s\n", ToWSTR(line).c_str());
	DebugOut(L"[DEBUG] tokens.size() = %d\n", (int)tokens.size());
	for (size_t i = 0; i < tokens.size(); ++i)
	{
		DebugOut(L"[DEBUG] token[%d] = %s\n", (int)i, ToWSTR(tokens[i]).c_str());
	}

	// skip invalid lines - an object set must have at least type, x, y
	if (tokens.size() < 3) {
		DebugOut(L"[WARN] _ParseSection_OBJECTS: invalid object line (need 3 tokens)\n");
		return;
	}

	int object_type = atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());

	CGameObject *obj = NULL;

	switch (object_type)
	{
	/*case OBJECT_TYPE_MARIO:
		if (player!=NULL) 
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CMario(x,y); 
		player = (CMario*)obj;  

		DebugOut(L"[INFO] Player object has been created!\n");
		break;*/
	//case OBJECT_TYPE_GOOMBA: obj = new CGoomba(x,y); break;
	case OBJECT_TYPE_PLAYER:
		if (player != NULL)
		{
			DebugOut(L"[ERROR] PLAYER object was created before!\n");
			return;
		}
		obj = new CPlayer(x, y);
		//obj->SetSpawnPoint(x, y);
		obj->Set_obj_Name("Player");

		player = (CPlayer*)obj;

		if (tokens.size() > 3) {
			float LevelBoundaryY = (float)atof(tokens[3].c_str());
			((CPlayer*)player)->LevelBoundaryY = LevelBoundaryY;
			DebugOut(L"[INFO] Player LevelBoundaryY set to %.6f\n", LevelBoundaryY);
		}
		DebugOut(L"[INFO] Player object has been created!\n");
		break;

	case OBJECT_TYPE_BRICK: {
		int type = 0;
		type = atoi(tokens[3].c_str());
		obj = new CBrick(x, y, type); 
		obj->Set_obj_Name("Brick");

		CBrick* brick = nullptr;
		brick = (CBrick*)obj;
		
		if (type == 1) obj->Set_obj_Name("Falling brick");

		if (type == 2 && brick->springPad != nullptr)
		{
			// Add spring pad slightly above the brick
			brick->springPad->IsOnFallingBrick = true;
			brick->springPad->SetPosition(x, y);   // adjust height as needed

			objects.push_back(brick->springPad);   // Add spring pad to scene
			obj->Set_obj_Name("Brick with spring pad");

			DebugOut(L"[INFO] SpringPad auto-created and added on top of Brick at (%.1f, %.1f)\n", x, y);
		}
		/*if (tokens.size() < 4) {type = 0;}
		else {
			type = atoi(tokens[3].c_str());
			if (type == NULL) type = 0;
			((CBrick*)obj)->type = type;
		}*/

		break;
	}
	case OBJECT_TYPE_SPIKE: {
		obj = new CSpike(x, y);
		//break; 


		int Rotation_Degree = (int)atof(tokens[3].c_str());

		if (tokens.size() > 4) {
			int length = atoi(tokens[4].c_str());
			((CSpike*)obj)->length = length;
		}

		DebugOut(L"[INFO] Spike object has been created with length %d and rotation %d\n", ((CSpike*)obj)->length, Rotation_Degree);
		/*if (Id_mod_sprite_type == 1) {
			DebugOut(L"spike will flip\n");
			((CSpike*)obj)->RotationDegree_IN_INT = true;
		}

		else if (Id_mod_sprite_type == 2) {
			DebugOut(L"spike will go 90 clockwise\n");
			((CSpike*)obj)->Clockwise_90 = true;

		}*/

		if (Rotation_Degree % 2 == 1) {

			((CSpike*)obj)->Vertical = true;
			DebugOut(L"this object is vertical\n");
		}
		((CSpike*)obj)->RotationDegree_IN_INT = Rotation_Degree;
		obj->Set_obj_Name("Spike");

		break;
	}

	case OBJECT_TYPE_SPRING_PAD: obj = new CSpringPad(x, y); obj->Set_obj_Name("Spring Pad"); break;

	//case OBJECT_TYPE_COIN: obj = new CCoin(x, y); break;
	case OBJECT_TYPE_CHECKPOINT: {
		obj = new CCheckPoint(x, y); obj->Set_obj_Name("CheckPoint");
		//((CCheckPoint*)obj)->SetHitboxInset(-2.0f, -50.0f);
		break;
	}
	case OBJECT_TYPE_PLATFORM:
	{

		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_begin = atoi(tokens[6].c_str());
		int sprite_middle = atoi(tokens[7].c_str());
		int sprite_end = atoi(tokens[8].c_str());

		obj = new CPlatform(
			x, y,
			cell_width, cell_height, length,
			sprite_begin, sprite_middle, sprite_end
		);
		((CPlatform*)obj)->SetHitboxInset(4.0f, 2.0f);
		obj->Set_obj_Name("Platform (cloud)");
		break;
	}

	case OBJECT_TYPE_ROW_OF_BRICK:
	{

		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_begin = atoi(tokens[6].c_str());
		int sprite_middle = atoi(tokens[7].c_str());
		int sprite_end = atoi(tokens[8].c_str());
		int vertical = atoi(tokens[9].c_str());

		cell_width = 16.0f;
		cell_height = 16.0f;

		obj = new CRowOfBrick(
			x, y,
			cell_width, cell_height, length,
			sprite_begin, sprite_middle, sprite_end
		);
		if (vertical == 1) {
			((CRowOfBrick*)obj)->isVertical = true;
			((CRowOfBrick*)obj)->spriteIdTop = sprite_begin;
			((CRowOfBrick*)obj)->spriteIdBottom = sprite_end;
		}
		obj->Set_obj_Name("Row of bricks");
		//((CRowOfBrick*)obj)->SetHitboxInset(-4.0f, -4.0f);


		break;
	}
	case OBJECT_TYPE_JUMP_GEM: {
		int type = (int)atof(tokens[3].c_str());

		obj = new CJumpGem(x, y); 
		((CJumpGem*)obj)->SetType(type);
		//DebugOut(L"[INFO] Created JumpGem of type %d at (%f, %f)\n", type, x, y);
		if (type == 0) obj->Set_obj_Name("Jump Gem");
		else if (type == 1) obj->Set_obj_Name("Dash Gem");
		else obj->Set_obj_Name("Gem");

		break;
		
	}
	case OBJECT_TYPE_PORTAL:
	{
		float r = (float)atof(tokens[3].c_str());
		float b = (float)atof(tokens[4].c_str());
		int scene_id = atoi(tokens[5].c_str());
		if (tokens.size() > 6) {
			bool allow_quit = atoi(tokens[6].c_str());
		}

		obj = new CPortal(x, y, x+16.0f, y+16.0f, scene_id);
		if (tokens.size() > 6) {
			((CPortal*)obj)->allow_quit = true;
		}

		obj->Set_obj_Name("Door");
	}
	break;

	
	default:
		DebugOut(L"[ERROR] Invalid object type: %d\n", object_type);
		obj->Set_obj_Name("What The Fuck");
		return;
	}

	// General object setup
	obj->SetPosition(x, y);

	obj->sourceLine = line;

	objects.push_back(obj);
}

void CPlayScene::LoadAssets(LPCWSTR assetFile)
{
	DebugOut(L"[INFO] Start loading assets from : %s \n", assetFile);

	// Convert wide string to narrow UTF-8 for std::ifstream
	int len = WideCharToMultiByte(CP_UTF8, 0, assetFile, -1, NULL, 0, NULL, NULL);
	if (len <= 0)
	{
		DebugOut(L"[ERROR] Failed to convert asset file path\n");
		return;
	}
	std::string assetPath;
	assetPath.resize(len);
	WideCharToMultiByte(CP_UTF8, 0, assetFile, -1, &assetPath[0], len, NULL, NULL);
	if (!assetPath.empty() && assetPath.back() == '\0') assetPath.pop_back();

	std::ifstream f;
	f.open(assetPath.c_str());
	if (!f.is_open())
	{
		DebugOut(L"[ERROR] Cannot open asset file: %s (as narrow: %S)\n", assetFile, assetPath.c_str());
		return;
	}

	int section = ASSETS_SECTION_UNKNOWN;
	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		std::string line(str);

		if (line.size() == 0) continue;
		if (line[0] == '#') continue;

		if (line == "[SPRITES]") { section = ASSETS_SECTION_SPRITES; continue; }
		if (line == "[ANIMATIONS]") { section = ASSETS_SECTION_ANIMATIONS; continue; }
		if (line[0] == '[') { section = ASSETS_SECTION_UNKNOWN; continue; }

		switch (section)
		{
		case ASSETS_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case ASSETS_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		}
	}

	f.close();
	DebugOut(L"[INFO] Done loading assets from %s\n", assetFile);
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene from : %s \n", sceneFilePath);

	// Convert wide path to narrow for ifstream
	int len = WideCharToMultiByte(CP_UTF8, 0, sceneFilePath, -1, NULL, 0, NULL, NULL);
	if (len <= 0)
	{
		DebugOut(L"[ERROR] Failed to convert scene file path\n");
		return;
	}
	std::string scenePath;
	scenePath.resize(len);
	WideCharToMultiByte(CP_UTF8, 0, sceneFilePath, -1, &scenePath[0], len, NULL, NULL);
	if (!scenePath.empty() && scenePath.back() == '\0') scenePath.pop_back();

	std::ifstream f;
	f.open(scenePath.c_str());
	if (!f.is_open())
	{
		DebugOut(L"[ERROR] Cannot open scene file: %s (as narrow: %S)\n", sceneFilePath, scenePath.c_str());
		return;
	}

	int section = SCENE_SECTION_UNKNOWN;
	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		std::string line(str);

		if (line.size() == 0) continue;
		if (line[0] == '#') continue;
		if (line == "[ASSETS]") { section = SCENE_SECTION_ASSETS; continue; }
		if (line == "[OBJECTS]") { section = SCENE_SECTION_OBJECTS; continue; }
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		switch (section)
		{
		case SCENE_SECTION_ASSETS: _ParseSection_ASSETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}
	f.close();

	DebugOut(L"[INFO] Done loading scene %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	/*vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}*/

		// For each object, build a list of other objects (so each object can see the player and others)
	for (size_t i = 0; i < objects.size(); i++)
	{
		vector<LPGAMEOBJECT> coObjects;
		coObjects.reserve(objects.size() > 0 ? objects.size() - 1 : 0);
		for (size_t j = 0; j < objects.size(); j++)
		{
			if (j == i) continue;
			coObjects.push_back(objects[j]);
		}

		objects[i]->Update(dt, &coObjects);
	}


	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return; 

	// Update camera to follow mario
	//float cx, cy;
	float CameraPositionX, CameraPositionY;
	float playerX = player->GetPositionX();
	float playerY = player->GetPositionY();


	player->GetPosition(CameraPositionX, CameraPositionY);



	CGame *game = CGame::GetInstance();
	CameraPositionX -= game->GetBackBufferWidth() / 2;
	CameraPositionY -= game->GetBackBufferHeight() / 2;
	/*if (CameraPositionX < 0) CameraPositionX = 0;
	if (CameraPositionY < 0) CameraPositionY = 0;*/
	float OffsetX = 0.0f;
	float OffsetY = -10.0f;

	CGame::GetInstance()->SetCamPos(CameraPositionX + OffsetX, CameraPositionY + OffsetY);

	//DebugOut(L"[INFO] Camera position: (%.2f, %.2f)\n", CameraPositionX + OffsetX, CameraPositionY + OffsetY);
	//DebugOut(L"[INFO] offset: (%.2f, %.2f)\n", OffsetX, OffsetY);

	PurgeDeletedObjects();
}

void CPlayScene::Render()
{
	for (int i = 0; i < objects.size(); i++)
		objects[i]->Render();
}

/*
*	Clear all objects from this scene
*/
void CPlayScene::Clear()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		delete (*it);
	}
	objects.clear();
}

/*
	Unload scene

	TODO: Beside objects, we need to clean up sprites, animations and textures as well 

*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %d unloaded! \n", id);
}

bool CPlayScene::IsGameObjectDeleted(const LPGAMEOBJECT& o) { return o == NULL; }

void CPlayScene::PurgeDeletedObjects()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted())
		{
			delete o;
			*it = NULL;
		}
	}

	// NOTE: remove_if will swap all deleted items to the end of the vector
	// then simply trim the vector, this is much more efficient than deleting individual items
	objects.erase(
		std::remove_if(objects.begin(), objects.end(), CPlayScene::IsGameObjectDeleted),
		objects.end());
}

void CPlayScene::SaveScene(const std::string& filepath)
{
	std::ofstream file(filepath);

	if (!file.is_open())
	{
		DebugOut(L"[ERROR] Failed to open file for saving\n");
		return;
	}

	file << "# DUMPED FROM IMGUI LEVEL EDITOR\n";

	file << "[ASSETS]\n";
	

	//====================
	file << "Player.txt\n";
	file << "SpringPad.txt\n";
	file << "brick.txt\n";
	file << "SpringPad.txt\n";
	file << "CheckPoint.txt\n";
	file << "cloud.txt\n";
	file << "RowOfBrick.txt\n";
	file << "Spike.txt\n";
	file << "JumpGem.txt\n";
	file << "Portal.txt\n";


//=============
	//vector<int> object_header_exists = {};
	//for (size_t i = 0; i < objects.size(); i++)
	//{

	//	if (objects[i] == NULL) continue;

	//	if (objects[i]->sourceLine == "") continue;
	//	vector<string> tokens = split(objects[i]->sourceLine);
	//	int object_type = atoi(tokens[0].c_str());

	//	if (std::find(object_header_exists.begin(), object_header_exists.end(), object_type) != object_header_exists.end()) {
	//		//std::cout << "Item found in the vector!" << std::endl;
	//		continue;
	//	}

	//	switch (object_type) {
	//	case OBJECT_TYPE_PLAYER:
	//		file << "Player.txt\n";
	//		break;
	//	case OBJECT_TYPE_BRICK:
	//		file << "SpringPad.txt\n";
	//		file << "brick.txt\n";
	//		break;
	//	case OBJECT_TYPE_SPRING_PAD:
	//		file << "SpringPad.txt\n";
	//		break;
	//	case OBJECT_TYPE_CHECKPOINT:
	//		file << "CheckPoint.txt\n";
	//		break;
	//	case OBJECT_TYPE_PLATFORM:
	//		file << "cloud.txt\n";
	//		break;
	//	case OBJECT_TYPE_ROW_OF_BRICK:
	//		file << "RowOfBrick.txt\n";
	//		break;
	//	case OBJECT_TYPE_SPIKE:
	//		file << "Spike.txt\n";
	//		break;
	//	case OBJECT_TYPE_JUMP_GEM:
	//		file << "JumpGem.txt\n";
	//		break;
	//	case OBJECT_TYPE_PORTAL:
	//		file << "Portal.txt\n";
	//		break;


	//	default:
	//		break;
	//	}
	//	object_header_exists.push_back(object_type);

	//}

	file << "\n\n";
	file << "[OBJECTS]\n";
	file << "# See OBJECT_TYPE constants in AssetIDs.h\n";
	file << "# 60001	: Player\n";
	file << "# 1		: Brick\n";
	file << "# 8000		: Row of brick\n";
	file << "# 5		: Platform (cloud)\n";
	file << "# 9000		: Spike\n";
	file << "# 8		: Spring Pad\n";
	file << "# 7000		: Checkpoint\n";
	file << "# 6		: Gem\n";
	file << "# 50		: Portal\n\n";

	file << "# Format: [ObjectType] [X] [Y] [Extra Params...]\n\n";

	for (size_t i = 0; i < objects.size(); i++)
	{
		if (objects[i] == NULL) continue;

		// If an object was created at runtime (like the auto-spawned SpringPad 
		// on top of your Brick), it won't have a sourceLine. We skip saving it 
		// so it doesn't duplicate the next time you load the game!
		if (objects[i]->sourceLine == "") continue;

		float x, y;
		objects[i]->GetPosition(x, y);

		// Split the original text back into tokens
		vector<string> tokens = split(objects[i]->sourceLine);

		if (tokens.size() < 3) continue;

		int object_type = atoi(tokens[0].c_str());
		switch (object_type) {
		case OBJECT_TYPE_PLAYER:
			file << "#Player\n";
			file << "#id	x	y	(Y_levelBoundary)\n";

			break;

		case OBJECT_TYPE_BRICK:
			file << "#Brick\n";
			file << "#id	x	y	type\n";
			break;
		case OBJECT_TYPE_SPRING_PAD:
			file << "#Spring Pad\n";
			break;

		case OBJECT_TYPE_CHECKPOINT:
			file << "#Checkpoint\n";
			break;
		case OBJECT_TYPE_PLATFORM:
			file << "#Platform\n";
			file << "#id	x	y	cellwidth	cellheight	length	spritebegin	mid	end\n";

			break;
		case OBJECT_TYPE_ROW_OF_BRICK:
			file << "#Row of brick\n";
			file << "#id      x   y   cwid chei  len   Spr_be     mid	end	      vertical? (1 for vertical)\n";
			break;
		case OBJECT_TYPE_SPIKE:
			file << "#Spike\n";
			file << "#id	x	y		rotation degree * 90\n";
			break;
		case OBJECT_TYPE_JUMP_GEM:
			file << "#Gem\n";
			file << "#id    x      y      type\n";
			break;
		case OBJECT_TYPE_PORTAL:
			file << "#Door===========================================\n";
			file << "#id    x      y      r		b	next scene\n";

			break;

		default:
			break;
		}

		file << "#Object[" << i << "]\n";

		// 1. Write the Object Type
		file << tokens[0] << "\t";

		// 2. Write the NEW X and Y coordinates
		file << x << "\t" << y;

		// 3. Loop through and write ANY extra config parameters (if they exist)
		// This safely preserves Platform lengths, Spike rotations, Brick types, etc.
		for (size_t t = 3; t < tokens.size(); t++)
		{
			file << "\t" << tokens[t];
		}

		file << "\n\n";

	}

	file.close();
	DebugOut(L"[INFO] Scene successfully saved to: %s\n", std::wstring(filepath.begin(), filepath.end()).c_str());
}

