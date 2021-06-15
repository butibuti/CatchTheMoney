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
#include"Frog.h"

bool ButiEngine::TalkText::isDelete = false;
bool ButiEngine::TalkText::isNotMove = false;

void ButiEngine::TalkText::OnUpdate()
{
	//�|�[�Y���͏�����ʂ��Ȃ�
	if (shp_pauseManager->IsPause()) { return; }

	//�J�G���̉�b��Ɉ�莞�Ԍo�߂ł�����x�e�L�X�g���Ăі߂�
	FrogInterval();

	//��ʊO�ɔ�΂���
	if (isDelete)
	{
		if (StageSelect::GetStageNum()==0&& !isGeneratedControl) {
			GetManager().lock()->AddObjectFromCereal("Control");
			isGeneratedControl = true;
		}

		const float AWAY_POS = -3000;
		gameObject.lock()->transform->SetLocalPosition(Vector3(0, AWAY_POS, 0));
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
	TextProgress();

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
	isNotMove = false;
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
	auto stageNum = StageSelect::GetStageNum();

	//�X�e�[�W�ɉ����ĉ�b�ɕt���鉉�o��ς���
	if (stageNum == TalkStageNum::FIRST_TALK ||
		stageNum == TalkStageNum::PANEL_TALK ||
		stageNum == TalkStageNum::REVERSE_RE_TALK)
	{
		wkp_cameraUI.lock()->GetGameComponent<ShakeComponent>()->ShakeStart(SHAKE_POWER);
		wkp_camera.lock()->GetGameComponent<ShakeComponent>()->ShakeStart(SHAKE_POWER);
		wkp_daikokutenReaction.lock()->GetGameComponent<ParentDaikokuten>()->Reaction(false);
		GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_bigText, GameSettings::masterVolume);
		GetManager().lock()->GetGameObject("TextWindow").lock()->GetGameComponent<ShakeComponent>()->ShakeStart(SHAKE_POWER);
	}
	else if (stageNum == TalkStageNum::REVERSE_TALK && !GameSettings::isTutorialInit)
	{
		if (textCount == 4 || textCount == 6)
		{
			GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_normalText, GameSettings::masterVolume);
			return;
		}
		wkp_daikokutenReaction.lock()->GetGameComponent<ParentDaikokuten>()->Reaction(false);
		GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_bigText, GameSettings::masterVolume);
		GetManager().lock()->GetGameObject("TextWindow").lock()->GetGameComponent<ShakeComponent>()->ShakeStart(SHAKE_POWER);
	}
	else if (stageNum == TalkStageNum::REVERSE_TALK && GameSettings::isTutorialInit)
	{
		//�`���[�g���A��������x
		wkp_cameraUI.lock()->GetGameComponent<ShakeComponent>()->ShakeStart(SHAKE_POWER);
		wkp_camera.lock()->GetGameComponent<ShakeComponent>()->ShakeStart(SHAKE_POWER);
		wkp_daikokutenReaction.lock()->GetGameComponent<ParentDaikokuten>()->Reaction(false);
		GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_bigText, GameSettings::masterVolume);
		GetManager().lock()->GetGameObject("TextWindow").lock()->GetGameComponent<ShakeComponent>()->ShakeStart(SHAKE_POWER);
	}
	else if (stageNum == TalkStageNum::GRAVITY_TALK ||
		     stageNum == TalkStageNum::FROG_TALK)
	{
		GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_normalText, GameSettings::masterVolume);
	}
	else if (stageNum == TalkStageNum::LAST_TALK)
	{
		wkp_camera.lock()->GetGameComponent<ShakeComponent>()->ShakeStart(SHAKE_POWER);
		wkp_cameraUI.lock()->GetGameComponent<ShakeComponent>()->ShakeStart(SHAKE_POWER);
		wkp_daikokutenReaction.lock()->GetGameComponent<ParentDaikokuten>()->Reaction(true);
		GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_bigText, GameSettings::masterVolume);
		GetManager().lock()->GetGameObject("TextWindow").lock()->GetGameComponent<ShakeComponent>()->ShakeStart(SHAKE_POWER);
	}
}

