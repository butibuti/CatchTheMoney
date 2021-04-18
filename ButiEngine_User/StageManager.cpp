#include "stdafx_u.h"
#include "StageManager.h"
#include"Map.h"

void ButiEngine::StageManager::OnUpdate()
{
	if (GameDevice::GetInput()->TriggerKey(Keys::C))
	{
		shp_map->DestoryBlock();
		auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();
		std::string nextSceneName = "StageSelect";
		sceneManager->RemoveScene(nextSceneName);
		sceneManager->LoadScene(nextSceneName);
		sceneManager->ChangeScene(nextSceneName);
	}
}

void ButiEngine::StageManager::OnSet()
{
}

void ButiEngine::StageManager::Start()
{
	shp_map = GetManager().lock()->GetGameObject("Map").lock()->GetGameComponent<Map>();
	shp_map->PutBlock();
}

void ButiEngine::StageManager::OnCollision(std::weak_ptr<GameObject> arg_other)
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::StageManager::Clone()
{
	return ObjectFactory::Create<StageManager>();
}
