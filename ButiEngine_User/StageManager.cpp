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
#include "ClearBand.h"
#include "NumberComponent.h"

void ButiEngine::StageManager::OnUpdate()
{
	if (!wkp_player.lock())
	{
		wkp_player = GetManager().lock()->GetGameObject("Player");
		return;
	}

	if (wkp_player.lock()->GetGameComponent<Player>()->IsClear())
	{
		if (clearAnimationFrame >= CLEAR_FRAME)
		{
			GetManager().lock()->AddObjectFromCereal("ClearFlash", ObjectFactory::Create<Transform>(Vector3(0.0f,0.0f,1000.0f)));
		}
		else if (clearAnimationFrame == CLEAR_FRAME - 10)
		{
			GetManager().lock()->AddObjectFromCereal("ClearBand");
		}
		else if (clearAnimationFrame == CLEAR_FRAME - 30)
		{
			GetManager().lock()->AddObjectFromCereal("ClearText");
		}
		clearAnimationFrame--;
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

	wkp_stageNumber = GetManager().lock()->AddObjectFromCereal("ParentNumber", ObjectFactory::Create<Transform>(Vector3(-550, 350, 0), Vector3::Zero, Vector3(80, 80, 0)));
	auto numComponent = wkp_stageNumber.lock()->GetGameComponent<NumberComponent>();
	numComponent->SetNumber(StageSelect::GetStageNum());

	shp_map->PutTile();

	clearAnimationFrame = CLEAR_FRAME;

	mode = GameMode::Normal;
}

void ButiEngine::StageManager::ShowGUI()
{
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
	if (clearAnimationFrame < 0)
	{
		shp_map->DestoryBlock();
		ChangeScene("StageSelect");
		int nextStageNum = StageSelect::GetStageNum() + 1;
		StageSelect::SetStageNum(nextStageNum);
	}
	if (GameDevice::GetInput()->TriggerKey(Keys::C))
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
			//auto line = wkp_player.lock()->GetGameComponent<Player>()->GetPredictionLine();
			//line.lock()->transform->SetBaseTransform(nullptr);

			//Vector3 scale = line.lock()->transform->GetLocalScale();
			//Vector3 playerScale = wkp_player.lock()->transform->GetWorldScale();
			//scale *= playerScale;
			//line.lock()->transform->SetLocalScale(scale);

			shp_cameraController->ZoomOut();
			shp_panelManager->ResetMoveNum();
			shp_panelManager->ResetMoveHistories();
		}
		else
		{
			mode = GameMode::Normal;
			//auto line = wkp_player.lock()->GetGameComponent<Player>()->GetPredictionLine();
			//line.lock()->transform->SetBaseTransform(wkp_player.lock()->transform);
			//Vector3 scale = line.lock()->transform->GetLocalScale();
			//Vector3 playerScale = wkp_player.lock()->transform->GetWorldScale();
			//scale.x /= playerScale.x;
			//scale.y /= playerScale.y;
			//scale.z /= playerScale.z;
			//line.lock()->transform->SetLocalScale(scale);
			//Vector3 position = Vector3(20, 0, -1);
			//line.lock()->transform->SetLocalPosition(position);
			shp_cameraController->ZoomIn();
		}
	}
}
