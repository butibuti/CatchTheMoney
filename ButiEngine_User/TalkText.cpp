#include "stdafx_u.h"
#include "TalkText.h"
#include "StageSelect.h"
#include "InputManager.h"
#include "ShakeComponent.h"
#include "Header/GameObjects/DefaultGameComponent/SpliteAnimationComponent.h"
#include"PauseManager.h"
#include "GameSettings.h"
#include "ParentDaikokuten.h"
#include "Daikokuten.h"

bool ButiEngine::TalkText::isDelete = false;

void ButiEngine::TalkText::OnUpdate()
{
	//�|�[�Y���͏�����ʂ��Ȃ�
	if (shp_pauseManager->IsPause()) { return; }

	//�J�G���̉�b��Ɉ�莞�Ԍo�߂ł�����x�e�L�X�g���Ăі߂�
	FrogInterval();

	//��ʊO�ɔ�΂���
	if (isDelete)
	{
		gameObject.lock()->transform->SetLocalPosition(Vector3(0, -3000, 0));
		return;
	}
	else
	{
		gameObject.lock()->transform->SetLocalPosition(initPos);
	}

	//�J�������Z�b�g
	if (SetCamera())return;

	//�e�L�X�g�������Ɉ�x�����s������
	if (Once()) return;

	//�e�L�X�g��i�߂�
	NextText();

	//�e�L�X�g���΂�
	Skip();
}

void ButiEngine::TalkText::OnSet()
{
}

void ButiEngine::TalkText::Start()
{
	shp_spriteAnimation = gameObject.lock()->GetGameComponent<SpliteAnimationComponent>();
	shp_pauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManager>();

	se_bigText = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/Daikokuten_Big.wav");
	se_normalText = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/Daikokuten_Small.wav");

	wkp_daikokuten = GetManager().lock()->GetGameObject("Daikokuten");
	wkp_daikokutenAppear = GetManager().lock()->GetGameObject("Daikokuten_Appear");
	wkp_daikokutenRHand = GetManager().lock()->GetGameObject("Daikokuten_Appear_R");
	wkp_daikokutenLHand = GetManager().lock()->GetGameObject("Daikokuten_Appear_L");
	wkp_daikokutenReaction = GetManager().lock()->GetGameObject("Daikokuten_Reaction");

	wkp_daikokuten.lock()->GetGameComponent<Daikokuten>()->TalkScale();
	wkp_daikokutenAppear.lock()->GetGameComponent<ParentDaikokuten>()->TalkAppear();
	wkp_daikokutenRHand.lock()->GetGameComponent<ParentDaikokuten>()->Appear();
	wkp_daikokutenLHand.lock()->GetGameComponent<ParentDaikokuten>()->Appear();

	initPos = gameObject.lock()->transform->GetLocalPosition();

	textCount = 0;
	waitTime = 0;
	onceFrame = 0;
	intervalTime = 0;
	isDelete = false;
	isInterval = false;
}

void ButiEngine::TalkText::ReTalk()
{
	isDelete = false;
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::TalkText::Clone()
{
	return ObjectFactory::Create<TalkText>();
}

void ButiEngine::TalkText::Delete()
{
	isDelete = true;
}

void ButiEngine::TalkText::Revive()
{
	isDelete = false;
}

void ButiEngine::TalkText::TextEffect()
{
	//�X�e�[�W�ɉ����ĉ�b�ɕt���鉉�o��ς���
	auto stageNum = StageSelect::GetStageNum();
	if (stageNum == TalkStageNum::FIRST_TALK ||
		stageNum == TalkStageNum::PANEL_TALK ||
		stageNum == TalkStageNum::REVERSE_RE_TALK)
	{
		wkp_cameraUI.lock()->GetGameComponent<ShakeComponent>()->ShakeStart(8);
		wkp_camera.lock()->GetGameComponent<ShakeComponent>()->ShakeStart(8);
		wkp_daikokutenReaction.lock()->GetGameComponent<ParentDaikokuten>()->Reaction(false);
		GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_bigText, GameSettings::masterVolume);
		GetManager().lock()->GetGameObject("TextWindow").lock()->GetGameComponent<ShakeComponent>()->ShakeStart(8);
	}
	else if (stageNum == TalkStageNum::REVERSE_TALK)
	{
		if (textCount == 4 || textCount == 6)
		{
			GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_normalText, GameSettings::masterVolume);
			return;
		}
		wkp_daikokutenReaction.lock()->GetGameComponent<ParentDaikokuten>()->Reaction(false);
		GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_bigText, GameSettings::masterVolume);
		GetManager().lock()->GetGameObject("TextWindow").lock()->GetGameComponent<ShakeComponent>()->ShakeStart(8);
	}
	else if (stageNum == TalkStageNum::GRAVITY_TALK ||
		     stageNum == TalkStageNum::FROG_TALK)
	{
		GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_normalText, GameSettings::masterVolume);
	}
	else if (stageNum == TalkStageNum::LAST_TALK)
	{
		wkp_camera.lock()->GetGameComponent<ShakeComponent>()->ShakeStart(8);
		wkp_cameraUI.lock()->GetGameComponent<ShakeComponent>()->ShakeStart(8);
		wkp_daikokutenReaction.lock()->GetGameComponent<ParentDaikokuten>()->Reaction(true);
		GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_bigText, GameSettings::masterVolume);
		GetManager().lock()->GetGameObject("TextWindow").lock()->GetGameComponent<ShakeComponent>()->ShakeStart(8);
	}
}

