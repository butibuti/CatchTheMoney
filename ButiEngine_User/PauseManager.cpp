#include "stdafx_u.h"
#include "PauseManager.h"
#include"PauseButton.h"
#include"InputManager.h"
#include"ShakeComponent.h"
#include"Map.h"
#include"StageSelect.h"
#include"Header/GameObjects/DefaultGameComponent/PositionAnimationComponent.h"
#include"Header/GameObjects/DefaultGameComponent/ScaleAnimationComponent.h"
#include"Player.h"
#include "GameSettings.h"
#include"GameExitMenu.h"
#include"SquareParticleEmitter.h"

bool ButiEngine::PauseManager::pause;

void ButiEngine::PauseManager::OnUpdate()
{
	if (GameSettings::isTitle) { return; }
	if (GameSettings::isStageSelect) { return; }
	if (GameExitMenu::IsOpenMenu()) { return; }
	StorePlayer();
	if (wkp_player.lock() && wkp_player.lock()->GetGameComponent<Player>()->IsClear()) { return; }
	if (InputManager::OnTriggerOpenMenuKey() && !pushPauseKey)
	{
		GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_pick, GameSettings::masterVolume);
		pushPauseKey = true;
		if (!pause)
		{
			progress = 0;
			selectedButton = BACK;
			disappear = false;
			if (shp_particleEmitter) {
				shp_particleEmitter->SetIsPause(true);
			}
			AppearUI();

		}
		else
		{
			if (shp_particleEmitter) {
				shp_particleEmitter->SetIsPause(false);
			}
			DisappearUI();
		}
	}
	SwitchPause();
	ButtonAnimation();
	OnDecide();
	FadeUpdate();
}

void ButiEngine::PauseManager::OnSet()
{
}

void ButiEngine::PauseManager::Start()
{
	if (GameSettings::isStageSelect)
	{
		wkp_text = GetManager().lock()->AddObjectFromCereal("PauseText_1");
		wkp_background = GetManager().lock()->AddObjectFromCereal("PauseBackground_1");
		wkp_button_back = GetManager().lock()->AddObjectFromCereal("PauseButton_Back_1");
		wkp_button_reset = GetManager().lock()->AddObjectFromCereal("PauseButton_Reset_1");
		wkp_button_select = GetManager().lock()->AddObjectFromCereal("PauseButton_Select_1");
	}
	else
	{
		wkp_text = GetManager().lock()->AddObjectFromCereal("PauseText");
		wkp_background = GetManager().lock()->AddObjectFromCereal("PauseBackground");
		wkp_button_back = GetManager().lock()->AddObjectFromCereal("PauseButton_Back");
		wkp_button_reset = GetManager().lock()->AddObjectFromCereal("PauseButton_Reset");
		wkp_button_select = GetManager().lock()->AddObjectFromCereal("PauseButton_Select");
	}

	se_enter = SoundTag("Sound/Enter.wav");
	se_select = SoundTag("Sound/Select-Click.wav");
	se_pick = SoundTag("Sound/Pause.wav");

	initBGScale = Vector3(0.0f, 0.0f, 1.0f);
	defaultBGScale = Vector3(1920.0f, 1080.0f, 1.0f);
	initTextPos = wkp_text.lock()->transform->GetWorldPosition();
	defaultTextPos = Vector3(0.0f, 100.0f, -0.5f);

	pause = false;
	pause_ = false;
	pushPauseKey = false;
	progress = 0;
	selectedButton = BACK;
	isNext = false;
	fadeCount = 0;
	reset = false;
	disappear = false;

	auto emiitter = GetManager().lock()->GetGameObject("SquareParticleEmitter");
	if (emiitter.lock()) {
		shp_particleEmitter = emiitter.lock()->GetGameComponent  <SquareParticleEmitter>();
	}
	
}

void ButiEngine::PauseManager::OnShowUI()
{
}

void ButiEngine::PauseManager::OnCollision(std::weak_ptr<GameObject> arg_other)
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::PauseManager::Clone()
{
	return ObjectFactory::Create<PauseManager>();
}

void ButiEngine::PauseManager::SwitchPause()
{
	if (!pushPauseKey) { return; }
	if (pause_)
	{
		if (wkp_button_back.lock()->GetGameComponent<PositionAnimation>()) { return; }
		pause = false;
		pause_ = false;
		pushPauseKey = false;
	}
	else
	{
		pause = true;
		if (wkp_button_back.lock()->GetGameComponent<PositionAnimation>()) { return; }
		pause_ = true;
		pushPauseKey = false;
		progress = 0;
	}
}

void ButiEngine::PauseManager::ButtonAnimation()
{
	if (!pause || disappear || isNext) { return; }
	progress++;
	if (progress < ANIMATION_FRAME) { return; }

	if (InputManager::OnTriggerRightKey() || 
		InputManager::OnTriggerLeftKey() ||
		progress == ANIMATION_FRAME)
	{
		if (InputManager::OnTriggerRightKey())
		{
			GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_select, GameSettings::masterVolume);
			selectedButton++;
			if (selectedButton > SELECT)
			{
				selectedButton = BACK;
			}
		}
		else if (InputManager::OnTriggerLeftKey())
		{
			GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_select, GameSettings::masterVolume);
			selectedButton--;
			if (selectedButton < BACK)
			{
				selectedButton = SELECT;
			}
		}

		SelectButton();
	}
}

