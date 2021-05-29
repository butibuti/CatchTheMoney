#include "stdafx_u.h"
#include "StageSelect.h"
#include "ParentSelectPanel.h"
#include "InputManager.h"
#include "SelectScreen.h"
#include "ShakeComponent.h"
#include "SelectPlayer.h"
#include "SceneChangeAnimation.h"
#include "GameSettings.h"
#include"PauseManager.h"
#include"Header/GameObjects/DefaultGameComponent/OutlineDrawComponent.h"
int ButiEngine::StageSelect::stageNum = 0;
int ButiEngine::StageSelect::maxStageNum = 23; //LastStageNum - 1  "rewrite to ParentSelectPanel::stageCount"
std::string ButiEngine::StageSelect::removeStageName = "none";
bool ButiEngine::StageSelect::isAnimation;

int count=0;
const float angle = 360.0f / (float)(ButiEngine::StageSelect::maxStageNum + 1) * 2.0f;
void ButiEngine::StageSelect::OnUpdate()
{
	Onece();
	const auto childAngle = 180.0f / (ButiEngine::StageSelect::maxStageNum + 1) * 2.0f;
	auto parentSelectPanel = wkp_parentSelectPanel.lock()->GetGameComponent<ParentSelectPanel>();
	if (intervalFrame > 20 && !isAnimation)
	{
		if (InputManager::OnPushRightKey() && !shp_pauseManager->IsPause())
		{
			count++;
			intervalFrame = 0;
			OnPushRight();
			parentSelectPanel->ChildRotation(-childAngle,stageNum);
			auto screen = GetManager().lock()->GetGameObject("SelectScreen").lock();
			screen->GetGameComponent<ShakeComponent>()->ShakeStart(20.0f);
			std::string materialSource = "StageSelect_";
			if (stageNum<10)
			{
				materialSource += "0";
			}
			screen->GetGameComponent<OutlineMeshDrawComponent>()->SetMaterialTag(MaterialTag(materialSource + std::to_string(stageNum)), 0);
			screen->GetGameComponent<OutlineMeshDrawComponent>()->ReRegist();
		}
		else if (InputManager::OnPushLeftKey() && !shp_pauseManager->IsPause())
		{
			count++;
			intervalFrame = 0;
			OnPushLeft();
			parentSelectPanel->ChildRotation(childAngle,stageNum);
			auto screen = GetManager().lock()->GetGameObject("SelectScreen").lock();
			screen->GetGameComponent<ShakeComponent>()->ShakeStart(20.0f); std::string materialSource = "StageSelect_";
			if (stageNum < 10)
			{
				materialSource += "0";
			}
			screen->GetGameComponent<OutlineMeshDrawComponent>()->SetMaterialTag(MaterialTag(materialSource + std::to_string(stageNum)),0);
			screen->GetGameComponent<OutlineMeshDrawComponent>()->ReRegist();
		}
		if (InputManager::OnSkipKey() && !shp_pauseManager->IsPause())
		{
			count++;
			intervalFrame = 0;
			OnPushSkip();
			parentSelectPanel->ChildRotation(-childAngle * ((maxStageNum + 1) / 2),stageNum);


			auto screen = GetManager().lock()->GetGameObject("SelectScreen").lock();
			screen->GetGameComponent<ShakeComponent>()->ShakeStart(20.0f); std::string materialSource = "StageSelect_";
			if (stageNum < 10)
			{
				materialSource += "0";
			}
			screen->GetGameComponent<OutlineMeshDrawComponent>()->SetMaterialTag(MaterialTag(materialSource + std::to_string(stageNum)),0);
			screen->GetGameComponent<OutlineMeshDrawComponent>()->ReRegist();
		}
	}
	else
	{
		intervalFrame++;
	}

	DecisionAnimation();

	SelectRotation();

	
}

void ButiEngine::StageSelect::OnSet()
{
}

void ButiEngine::StageSelect::Start()
{
	shp_pauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManager>();

	GameSettings::isStageSelect = true;
	isOnece = false;
	isAnimation = false;
	animationFrame = 90;
	intervalFrame = 0;
	fadeCount = 0;
	wkp_parentSelectPanel = GetManager().lock()->GetGameObject("ParentSelectPanel");
	wkp_animationPlayer = GetManager().lock()->AddObjectFromCereal("AnimationPlayer");

	wkp_fadeObject = GetManager().lock()->AddObjectFromCereal("FadeObject", ObjectFactory::Create<Transform>(Vector3(0, 0, 0), Vector3::Zero, Vector3(1920, 1080, 1)));

	preParentRotation = stageNum * angle;

	auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();
	if (stageNum <= 0)
	{
		std::string sceneName = "Stage" + std::to_string(maxStageNum);
		sceneManager->RemoveScene(sceneName);
	}
	else
	{
		int preStageNum = stageNum - 1;
		std::string sceneName = "Stage" + std::to_string(preStageNum);
		sceneManager->RemoveScene(sceneName);
	}

	se_enter = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/Enter.wav");
	se_select = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/Select-Click.wav");
	se_dash = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/Rat_Dash.wav");
	se_hit = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/Rat_Hit.wav");
	se_start = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/Rat_Start.wav");
	bgm = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/BGM2.wav");
	GetManager().lock()->GetApplication().lock()->GetSoundManager()->StopBGM();
	GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlayBGM(bgm, GameSettings::masterVolume + 0.5f);
	auto screen = GetManager().lock()->GetGameObject("SelectScreen").lock();
	std::string materialSource = "StageSelect_";
	if (stageNum < 10)
	{
		materialSource += "0";
	}
	screen->GetGameComponent<OutlineMeshDrawComponent>()->SetMaterialTag(MaterialTag(materialSource + std::to_string(stageNum)), 0);
	screen->GetGameComponent<OutlineMeshDrawComponent>()->ReRegist();
}

