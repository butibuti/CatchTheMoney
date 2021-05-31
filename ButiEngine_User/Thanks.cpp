#include "stdafx_u.h"
#include "Thanks.h"
#include "InputManager.h"
#include "SceneChangeAnimation.h"
#include "TitleLogo.h"
#include "GameSettings.h"
#include "CameraController.h"
#include"PauseManager.h"
#include"ThanksPlayer.h"
const float angleUnit =360.0f/20.0f;

void ButiEngine::Thanks::OnUpdate()
{
	bgmsize -= 0.01f;
	if (bgmsize > 0.0f) {

		GetManager().lock()->GetApplication().lock()->GetSoundManager()->SetBGMVolume(GameSettings::masterVolume * 0.1f* bgmsize);
	}
	else {

		GetManager().lock()->GetApplication().lock()->GetSoundManager()->StopBGM();
	}

	if (shp_appleGenerateTimer->Update()) {
		Vector3 pos = Vector3(0, 400*(1.0f-appleRadius), 0) * Matrix4x4::RollZ(MathHelper::ToRadian( -angleUnit * appleCount));
		auto apple= GetManager().lock()->AddObjectFromCereal("Apple", ObjectFactory::Create<Transform>(Vector3(pos.x, pos.y,1.0f+ 0.01*appleCount), Vector3::Zero, Vector3(0, 0, 1)));
		auto anim = apple.lock()->GetGameComponent<TransformAnimation>();
		anim->SetInitTransform(apple.lock()->transform->Clone());
		anim->SetTargetTransform(apple.lock()->transform->Clone());
		anim->GetTargetTransform()->SetLocalScale(Vector3(150,150,1));
		appleCount++;

		vec_apples.push_back(apple);
		apple.lock()->transform->SetBaseTransform(shp_appleParent->transform, true);

		if (appleCount >= 20) {
			shp_appleGenerateTimer->Stop();

			shp_appleGather->Start();
		}
		GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_apple,GameSettings::masterVolume);
	}

	if (shp_appleGather->Update()) {
		shp_appleRotate->Start();
		shp_appleGather->Stop();

		GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_zoom, GameSettings::masterVolume*2);
	}

	if (shp_appleRotate->IsOn()) {

		appleRadius += 1.0f / 60.0f;
		appleAngle += 3.0f;
		float t = Easing::EaseInBack(appleRadius);

		for (int i = 0; i < vec_apples.size(); i++) {
			Vector3 pos = Vector3(0, 400 * (1.0f - t), 0) * Matrix4x4::RollZ(MathHelper::ToRadian(-angleUnit * i+ appleAngle));
			vec_apples[i].lock()->transform->SetLocalPostionX(pos.x);
			vec_apples[i].lock()->transform->SetLocalPostionY(pos.y);
		}

	}

	if (shp_appleRotate->Update()) {
		auto anim = shp_appleParent->AddGameComponent<TransformAnimation>();
		anim->SetTargetTransform(shp_appleParent->transform->Clone());
		anim->GetTargetTransform()->SetLocalScale(Vector3(0, 0, 0));
		anim->SetSpeed(1.0f / 30.0f);
		anim->SetEaseType(Easing::EasingType::EaseInBack);
		shp_appleRotate->Stop();
	}


	if (shp_animationTimer->Update()) {
		animationEnd = true;

		GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_exp, GameSettings::masterVolume );
		shp_FrameGenerateTimer->Start();
		GenerateBackGroundFrame();
	}

	if (InputManager::OnTriggerDecisionKey() && animationEnd)
	{
		shp_endTimer->Start();

		GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_enter, GameSettings::masterVolume * 2);
		wkp_mouse.lock()->GetGameComponent<ThanksPlayer>()->Decision();

	}

	if (shp_endTimer->Update()) {

		GetManager().lock()->AddObjectFromCereal("FadeObject2", ObjectFactory::Create<Transform>(Vector3(0, 1134, 0), Vector3::Zero, Vector3(2112, 1188, 1)));
		shp_transitionTimer->Start();
	}

	if (shp_transitionTimer->Update())
	{

		auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();
		sceneManager->LoadScene("Title");
		sceneManager->ChangeScene("Title");

	}
	if (shp_FrameGenerateTimer->Update()) {
		GenerateBackGroundFrame();
	}

	if (shp_textGenerateTimer->Update()) {
		auto thanksText = GetManager().lock()->AddObjectFromCereal("ThanksText");

		shp_textGenerateTimer->Stop();
	}
	if (shp_smileDaikokutenGenerateTimer->Update()) {
		wkp_mouse = GetManager().lock()->AddObjectFromCereal("SmileDaikokuten");
		
		shp_smileDaikokutenGenerateTimer->Stop();
	}
	if (shp_mouseGenerateTimer->Update()) {
		wkp_mouse = GetManager().lock()->AddObjectFromCereal("AnimationPlayer_Thanks");
		shp_mouseGenerateTimer->Stop();
	}

}

void ButiEngine::Thanks::OnSet()
{
}

void ButiEngine::Thanks::Start()
{
	shp_PauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManager>();

	

	nextFlag = false;

	//wkp_fadeObject = GetManager().lock()->AddObjectFromCereal("FadeObject", ObjectFactory::Create<Transform>(Vector3(0, 0, 0), Vector3::Zero, Vector3(1920, 1080, 1)));

	bgm = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/BGM1.wav");
	se_apple = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/Clear.wav");
	se_zoom = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/Title.wav");
	se_exp = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/ClearExp.wav");
	se_dash = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/Rat_Dash.wav");
	se_enter = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/Enter.wav");

	shp_appleParent = GetManager().lock()->AddObject(ObjectFactory::Create<Transform>(),"appleParent").lock();

	shp_FrameGenerateTimer = ObjectFactory::Create<RelativeTimer>(1);
	shp_transitionTimer = ObjectFactory::Create<RelativeTimer>(43);
	shp_animationTimer = ObjectFactory::Create<RelativeTimer>(430);
	shp_animationTimer->Start();
	shp_appleGenerateTimer = ObjectFactory::Create<RelativeTimer>(15);
	shp_appleGenerateTimer->Start();

	shp_textGenerateTimer = ObjectFactory::Create<RelativeTimer>(430); 
	shp_mouseGenerateTimer = ObjectFactory::Create<RelativeTimer>(430);
	shp_smileDaikokutenGenerateTimer = ObjectFactory::Create<RelativeTimer>(430);
	shp_textGenerateTimer->Start(); shp_mouseGenerateTimer->Start(); shp_smileDaikokutenGenerateTimer->Start();

	shp_endTimer= ObjectFactory::Create<RelativeTimer>(50);

	shp_appleGather = ObjectFactory::Create<RelativeTimer>(15);
	shp_appleRotate = ObjectFactory::Create<RelativeTimer>(60);
	shp_appleMin= ObjectFactory::Create<RelativeTimer>(15);
}

void ButiEngine::Thanks::OnShowUI()
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::Thanks::Clone()
{
    auto output = ObjectFactory::Create<Thanks>();
    return output;
}

void ButiEngine::Thanks::GenerateBackGroundFrame()
{
	for (int i = 0; i < 2; i++) {
		auto frame = GetManager().lock()->AddObjectFromCereal("BackGroundFrame_Clear").lock();


		frame->transform->SetLocalPostionZ(i * 0.01f);

	}
}
