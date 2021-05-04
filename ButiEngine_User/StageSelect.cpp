#include "stdafx_u.h"
#include "StageSelect.h"
#include "ParentSelectPanel.h"
#include "InputManager.h"
#include "SelectScreen.h"
#include "ShakeComponent.h"

int ButiEngine::StageSelect::stageNum = 0;
int ButiEngine::StageSelect::maxStageNum = 19; //LastStageNum - 1  "rewrite to ParentSelectPanel::stageCount"

void ButiEngine::StageSelect::OnUpdate()
{
	Onece();
	auto childAngle = 180.0f / (maxStageNum + 1) * 2.0f;
	auto parentSelectPanel = wkp_parentSelectPanel.lock()->GetGameComponent<ParentSelectPanel>();
	if (intervalFrame > 20 && !isAnimation)
	{
		if (InputManager::OnPushRightKey())
		{
			intervalFrame = 0;
			OnPushRight();
			parentSelectPanel->ChildRotation(-childAngle);
			GetManager().lock()->GetGameObject("SelectScreen").lock()->GetGameComponent<ShakeComponent>()->ShakeStart(20.0f);
		}
		else if (InputManager::OnPushLeftKey())
		{
			intervalFrame = 0;
			OnPushLeft();
			parentSelectPanel->ChildRotation(childAngle);
			GetManager().lock()->GetGameObject("SelectScreen").lock()->GetGameComponent<ShakeComponent>()->ShakeStart(20.0f);
		}
		if (InputManager::OnSkipKey())
		{
			intervalFrame = 0;
			OnPushSkip();
			parentSelectPanel->ChildRotation(-childAngle * ((maxStageNum + 1) / 2));
			GetManager().lock()->GetGameObject("SelectScreen").lock()->GetGameComponent<ShakeComponent>()->ShakeStart(20.0f);
		}
	}
	else
	{
		intervalFrame++;
	}

	DecisionAnimation();

	SelectRotation();

	GetManager().lock()->GetApplication().lock()->GetGUIController()->SetGUIObject(GetThis<StageSelect>());
}

void ButiEngine::StageSelect::OnSet()
{
}

void ButiEngine::StageSelect::Start()
{
	isOnece = false;
	isAnimation = false;
	animationFrame = 90;
	intervalFrame = 0;
	wkp_parentSelectPanel = GetManager().lock()->GetGameObject("ParentSelectPanel");

	preParentRotation = Vector3::Zero;
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
	GUI::Text(stageNum);
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

void ButiEngine::StageSelect::OnPushRight()
{
	float angle = 360.0f / (float)(maxStageNum + 1) * 2.0f;
	preParentRotation.y += angle;

	stageNum++;
	if (stageNum > maxStageNum)
	{
		stageNum = 0;
	}
}

void ButiEngine::StageSelect::OnPushLeft()
{
	float angle = 360.0f / (float)(maxStageNum + 1) * 2.0f;
	preParentRotation.y -= angle;

	stageNum--;
	if (stageNum < 0)
	{
		stageNum = maxStageNum;
	}
}

void ButiEngine::StageSelect::OnPushSkip()
{
	float angle = 360.0f / (float)(maxStageNum + 1) * 2.0f;
	preParentRotation.y += angle * ((maxStageNum + 1) / 2);

	stageNum += (maxStageNum + 1) / 2;
	if (stageNum > maxStageNum)
	{
		stageNum = stageNum - maxStageNum - 1;
	}
}

void ButiEngine::StageSelect::OnDecision()
{
	auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();
	std::string sceneName = "Stage" + std::to_string(stageNum);
	sceneManager->RemoveScene(sceneName);
	sceneManager->LoadScene(sceneName);
	sceneManager->ChangeScene(sceneName);
}

void ButiEngine::StageSelect::DecisionAnimation()
{
	if (InputManager::OnTriggerDecisionKey() && !isAnimation)
	{
		isAnimation = true;
	}

	if (!isAnimation) return;

	if (animationFrame == screenRotateFrame)
	{
		GetManager().lock()->GetGameObject("SelectScreen").lock()->GetGameComponent<SelectScreen>()->StartAnimation();
	}

	if (animationFrame <= 0)
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
		anim->GetTargetTransform()->SetLocalRotation(preParentRotation);
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
	auto angle = 360.0f / (float)(maxStageNum + 1) * 2.0f;

	for (int i = 0; i < stageNum; i++)
	{
		preParentRotation.y += angle;
	}
	parentSelectPanel->ChildRotation(-rotate);
}