void ButiEngine::StageSelect::OnShowUI()
{
}

void ButiEngine::StageSelect::ShowGUI()
{
	GUI::Begin("AnimationFrame");
	GUI::Text(animationFrame);
	GUI::End();

	GUI::Begin("StageNum");
	GUI::BulletText("StageNum");
	GUI::Text(stageNum);
	GUI::BulletText("Count");
	GUI::Text(count);
	GUI::End();
}

void ButiEngine::StageSelect::OnCollision(std::weak_ptr<GameObject> arg_other)
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::StageSelect::Clone()
{
	return ObjectFactory::Create<StageSelect>();
}

void ButiEngine::StageSelect::SetStageNum(int arg_stageNum)
{
	stageNum = arg_stageNum;
	if (stageNum > maxStageNum)
	{
		stageNum = 0;
	}
}

void ButiEngine::StageSelect::SetRemoveStageName(std::string arg_removeStageName)
{
	removeStageName = arg_removeStageName;
}

void ButiEngine::StageSelect::OnPushRight()
{
	GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_select, GameSettings::masterVolume);

	stageNum++;
	if (stageNum > maxStageNum)
	{
		stageNum = 0;
	}
	preParentRotation = angle * stageNum;
}

void ButiEngine::StageSelect::OnPushLeft()
{
	GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_select, GameSettings::masterVolume);

	stageNum--;
	if (stageNum < 0)
	{
		stageNum = maxStageNum;
	}
	preParentRotation = angle * stageNum;
}

void ButiEngine::StageSelect::OnPushSkip()
{
	GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_select, GameSettings::masterVolume);

	stageNum += (maxStageNum + 1) / 2;
	if (stageNum > maxStageNum)
	{
		stageNum = stageNum - maxStageNum - 1;
	}
	preParentRotation = angle * stageNum;
}

void ButiEngine::StageSelect::OnDecision()
{
	GameSettings::isStageSelect = false;
	isAnimation = false;
	auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();
	std::string sceneName = "Stage" + std::to_string(stageNum);
	sceneManager->RemoveScene(sceneName);
	sceneManager->LoadScene(sceneName);
	sceneManager->ChangeScene(sceneName);
}

void ButiEngine::StageSelect::DecisionAnimation()
{
	if (InputManager::OnTriggerDecisionKey() && !isAnimation && !shp_pauseManager->IsPause())
	{
		GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_enter, GameSettings::masterVolume);
		isAnimation = true;
	}

	if (!isAnimation) return;

	if (animationFrame == screenRotateFrame)
	{
		GetManager().lock()->GetGameObject("SelectScreen").lock()->GetGameComponent<SelectScreen>()->StartAnimation();
	}

	if (animationFrame == 89)
	{
		wkp_animationPlayer.lock()->GetGameComponent<SelectPlayer>()->Decision();
	}
	else if (animationFrame == 60)
	{
		GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_dash, GameSettings::masterVolume);
		GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_start, GameSettings::masterVolume);
		GetManager().lock()->AddObjectFromCereal("SelectZanzo");
	}
	else if (animationFrame == 40)
	{
		GetManager().lock()->AddObjectFromCereal("SelectFlash");
		GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_hit, GameSettings::masterVolume);
		GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_dash, GameSettings::masterVolume);
	}
	else if (animationFrame == 35)
	{
		wkp_animationPlayer.lock()->GetGameComponent<SelectPlayer>()->Away();
	}

	if (animationFrame <= 0)
	{
		isNext = true;
	}

	if (isNext)
	{
		fadeCount++;
	}
	if (fadeCount == 1)
	{
		GetManager().lock()->AddObjectFromCereal("FadeObject", ObjectFactory::Create<Transform>(Vector3(0, 1080, 0), Vector3::Zero, Vector3(1920, 1080, 1)));
	}

	if (fadeCount > 30)
	{
		OnDecision();
	}

	if (animationFrame > 0)
	{
		animationFrame--;
	}
}

void ButiEngine::StageSelect::SelectRotation()
{
	auto anim = wkp_parentSelectPanel.lock()->GetGameComponent<TransformAnimation>();
	if (!anim)
	{
		anim = wkp_parentSelectPanel.lock()->AddGameComponent<TransformAnimation>();
		anim->SetTargetTransform(wkp_parentSelectPanel.lock()->transform->Clone());
		anim->GetTargetTransform()->SetLocalRotation(Matrix4x4::RollY(MathHelper::ToRadian( preParentRotation)));
		anim->SetSpeed(0.1f);
		anim->SetEaseType(Easing::EasingType::EaseOut);
	}
}

void ButiEngine::StageSelect::Onece()
{
	if (isOnece) return;
	isOnece = true;

	auto parentSelectPanel = wkp_parentSelectPanel.lock()->GetGameComponent<ParentSelectPanel>();
	auto childAngle = 180.0f / (maxStageNum + 1) * 2.0f;
	float rotate = childAngle * stageNum;
	preParentRotation = stageNum * angle;

	parentSelectPanel->ChildRotation(-rotate,stageNum);
}
