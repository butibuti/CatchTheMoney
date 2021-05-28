#include "stdafx_u.h"
#include "GameExitMenu.h"
#include"PauseButton.h"
#include"ShakeComponent.h"
#include"InputManager.h"
#include"GameSettings.h"
#include"Header/GameObjects/DefaultGameComponent/PositionAnimationComponent.h"
#include"Header/GameObjects/DefaultGameComponent/ScaleAnimationComponent.h"
#include"PauseManager.h"
#include"Player.h"
#include"Title.h"
#include"StageSelect.h"

bool ButiEngine::GameExitMenu::open;

void ButiEngine::GameExitMenu::OnUpdate()
{
	StorePlayer();
	//if (GameSettings::isTitle) { return; }
	if (wkp_player.lock() && wkp_player.lock()->GetGameComponent<Player>()->IsClear()) { return; }
	if (Title::IsAnimation()) { return; }
	if (StageSelect::IsAnimation()) { return; }
	if (InputManager::OnTriggerOpenGameExitMenuKey() && !pushMenuKey)
	{
		pushMenuKey = true;
		if (!open)
		{
			if (shp_pauseManager && shp_pauseManager->IsPause())
			{
				pushMenuKey = false;
				return; 
			}
			GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_pick, GameSettings::masterVolume);
			progress = 0;
			selectYesButton = false;
			disappear = false;
			AppearUI();
		}
		else
		{
			GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_pick, GameSettings::masterVolume);
			DisappearUI();
		}
	}
	SwitchPause();
	ButtonAnimation();
	OnDecide();
	Exit();
}

void ButiEngine::GameExitMenu::OnSet()
{
}

void ButiEngine::GameExitMenu::Start()
{
	shp_pauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManager>();

	if (GameSettings::isStageSelect)
	{
		wkp_text = GetManager().lock()->AddObjectFromCereal("GameExitMenuText_1");
		wkp_background = GetManager().lock()->AddObjectFromCereal("GameExitMenuBackground_1");
		wkp_button_yes = GetManager().lock()->AddObjectFromCereal("GameExitMenuButton_Yes_1");
		wkp_button_no = GetManager().lock()->AddObjectFromCereal("GameExitMenuButton_No_1");
	}
	else
	{
		wkp_text = GetManager().lock()->AddObjectFromCereal("GameExitMenuText");
		wkp_background = GetManager().lock()->AddObjectFromCereal("GameExitMenuBackground");
		wkp_button_yes = GetManager().lock()->AddObjectFromCereal("GameExitMenuButton_Yes");
		wkp_button_no = GetManager().lock()->AddObjectFromCereal("GameExitMenuButton_No");
	}

	se_enter = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/Enter.wav");
	se_select = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/Select-Click.wav");
	se_pick = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/Pause.wav");

	initBGScale = Vector3(0.0f, 0.0f, 1.0f);
	defaultBGScale = Vector3(1920.0f, 1080.0f, 1.0f);
	initTextPos = wkp_text.lock()->transform->GetWorldPosition();
	defaultTextPos = Vector3(0.0f, 100.0f, -0.5f);

	open = false;
	open_ = false;
	pushMenuKey = false;
	progress = 0;
	selectYesButton = false;
	exit = false;
	disappear = false;
}

void ButiEngine::GameExitMenu::OnShowUI()
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::GameExitMenu::Clone()
{
	return ObjectFactory::Create<GameExitMenu>();
}

void ButiEngine::GameExitMenu::SwitchPause()
{
	if (!pushMenuKey) { return; }
	if (open_)
	{
		if (wkp_button_yes.lock()->GetGameComponent<PositionAnimation>()) { return; }
		open = false;
		shp_pauseManager->SetPause(false);
		open_ = false;
		pushMenuKey = false;
	}
	else
	{
		shp_pauseManager->SetPause(true);
		open = true;
		if (wkp_button_yes.lock()->GetGameComponent<PositionAnimation>()) { return; }
		open_ = true;
		pushMenuKey = false;
		progress = 0;
	}
}

