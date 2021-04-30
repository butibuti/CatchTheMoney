#include "stdafx_u.h"
#include "StageManager.h"
#include"Map.h"
#include"PauseManager.h"

void ButiEngine::StageManager::OnUpdate()
{
	//ƒNƒŠƒA‚µ‚½‚ç
	if (GameDevice::GetInput()->TriggerKey(Keys::C))
	{
		shp_map->DestoryBlock();
		auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();
		std::string nextSceneName = "StageSelect";
		sceneManager->RemoveScene(nextSceneName);
		sceneManager->LoadScene(nextSceneName);
		sceneManager->ChangeScene(nextSceneName);
	}

	ModeChange();
}

void ButiEngine::StageManager::OnSet()
{
}

void ButiEngine::StageManager::Start()
{
	shp_map = GetManager().lock()->GetGameObject("Map").lock()->GetGameComponent<Map>();
	shp_pauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManager>();

	shp_map->PutTile();

	mode = GameMode::Action;
}

void ButiEngine::StageManager::OnCollision(std::weak_ptr<GameObject> arg_other)
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::StageManager::Clone()
{
	return ObjectFactory::Create<StageManager>();
}

void ButiEngine::StageManager::ModeChange()
{
	if (GameDevice::GetInput()->TriggerKey(Keys::Space))
	{
		shp_pauseManager->SwitchPause();
		
		if (mode = GameMode::Action)
		{
			mode = GameMode::Edit;
		}
		else
		{
			mode = GameMode::Action;
		}
	}
}
