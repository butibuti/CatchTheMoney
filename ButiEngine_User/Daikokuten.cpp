#include "stdafx_u.h"
#include "Daikokuten.h"
#include "StageManager.h"
#include "PauseManager.h"
#include "Header/GameObjects/DefaultGameComponent/SimpleBoneAnimator.Component.h"

void ButiEngine::Daikokuten::OnUpdate()
{
	//ポーズ中は手の動きを止める
	if (PauseManager::IsPause())
	{
		if (gameObject.lock()->GetGameObjectName() == "DaikokutenHand_L" ||
			gameObject.lock()->GetGameObjectName() == "DaikokutenHand_R")
		{
			gameObject.lock()->GetGameComponent<SimpleBoneAnimatorComponent>()->SetIsActive(false);
		}
		isActiveChange = true;
		return;
	}
	
	//手の動きが止まっていたら動きを再開する
	if(isActiveChange)
	{
		isActiveChange = false;

		if (gameObject.lock()->GetGameObjectName() == "DaikokutenHand_L" ||
			gameObject.lock()->GetGameObjectName() == "DaikokutenHand_R")
		{
			gameObject.lock()->GetGameComponent<SimpleBoneAnimatorComponent>()->SetIsActive(true);
		}
	}

}

void ButiEngine::Daikokuten::OnSet()
{
}

void ButiEngine::Daikokuten::Start()
{
	isActiveChange = false;
}

void ButiEngine::Daikokuten::TalkScale()
{
	//会話時はステージより前面に来るので小さめにする
	const float TALK_SCALE = 100;
	gameObject.lock()->transform->SetLocalScale(Vector3(TALK_SCALE, TALK_SCALE, TALK_SCALE));
}

void ButiEngine::Daikokuten::NormalScale()
{
	//通常時はステージより後方にいるので大きめにする
	const float NORMAL_SCALE = 350;
	gameObject.lock()->transform->SetLocalScale(Vector3(NORMAL_SCALE, NORMAL_SCALE, NORMAL_SCALE));
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::Daikokuten::Clone()
{
	return ObjectFactory::Create<Daikokuten>();
}