//�ʏ펞�̉�b
void ButiEngine::TalkText::NormalTalk()
{
	if (textCount < shp_spriteAnimation->GetVarticalSplitScale())
	{
		//�e�L�X�g��i�߂�
		NextTalk();
	}
	else
	{
		isDelete = true;
		DaikokutenDisappear();
	}
}

//�J�G���������̗�O����
void ButiEngine::TalkText::FrogTalk()
{
	if (!isInterval)
	{
		if (textCount < shp_spriteAnimation->GetVarticalSplitScale() - 1)
		{
			//�e�L�X�g��i�߂�
			NextTalk();
		}
		else
		{
			isInterval = true;
			isDelete = true;
			isNotMove = true;
			TextEffect();
			DaikokutenDisappear();
		}
	}
	else
	{
		isDelete = true;
		isNotMove = false;
		TextEffect();
		DaikokutenDisappear();

		auto frog = GetManager().lock()->GetGameObject("Frog").lock()->GetGameComponent<Frog>();
		frog->PlayAnimation();
		frog->GetBackFrog().lock()->GetGameComponent<Frog>()->PlayAnimation();
	}

}

void ButiEngine::TalkText::NextTalk()
{
	TextEffect();
	shp_spriteAnimation->UpdateVarticalAnim(1);
}

//�I���u�������鏈��
void ButiEngine::TalkText::DaikokutenDisappear()
{
	wkp_daikokutenAppear.lock()->GetGameComponent<ParentDaikokuten>()->Disappear();
	wkp_daikokutenRHand.lock()->GetGameComponent<ParentDaikokuten>()->Disappear();
	wkp_daikokutenLHand.lock()->GetGameComponent<ParentDaikokuten>()->Disappear();
}

void ButiEngine::TalkText::FrogInterval()
{
	//�Ăь����܂ł̎���
	const int MAX_INTERVAL_TIME = 60;

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
	if (GameSettings::isTutorialInit)
	{
		return false;
	}
	const int SHAKE_FRAME = 40;
	const int ONCE_FRAME = 44;	//�����x�点��
	if (onceFrame == ONCE_FRAME)
	{
		wkp_camera.lock()->GetGameComponent<ShakeComponent>()->ShakeStart(SHAKE_POWER, SHAKE_FRAME);
		wkp_cameraUI.lock()->GetGameComponent<ShakeComponent>()->ShakeStart(SHAKE_POWER, SHAKE_FRAME);
		wkp_daikokutenReaction.lock()->GetGameComponent<ParentDaikokuten>()->Reaction(true);
		GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_bigText, GameSettings::masterVolume);
		GetManager().lock()->GetGameObject("TextWindow").lock()->GetGameComponent<ShakeComponent>()->ShakeStart(SHAKE_POWER, SHAKE_FRAME);
	}
	if (onceFrame <= ONCE_FRAME)
	{
		onceFrame++;
		return true;
	}
	return false;
}

void ButiEngine::TalkText::TextProgress()
{
	const int WAIT_FRAME = 12;
	if (InputManager::OnTriggerDecisionKey() && waitTime > WAIT_FRAME)
	{
		waitTime = 0;
		textCount++;
		if (StageSelect::GetStageNum() != TalkStageNum::FROG_TALK)
		{
			NormalTalk();
		}
		else
		{
			FrogTalk();
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
		isNotMove = false;
		isDelete = true;
		DaikokutenDisappear();

		if (StageSelect::GetStageNum() == TalkStageNum::FROG_TALK)
		{
			auto frog = GetManager().lock()->GetGameObject("Frog").lock()->GetGameComponent<Frog>();
			frog->PlayAnimation();
			frog->GetBackFrog().lock()->GetGameComponent<Frog>()->PlayAnimation();
		}
	}
}