void ButiEngine::GameExitMenu::ButtonAnimation()
{
	if (!open || disappear || exit) { return; }
	progress++;
	if (progress < ANIMATION_FRAME) { return; }

	if (InputManager::OnTriggerRightKey() ||
		InputManager::OnTriggerLeftKey() ||
		progress == ANIMATION_FRAME)
	{
		if (InputManager::OnTriggerRightKey() && !selectYesButton)
		{
			GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_select, GameSettings::masterVolume);
			selectYesButton = true;
		}
		else if (InputManager::OnTriggerLeftKey() && selectYesButton)
		{
			GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_select, GameSettings::masterVolume);
			selectYesButton = false;
		}

		SelectButton();
	}
}

void ButiEngine::GameExitMenu::SelectButton()
{
	if (selectYesButton)
	{
		wkp_button_yes.lock()->GetGameComponent<PauseButton>()->OnSelected();
		wkp_button_no.lock()->GetGameComponent<PauseButton>()->OnEndSelect();
	}
	else
	{
		wkp_button_yes.lock()->GetGameComponent<PauseButton>()->OnEndSelect();
		wkp_button_no.lock()->GetGameComponent<PauseButton>()->OnSelected();
	}
}

void ButiEngine::GameExitMenu::OnDecide()
{
	if (!open || disappear || pushMenuKey || exit) { return; }
	if (progress < ANIMATION_FRAME) { return; }

	if (InputManager::OnTriggerDecisionKey())
	{
		GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_enter, GameSettings::masterVolume);
		if (selectYesButton)
		{
			OnDecideYes();
		}
		else
		{
			OnDecideNo();
		}
	}
}

void ButiEngine::GameExitMenu::OnDecideNo()
{
	wkp_button_no.lock()->GetGameComponent<ShakeComponent>()->ShakeStart(20.0f);

	pushMenuKey = true;
	DisappearUI();
}

void ButiEngine::GameExitMenu::OnDecideYes()
{
	wkp_button_yes.lock()->GetGameComponent<ShakeComponent>()->ShakeStart(20.0f);
	exit = true;
}

void ButiEngine::GameExitMenu::AppearUI()
{
	AddPositionAnimation(wkp_text, defaultTextPos, 20, Easing::EasingType::EaseOutBack);
	AddScaleAnimation(wkp_background, defaultBGScale, 10, Easing::EasingType::EaseInQuad);
	wkp_button_yes.lock()->GetGameComponent<PauseButton>()->Appear();
	wkp_button_no.lock()->GetGameComponent<PauseButton>()->Appear();
}

void ButiEngine::GameExitMenu::DisappearUI()
{
	disappear = true;
	AddPositionAnimation(wkp_text, initTextPos, 20, Easing::EasingType::EaseInBack);
	AddScaleAnimation(wkp_background, initBGScale, 10, Easing::EasingType::EaseOutQuad);
	wkp_button_yes.lock()->GetGameComponent<PauseButton>()->Disappear();
	wkp_button_no.lock()->GetGameComponent<PauseButton>()->Disappear();
}

void ButiEngine::GameExitMenu::AddPositionAnimation(std::weak_ptr<GameObject> arg_object, const Vector3& arg_targetPosition, int frame, Easing::EasingType easingType)
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

void ButiEngine::GameExitMenu::AddScaleAnimation(std::weak_ptr<GameObject> arg_object, const Vector3& arg_targetScale, int frame, Easing::EasingType easingType)
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

void ButiEngine::GameExitMenu::StorePlayer()
{
	if (!wkp_player.lock())
	{
		wkp_player = GetManager().lock()->GetGameObject("Player");
	}
}

void ButiEngine::GameExitMenu::Exit()
{
	if (!exit) { return; }
	
	exitProgressFrame++;
	if (exitProgressFrame > EXIT_WAIT_FRAME)
	{
		GameDevice::SetIsEnd(true);
	}
}
