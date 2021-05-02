#include "stdafx_u.h"
#include "Title.h"
#include "InputManager.h"

void ButiEngine::Title::OnUpdate()
{
	if (InputManager::OnGameStartKey())
	{
		auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();
		sceneManager->LoadScene("StageSelect");
		sceneManager->ChangeScene("StageSelect");
	}
}

void ButiEngine::Title::OnSet()
{
}

void ButiEngine::Title::Start()
{
}

void ButiEngine::Title::OnShowUI()
{
}

void ButiEngine::Title::OnCollision(std::weak_ptr<GameObject> arg_other)
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::Title::Clone()
{
	return ObjectFactory::Create<Title>();
}
