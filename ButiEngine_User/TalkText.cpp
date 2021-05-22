#include "stdafx_u.h"
#include "TalkText.h"
#include "StageSelect.h"
#include "InputManager.h"
#include "ShakeComponent.h"
#include "Header/GameObjects/DefaultGameComponent/SpliteAnimationComponent.h"
#include"PauseManager.h"

bool ButiEngine::TalkText::isDelete = false;

void ButiEngine::TalkText::OnUpdate()
{
	if (isDelete)
	{
		gameObject.lock()->transform->SetLocalPosition(Vector3(0, -3000, 0));
		return;
	}

	if (!isOnce)
	{
		isOnce = true;
		GetManager().lock()->GetGameObject("TextWindow").lock()->GetGameComponent<ShakeComponent>()->ShakeStart(8, 30);
	}

	if (shp_pauseManager->IsPause()) { return; }

	const int WAIT_FRAME = 12;
	if (InputManager::OnTriggerDecisionKey() && waitTime > WAIT_FRAME)
	{
		waitTime = 0;
		textCount++;
		ShakeAimation();
		if (textCount < shp_spriteAnimation->GetVarticalSplitScale())
		{
			shp_spriteAnimation->UpdateVarticalAnim(1);
		}
		else
		{
			isDelete = true;
		}
	}
	if (InputManager::OnTextSkipKey())
	{
		isDelete = true;
	}
	if(waitTime <= WAIT_FRAME)
	{
		waitTime++;
	}
}

void ButiEngine::TalkText::OnSet()
{
}

void ButiEngine::TalkText::Start()
{
	shp_spriteAnimation = gameObject.lock()->GetGameComponent<SpliteAnimationComponent>();
	shp_pauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManager>();

	textCount = 0;
	waitTime = 0;
	isDelete = false;
	isOnce = false;
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::TalkText::Clone()
{
	return ObjectFactory::Create<TalkText>();
}

void ButiEngine::TalkText::Delete()
{
	isDelete = true;
}

void ButiEngine::TalkText::ShakeAimation()
{
	if (StageSelect::GetStageNum() == 0)
	{
		GetManager().lock()->GetGameObject("TextWindow").lock()->GetGameComponent<ShakeComponent>()->ShakeStart(8);
	}
	else if (StageSelect::GetStageNum() == 3)
	{
		if (textCount == 4 || textCount == 6)
		{
			return;
		}
		GetManager().lock()->GetGameObject("TextWindow").lock()->GetGameComponent<ShakeComponent>()->ShakeStart(8);
	}
	else if (StageSelect::GetStageNum() == 6)
	{
		if (textCount == 0)
		{
			GetManager().lock()->GetGameObject("TextWindow").lock()->GetGameComponent<ShakeComponent>()->ShakeStart(8);
		}
	}
}
