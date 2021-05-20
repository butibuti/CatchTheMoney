#include "stdafx_u.h"
#include "Title.h"
#include "InputManager.h"
#include "SceneChangeAnimation.h"
#include "TitleLogo.h"

void ButiEngine::Title::OnUpdate()
{
	if (InputManager::OnGameStartKey())
	{
		wkp_titleLogo.lock()->GetGameComponent<TitleLogo>()->AnimationStart();
		isAnimation = true;
	}
	if (isAnimation)
	{
		animationCount++;
	}
	if (animationCount > 120)
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
		//sceneManager->LoadScene("StageSelect");
		//sceneManager->ChangeScene("StageSelect");
		sceneManager->LoadScene("Stage0");
		sceneManager->ChangeScene("Stage0");
	}
}

void ButiEngine::Title::OnSet()
{
}

void ButiEngine::Title::Start()
{
	isAnimation = false;
	nextFlag = false;
	animationCount = 0;
	nextSceneCount = 0;
	//wkp_fadeObject = GetManager().lock()->AddObjectFromCereal("FadeObject", ObjectFactory::Create<Transform>(Vector3(0, 0, 0), Vector3::Zero, Vector3(1920, 1080, 1)));
	wkp_titleLogo = GetManager().lock()->AddObjectFromCereal("TitleLogo", ObjectFactory::Create<Transform>(Vector3::Zero, Vector3::Zero, Vector3(960, 540, 1)));
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