void ButiEngine::TalkText::NormalTalk()
{
	//�ʏ펞�̉�b
	if (textCount < shp_spriteAnimation->GetVarticalSplitScale())
	{
		TextEffect();
		shp_spriteAnimation->UpdateVarticalAnim(1);
	}
	else
	{
		isDelete = true;
		wkp_daikokutenAppear.lock()->GetGameComponent<ParentDaikokuten>()->Disappear();
		wkp_daikokutenRHand.lock()->GetGameComponent<ParentDaikokuten>()->Disappear();
		wkp_daikokutenLHand.lock()->GetGameComponent<ParentDaikokuten>()->Disappear();
	}
}

void ButiEngine::TalkText::AbnormalTalk()
{
	//�J�G���������̗�O����
	if (!isInterval)
	{
		if (textCount < shp_spriteAnimation->GetVarticalSplitScale() - 1)
		{
			TextEffect();
			shp_spriteAnimation->UpdateVarticalAnim(1);
		}
		else
		{
			isInterval = true;
			isDelete = true;
			TextEffect();
			wkp_daikokutenAppear.lock()->GetGameComponent<ParentDaikokuten>()->Disappear();
			wkp_daikokutenRHand.lock()->GetGameComponent<ParentDaikokuten>()->Disappear();
			wkp_daikokutenLHand.lock()->GetGameComponent<ParentDaikokuten>()->Disappear();
		}
	}
	else
	{
		isDelete = true;
		TextEffect();
		wkp_daikokutenAppear.lock()->GetGameComponent<ParentDaikokuten>()->Disappear();
		wkp_daikokutenRHand.lock()->GetGameComponent<ParentDaikokuten>()->Disappear();
		wkp_daikokutenLHand.lock()->GetGameComponent<ParentDaikokuten>()->Disappear();
	}

}

void ButiEngine::TalkText::FrogInterval()
{
	//�Ăь����܂ł̎���
	const int MAX_INTERVAL_TIME = 120;

	if (isInterval && intervalTime <= MAX_INTERVAL_TIME)
	{
		intervalTime++;
	}
	if (intervalTime == MAX_INTERVAL_TIME)
	{
		isDelete = false;
		shp_spriteAnimation->UpdateVarticalAnim(1);
		wkp_daikokuten.lock()->GetGameComponent<Daikokuten>()->TalkScale();
		wkp_daikokutenAppear.lock()->GetGameComponent<ParentDaikokuten>()->TalkAppear();
		wkp_daikokutenRHand.lock()->GetGameComponent<ParentDaikokuten>()->Appear();
		wkp_daikokutenLHand.lock()->GetGameComponent<ParentDaikokuten>()->Appear();
	}
}

bool ButiEngine::TalkText::SetCamera()
{
	if (!wkp_camera.lock())
	{
		wkp_camera = GetManager().lock()->GetGameObject("Camera");
		wkp_cameraUI = GetManager().lock()->GetGameObject("UICamera");
		return true;
	}
	return false;
}

bool ButiEngine::TalkText::Once()
{
	//�����x�点��
	const int ONCE_FRAME = 24;
	if (onceFrame == ONCE_FRAME)
	{
		wkp_camera.lock()->GetGameComponent<ShakeComponent>()->ShakeStart(8, 40);
		wkp_cameraUI.lock()->GetGameComponent<ShakeComponent>()->ShakeStart(8, 40);
		wkp_daikokutenReaction.lock()->GetGameComponent<ParentDaikokuten>()->Reaction(true);
		GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_bigText, GameSettings::masterVolume);
		GetManager().lock()->GetGameObject("TextWindow").lock()->GetGameComponent<ShakeComponent>()->ShakeStart(8, 40);
	}
	if (onceFrame <= ONCE_FRAME)
	{
		onceFrame++;
		return true;
	}
	return false;
}

void ButiEngine::TalkText::NextText()
{
	const int WAIT_FRAME = 12;
	if (InputManager::OnTriggerDecisionKey() && waitTime > WAIT_FRAME)
	{
		waitTime = 0;
		textCount++;
		if (StageSelect::GetStageNum() != 13)
		{
			NormalTalk();
		}
		else
		{
			AbnormalTalk();
		}

	}
	if (waitTime <= WAIT_FRAME)
	{
		waitTime++;
	}
}

void ButiEngine::TalkText::Skip()
{
	if (InputManager::OnTextSkipKey())
	{
		isDelete = true;
		wkp_daikokutenAppear.lock()->GetGameComponent<ParentDaikokuten>()->Disappear();
		wkp_daikokutenRHand.lock()->GetGameComponent<ParentDaikokuten>()->Disappear();
		wkp_daikokutenLHand.lock()->GetGameComponent<ParentDaikokuten>()->Disappear();
	}
}
