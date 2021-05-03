#include "stdafx_u.h"
#include "StageManager.h"
#include"Map.h"
#include"PauseManager.h"
#include"CameraController.h"
#include"ScrollManager.h"
#include"InputManager.h"
#include"PanelManager.h"
#include"StageSelect.h"
#include "Player.h"
#include "ShakeComponent.h"

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

	if (GameDevice::GetInput()->TriggerKey(Keys::Q))
	{
		GetManager().lock()->GetGameObject("Screen").lock()->GetGameComponent<ShakeComponent>()->ShakeStart(11.3f);
	}

	ResetStage();
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
	shp_panelManager = GetManager().lock()->GetGameObject("PanelManager").lock()->GetGameComponent<PanelManager>();
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

void ButiEngine::StageManager::ResetStage()
{
	if (InputManager::OnTriggerOpenMenuKey())
	{
		shp_map->DestoryBlock();
		auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();
		sceneManager->ReloadScene();
	}
}

void ButiEngine::StageManager::OnGoal()
{
	//クリアしたら
	if (clearAnimationFrame < 0)
	{
		shp_map->DestoryBlock();
		ChangeScene("StageSelect");
	}
}

void ButiEngine::StageManager::ChangeScene(const std::string& arg_sceneName)
{
	auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();
	sceneManager->RemoveScene(arg_sceneName);
	sceneManager->LoadScene(arg_sceneName);
	sceneManager->ChangeScene(arg_sceneName);
}

void ButiEngine::StageManager::ModeChange()
{
	if (shp_cameraController->IsAnimation()) { return; }
	if (shp_panelManager->IsAnimation()) { return; }
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
