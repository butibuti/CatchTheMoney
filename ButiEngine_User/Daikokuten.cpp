#include "stdafx_u.h"
#include "Daikokuten.h"
#include "StageManager.h"
#include "PauseManager.h"
#include "Header/GameObjects/DefaultGameComponent/SimpleBoneAnimator.Component.h"

void ButiEngine::Daikokuten::OnUpdate()
{
	//�|�[�Y���͎�̓������~�߂�
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
	
	//��̓������~�܂��Ă����瓮�����ĊJ����
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
	//��b���̓X�e�[�W���O�ʂɗ���̂ŏ����߂ɂ���
	const float TALK_SCALE = 100;
	gameObject.lock()->transform->SetLocalScale(Vector3(TALK_SCALE, TALK_SCALE, TALK_SCALE));
}

void ButiEngine::Daikokuten::NormalScale()
{
	//�ʏ펞�̓X�e�[�W������ɂ���̂ő傫�߂ɂ���
	const float NORMAL_SCALE = 350;
	gameObject.lock()->transform->SetLocalScale(Vector3(NORMAL_SCALE, NORMAL_SCALE, NORMAL_SCALE));
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::Daikokuten::Clone()
{
	return ObjectFactory::Create<Daikokuten>();
}
