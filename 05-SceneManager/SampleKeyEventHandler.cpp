#include "SampleKeyEventHandler.h"

#include "debug.h"
#include "Game.h"

//#include "Mario.h"
#include "PlayScene.h"
#include "Player.h"

/*
void CSampleKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	CMario* mario = (CMario *)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer(); 
	//CPlayer* mario = (CPlayer *)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer(); 

	switch (KeyCode)
	{
	case DIK_DOWN:
		mario->SetState(MARIO_STATE_SIT);
		break;
	case DIK_S:
		mario->SetState(MARIO_STATE_JUMP);
		break;
	case DIK_1:
		mario->SetLevel(MARIO_LEVEL_SMALL);
		break;
	case DIK_2:
		mario->SetLevel(MARIO_LEVEL_BIG);
		break;
	case DIK_0:
		mario->SetState(MARIO_STATE_DIE);
		break;
	case DIK_R: // reset
		//Reload();
		break;
	}
}

void CSampleKeyHandler::OnKeyUp(int KeyCode)
{
	DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);

	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	//CPlayer* mario = (CPlayer*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	switch (KeyCode)
	{
	case DIK_S:
		mario->SetState(MARIO_STATE_RELEASE_JUMP);
		break;
	case DIK_DOWN:
		mario->SetState(MARIO_STATE_SIT_RELEASE);
		break;
	}
}

void CSampleKeyHandler::KeyState(BYTE *states)
{
	LPGAME game = CGame::GetInstance();
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	//CPlayer* mario = (CPlayer*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if (game->IsKeyDown(DIK_RIGHT))
	{
		if (game->IsKeyDown(DIK_A))
			mario->SetState(MARIO_STATE_RUNNING_RIGHT);
		else
			mario->SetState(MARIO_STATE_WALKING_RIGHT);
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		if (game->IsKeyDown(DIK_A))
			mario->SetState(MARIO_STATE_RUNNING_LEFT);
		else
			mario->SetState(MARIO_STATE_WALKING_LEFT);
	}
	else
		mario->SetState(MARIO_STATE_IDLE);
}

*/





void CSampleKeyHandler::OnKeyDown(int KeyCode)
{
	CPlayer* player = (CPlayer*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (KeyCode == DIK_R) {
		DebugOut(L"[INFO] Resetting player...\n");
		player->RespawnPlayer();
		return;
	}

	if (KeyCode == DIK_L) {
		//reset level
		CGame::GetInstance()->GetCurrentScene()->Unload();
		CGame::GetInstance()->GetCurrentScene()->Load();
		player->infiniteJump = false;
		DebugOut(L"[INFO] Level reset triggered.\n");
	}


	if (player == NULL || player->GetState() == ID_PLAYER_STATE_DIE) {
		player->SetVelocityX(0);
		return;
	}

	
	switch (KeyCode)
	{
	case DIK_SPACE:
		player->SetState(ID_PLAYER_STATE_JUMP);
		break;
	case DIK_DOWN:
		//if (player->isOnPlatform)
		//player->SetPosition(player->GetPositionX(), player->GetPositionY() + 0.1f);
		player->SetState(ID_PLAYER_STATE_FALL_OFF_PLATFORM);
		break;

	case DIK_W:
		player->SetState(ID_PLAYER_STATE_DIE);
		//CGame::GetInstance()->KeyBoardEnabled = false;
		break;
	case DIK_R:
		player->SetState(ID_PLAYER_STATE_IDLE);
		//CGame::GetInstance()->KeyBoardEnabled = true;
		//respawn player
		break;
	case DIK_X:
		player->SetState(ID_PLAYER_STATE_DASH);
		break;
	}
}
void CSampleKeyHandler::OnKeyUp(int KeyCode) {
	//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
	// safe early exit for this project (no-op) or mirror the same guards as OnKeyDown
	LPSCENE cur = CGame::GetInstance()->GetCurrentScene();
	if (!cur) return;
	LPGAMEOBJECT obj = ((LPPLAYSCENE)cur)->GetPlayer();
	if (!obj) return;
	CPlayer* player = dynamic_cast<CPlayer*>(obj);
	if (!player) return;

	// implement OnKeyUp behavior if needed

	if (KeyCode == DIK_SPACE) {
		player->JumpKeyReleased = true;
		player->JumpKeyDown = false;
	}

	/*switch (KeyCode)
	{
		case DIK_RIGHT:
			player->JumpKeyDown = false;
		break;*/
		//case DIK_LEFT:
		//	player->SetState(ID_PLAYER_STATE_IDLE);
		//	break;
		//case DIK_SPACE:
		//	player->SetState(ID_PLAYER_STATE_IDLE);
		//	break;
		//default:
		//	player->SetState(ID_PLAYER_STATE_IDLE);
		//	break;
		//}
	
}

void CSampleKeyHandler::KeyState(BYTE* states)
{
	LPGAME game = CGame::GetInstance();
	CPlayer* player = (CPlayer*)((LPPLAYSCENE)game->GetCurrentScene())->GetPlayer();
	if (player == NULL || player->GetState() == ID_PLAYER_STATE_DIE) return;



	if (game->IsKeyDown(DIK_SPACE)) {
		//player->SetState(ID_PLAYER_STATE_JUMP);
		player->JumpKeyDown = true;
		player->JumpKeyReleased = false;
	}

	if (!player->isDashing) {
		// Check for horizontal movement
		if (game->IsKeyDown(DIK_RIGHT))
		{
			player->SetState(ID_PLAYER_STATE_WALKING);
			//DebugOut(L"[INFO] Right key held down\n");
			//prioritize left key if both are held for dynamically changing direction
			if (game->IsKeyDown(DIK_LEFT)) player->SetFacing(-1);
			else player->SetFacing(1);
		}
		else if (game->IsKeyDown(DIK_LEFT))
		{
			player->SetState(ID_PLAYER_STATE_WALKING);
			player->SetFacing(-1);
		}
		else
		{
			// If no movement keys are held, return to Idle
			player->SetState(ID_PLAYER_STATE_IDLE);

		}
	}
}