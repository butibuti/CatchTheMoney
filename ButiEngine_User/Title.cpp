#include "stdafx_u.h"
#include "Title.h"
#include "InputManager.h"
#include "SceneChangeAnimation.h"
#include "TitleLogo.h"
#include "GameSettings.h"
#include "CameraController.h"
#include"PauseManager.h"

bool ButiEngine::Title::isAnimation;

void ButiEngine::Title::OnUpdate()
{
	if (shp_PauseManager->IsPause()) { return; }
	if (!wkp_camera.lock())
	{
		wkp_camera = GetManager().lock()->GetGameObject("Camera");
		return;
	}
	if (!isOnce)
	{
		isOnce = true;
		wkp_camera.lock()->transform->SetLocalPosition(Vector3(60, 315, -280));
	}

	if (InputManager::OnTriggerDecisionKey() && !isAnimation)
	{
		GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_enter, GameSettings::masterVolume);
		GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_zoomOut, GameSettings::masterVolume * 3.0f);
		wkp_titleLogo.lock()->GetGameComponent<TitleLogo>()->AnimationStart();
		wkp_camera.lock()->GetGameComponent<CameraController>()->TitleZoomOut();
		shp_FrameGenerateTimer->Start();
		GenerateBackGroundFrame();
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
		isAnimation = false;
		auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();
		sceneManager->LoadScene("StageSelect");
		sceneManager->ChangeScene("StageSelect");
		//sceneManager->LoadScene("Stage0");
		//sceneManager->ChangeScene("Stage0");
	}
	if (shp_FrameGenerateTimer->Update()) {
		GenerateBackGroundFrame();
	}
}

void ButiEngine::Title::OnSet()
{
}

void ButiEngine::Title::Start()
{
	shp_PauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManager>();

	GameSettings::isTitle = true;
	isAnimation = false;
	nextFlag = false;
	isOnce = false;
	animationCount = 0;
	nextSceneCount = 0;
	//wkp_fadeObject = GetManager().lock()->AddObjectFromCereal("FadeObject", ObjectFactory::Create<Transform>(Vector3(0, 0, 0), Vector3::Zero, Vector3(1920, 1080, 1)));
	wkp_titleLogo = GetManager().lock()->AddObjectFromCereal("TitleLogo", ObjectFactory::Create<Transform>(Vector3(0,100,0), Vector3::Zero, Vector3(1440, 810, 1)));
	GetManager().lock()->AddObjectFromCereal("TitleAbutton", ObjectFactory::Create<Transform>(Vector3(0, -180, -0.02f), Vector3::Zero, Vector3(180, 180, 1)));
	
	bgm = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/BGM1.wav");
	se_enter = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/Enter.wav");
	se_zoomOut = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/Title.wav");

	GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlayBGM(bgm, GameSettings::masterVolume + 0.3f);

	auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();
	sceneManager->LoadScene("Stage0");
	shp_FrameGenerateTimer = ObjectFactory::Create<RelativeTimer>(1);
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

void ButiEngine::Title::GenerateBackGroundFrame()
{
	for (int i = 0; i <2; i++) {
		auto frame= GetManager().lock()->AddObjectFromCereal("BackGroundFrame_Title").lock();


		frame->transform->SetLocalPostionZ(i * 0.01f);

	}
}
