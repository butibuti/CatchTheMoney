#include "stdafx_u.h"
#include "StageSelect.h"

int ButiEngine::StageSelect::stageNum = 0;
int ButiEngine::StageSelect::maxStageNum = 10;

void ButiEngine::StageSelect::OnUpdate()
{
	GUI::Begin("StageNum");
	GUI::Text(stageNum);
	GUI::End();
	if (GameDevice::GetInput()->TriggerKey(Keys::D))
	{
		OnPushRight();
	}
	else if (GameDevice::GetInput()->TriggerKey(Keys::A))
	{
		OnPushLeft();
	}
	if (GameDevice::GetInput()->TriggerKey(Keys::Enter))
	{
		OnDecision();
	}
}

void ButiEngine::StageSelect::OnSet()
{
}

void ButiEngine::StageSelect::Start()
{
}

void ButiEngine::StageSelect::OnShowUI()
{
}

void ButiEngine::StageSelect::OnCollision(std::weak_ptr<GameObject> arg_other)
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::StageSelect::Clone()
{
	return ObjectFactory::Create<StageSelect>();
}

void ButiEngine::StageSelect::OnPushRight()
{
	stageNum++;
	if (stageNum > maxStageNum)
	{
		stageNum = 0;
	}
}

void ButiEngine::StageSelect::OnPushLeft()
{
	stageNum--;
	if (stageNum < 0)
	{
		stageNum = maxStageNum;
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
