#include "stdafx_u.h"
#include "Title.h"
#include "InputManager.h"
#include "SceneChangeAnimation.h"
#include "TitleLogo.h"
#include "GameSettings.h"
#include "CameraController.h"

void ButiEngine::Title::OnUpdate()
{
	if (!wkp_camera.lock())
	{
		wkp_camera = GetManager().lock()->GetGameObject("Camera");
		return;
	}
	if (InputManager::OnGameStartKey() && !isAnimation)
	{
		GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_enter, 0.1f);
		wkp_titleLogo.lock()->GetGameComponent<TitleLogo>()->AnimationStart();
		wkp_camera.lock()->GetGameComponent<CameraController>()->TitleZoomOut();
		isAnimation = true;
	}
	if (isAnimation)
	{
		animationCount++;
	}
	if (animationCount > 90)
	{
		nextFlag = true;
	}

	if (nextFlag)
	{
		nextSceneCount++;
	}
	if (nextSceneCount == 1)
	{
		GetManager().lock()->AddObjectFromCereal("FadeObject2", ObjectFactory::Create<Transform>(Vector3(0, 1080, 0), Vector3::Zero, Vector3(1920, 1080, 1)));
	}

	if (nextSceneCount > 45)
	{
		GameSettings::isTitle = false;
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
	GameSettings::isTitle = true;
	isAnimation = false;
	nextFlag = false;
	animationCount = 0;
	nextSceneCount = 0;
	//wkp_fadeObject = GetManager().lock()->AddObjectFromCereal("FadeObject", ObjectFactory::Create<Transform>(Vector3(0, 0, 0), Vector3::Zero, Vector3(1920, 1080, 1)));
	wkp_titleLogo = GetManager().lock()->AddObjectFromCereal("TitleLogo", ObjectFactory::Create<Transform>(Vector3(0,100,0), Vector3::Zero, Vector3(1440, 810, 1)));
	GetManager().lock()->AddObjectFromCereal("TitleAbutton", ObjectFactory::Create<Transform>(Vector3(0, -180, -0.02f), Vector3::Zero, Vector3(180, 180, 1)));
	se_enter = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/Enter.wav");
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
