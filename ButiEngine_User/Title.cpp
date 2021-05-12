#include "stdafx_u.h"
#include "Title.h"
#include "InputManager.h"
#include "SceneChangeAnimation.h"

void ButiEngine::Title::OnUpdate()
{
	if (InputManager::OnGameStartKey())
	{
		nextFlag = true;
	}
	if (nextFlag)
	{
		nextSceneCount++;
	}
	if (nextSceneCount == 1)
	{
		GetManager().lock()->AddObjectFromCereal("FadeObject", ObjectFactory::Create<Transform>(Vector3(0, 1080, 0), Vector3::Zero, Vector3(1920, 1080, 1)));
	}

	if (nextSceneCount > 45)
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
	nextFlag = false;
	wkp_fadeObject = GetManager().lock()->AddObjectFromCereal("FadeObject", ObjectFactory::Create<Transform>(Vector3(0, 0, 0), Vector3::Zero, Vector3(1920, 1080, 1)));
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