void ButiEngine::PauseManager::SelectButton()
{
	if (selectedButton == BACK)
	{
		wkp_button_back.lock()->GetGameComponent<PauseButton>()->OnSelected();
		wkp_button_reset.lock()->GetGameComponent<PauseButton>()->OnEndSelect();
		wkp_button_select.lock()->GetGameComponent<PauseButton>()->OnEndSelect();
	}
	else if (selectedButton == RESET)
	{
		wkp_button_back.lock()->GetGameComponent<PauseButton>()->OnEndSelect();
		wkp_button_reset.lock()->GetGameComponent<PauseButton>()->OnSelected();
		wkp_button_select.lock()->GetGameComponent<PauseButton>()->OnEndSelect();
	}
	else if (selectedButton == SELECT)
	{
		wkp_button_back.lock()->GetGameComponent<PauseButton>()->OnEndSelect();
		wkp_button_reset.lock()->GetGameComponent<PauseButton>()->OnEndSelect();
		wkp_button_select.lock()->GetGameComponent<PauseButton>()->OnSelected();
	}
}

void ButiEngine::PauseManager::OnDecide()
{
	if (!pause || disappear || isNext || pushPauseKey) { return; }
	if (progress < ANIMATION_FRAME) { return; }

	if (InputManager::OnTriggerDecisionKey())
	{
		GameSettings::isTutorialInit = false;
		GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_enter, GameSettings::masterVolume);
		if (selectedButton == BACK)
		{
			OnDecideBack();
		}
		else if (selectedButton == RESET)
		{
			OnDecideReset();
		}
		else if (selectedButton == SELECT)
		{
			OnDecideSelect();
		}
	}
}

void ButiEngine::PauseManager::OnDecideBack()
{
	wkp_button_back.lock()->GetGameComponent<ShakeComponent>()->ShakeStart(20.0f);

	pushPauseKey = true;
	if (shp_particleEmitter) {
		shp_particleEmitter->SetIsPause(false);
	}
	DisappearUI();
}

void ButiEngine::PauseManager::OnDecideReset()
{
	wkp_button_reset.lock()->GetGameComponent<ShakeComponent>()->ShakeStart(20.0f);
	isNext = true;
	reset = true;
}

void ButiEngine::PauseManager::OnDecideSelect()
{
	wkp_button_select.lock()->GetGameComponent<ShakeComponent>()->ShakeStart(20.0f);
	isNext = true;
	reset = false;
}

void ButiEngine::PauseManager::AppearUI()
{
	AddPositionAnimation(wkp_text, defaultTextPos,20, Easing::EasingType::EaseOutBack);
	AddScaleAnimation(wkp_background, defaultBGScale, 10, Easing::EasingType::EaseInQuad);
	wkp_button_back.lock()->GetGameComponent<PauseButton>()->Appear();
	wkp_button_reset.lock()->GetGameComponent<PauseButton>()->Appear();
	wkp_button_select.lock()->GetGameComponent<PauseButton>()->Appear();
}

void ButiEngine::PauseManager::DisappearUI()
{
	disappear = true;
	AddPositionAnimation(wkp_text, initTextPos, 20, Easing::EasingType::EaseInBack);
	AddScaleAnimation(wkp_background, initBGScale, 10, Easing::EasingType::EaseOutQuad);
	wkp_button_back.lock()->GetGameComponent<PauseButton>()->Disappear();
	wkp_button_reset.lock()->GetGameComponent<PauseButton>()->Disappear();
	wkp_button_select.lock()->GetGameComponent<PauseButton>()->Disappear();
}

void ButiEngine::PauseManager::FadeUpdate()
{
	if (!isNext) { return; }

	fadeCount++;
	if (fadeCount == 1)
	{
		GetManager().lock()->AddObjectFromCereal("FadeObject2", ObjectFactory::Create<Transform>(Vector3(0, 1134, -0.7f), Vector3::Zero, Vector3(2112, 1188, 1)));
	}
	if (fadeCount == 30)
	{
		GetManager().lock()->GetGameObject("Map").lock()->GetGameComponent<Map>()->DestoryBlock();
		if (reset)
		{
			auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();
			sceneManager->ReloadScene();
		}
		else
		{
			StageSelect::SetRemoveStageName("none");
			ChangeScene("StageSelect");
		}
	}
}

void ButiEngine::PauseManager::ChangeScene(const std::string& arg_sceneName)
{
	auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();
	sceneManager->RemoveScene(arg_sceneName);
	sceneManager->LoadScene(arg_sceneName);
	sceneManager->ChangeScene(arg_sceneName);
}

void ButiEngine::PauseManager::AddPositionAnimation(std::weak_ptr<GameObject> arg_object, const Vector3& arg_targetPosition, int frame, Easing::EasingType easingType)
{
	auto anim_ = arg_object.lock()->GetGameComponent<PositionAnimation>();
	if (anim_)
	{
		arg_object.lock()->RemoveGameComponent("PositionAnimation");
	}
	auto anim = arg_object.lock()->AddGameComponent<PositionAnimation>();

	anim->SetInitPosition(arg_object.lock()->transform->GetWorldPosition());
	anim->SetTargetPosition(arg_targetPosition);
	anim->SetSpeed(1.0f / frame);
	anim->SetEaseType(easingType);
}

void ButiEngine::PauseManager::AddScaleAnimation(std::weak_ptr<GameObject> arg_object, const Vector3& arg_targetScale, int frame, Easing::EasingType easingType)
{
	auto anim_ = arg_object.lock()->GetGameComponent<ScaleAnimation>();
	if (anim_)
	{
		arg_object.lock()->RemoveGameComponent("ScaleAnimation");
	}
	auto anim = arg_object.lock()->AddGameComponent<ScaleAnimation>();

	anim->SetInitScale(arg_object.lock()->transform->GetWorldScale());
	anim->SetTargetScale(arg_targetScale);
	anim->SetSpeed(1.0f / frame);
	anim->SetEaseType(easingType);
}

void ButiEngine::PauseManager::StorePlayer()
{
	if (!wkp_player.lock())
	{
		wkp_player = GetManager().lock()->GetGameObject("Player");
	}
}
