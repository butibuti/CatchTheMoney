#include "stdafx_u.h"
#include "StageManager.h"
#include"Map.h"
#include"PauseManager.h"
#include"CameraController.h"
#include"ScrollManager.h"
#include"InputManager.h"
#include "Player.h"

void ButiEngine::StageManager::OnUpdate()
{
	if (!wkp_player.lock())
	{
		wkp_player = GetManager().lock()->GetGameObject("Player");
		return;
	}

	if (wkp_player.lock()->GetGameComponent<Player>()->IsClear())
	{
		clearAnimationFrame--;
	}

	OnGoal();
	ModeChange();

	GetManager().lock()->GetApplication().lock()->GetGUIController()->SetGUIObject(GetThis<StageManager>());
}

void ButiEngine::StageManager::OnSet()
{
}

void ButiEngine::StageManager::Start()
{
	shp_map = GetManager().lock()->GetGameObject("Map").lock()->GetGameComponent<Map>();
	shp_pauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManager>();
	shp_scrollManager = GetManager().lock()->GetGameObject("Screen").lock()->GetGameComponent<ScrollManager>();
	shp_cameraController = GetManager().lock()->GetGameObject("Camera").lock()->GetGameComponent<CameraController>();

	shp_map->PutTile();

	clearAnimationFrame = 180;

	mode = GameMode::Normal;
}

void ButiEngine::StageManager::ShowGUI()
{
	GUI::Begin("ClearAnimationTime");
	GUI::Text(clearAnimationFrame);
	GUI::End();
}

void ButiEngine::StageManager::OnCollision(std::weak_ptr<GameObject> arg_other)
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::StageManager::Clone()
{
	return ObjectFactory::Create<StageManager>();
}

void ButiEngine::StageManager::OnGoal()
{
	//ÉNÉäÉAÇµÇΩÇÁ
	if (clearAnimationFrame < 0)
	{
		shp_map->DestoryBlock();
		auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();
		std::string nextSceneName = "StageSelect";
		sceneManager->RemoveScene(nextSceneName);
		sceneManager->LoadScene(nextSceneName);
		sceneManager->ChangeScene(nextSceneName);
	}
}

void ButiEngine::StageManager::ModeChange()
{
	if (shp_cameraController->IsAnimation()) { return; }
	if (InputManager::OnTriggerModeChangeKey())
	{
		shp_pauseManager->SwitchPause();
		shp_scrollManager->ResetScroll();
		
		if (mode == GameMode::Normal)
		{
			mode = GameMode::Edit;
			shp_cameraController->ZoomOut();
		}
		else
		{
			mode = GameMode::Normal;
			shp_cameraController->ZoomIn();
		}
	}
}
