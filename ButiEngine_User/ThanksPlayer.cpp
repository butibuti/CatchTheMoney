#include "stdafx_u.h"
#include "ThanksPlayer.h"
#include "Header/GameObjects/DefaultGameComponent/SpliteAnimationComponent.h"
#include"GameSettings.h"
void ButiEngine::ThanksPlayer::OnUpdate()
{
	if (isAway)
	{
		gameObject.lock()->transform->SetLocalPosition(Vector3(0, -3000, 0));
		return;
	}
	if (isDecision)
	{
		DecisionAnimation();
	}
	else
	{
		if (animationFrame < 4)
		{
			animationFrame++;
		}
		else
		{
			animationFrame = 0;
			shp_spriteAnimation->UpdateHorizontalAnim(1);
		}
	}
	gameObject.lock()->transform->SetLocalPosition(position);
}

void ButiEngine::ThanksPlayer::OnSet()
{
}

void ButiEngine::ThanksPlayer::Start()
{
	isDecision = false;
	animationFrame = 0;
	animationTime = 0;
	velocityX = -1.0f;
	position = Vector3(-750, 350, 0);
	gameObject.lock()->transform->SetLocalPosition(position);
	gameObject.lock()->transform->SetLocalScale(Vector3(320, 320, 0));
	gameObject.lock()->transform->SetLocalRotation(Vector3(0, 0, 0));
	shp_spriteAnimation = gameObject.lock()->GetGameComponent<SpliteAnimationComponent>();
	shp_spriteAnimation->SetHorizontalAnim(0);
	se_dash = SoundTag("Sound/Rat_Dash.wav");
}

void ButiEngine::ThanksPlayer::Decision()
{
	isDecision = true;
	shp_spriteAnimation->SetHorizontalAnim(1);
}

void ButiEngine::ThanksPlayer::Away()
{
}

std::shared_ptr < ButiEngine:: GameComponent > ButiEngine::ThanksPlayer::Clone()
{
    return ObjectFactory::Create<ThanksPlayer>();
}

void ButiEngine::ThanksPlayer::DecisionAnimation()
{
	animationTime++;

	if (animationTime < 30)
	{
		velocityX += 0.05f;
		position.x += velocityX;
	}
	else if (animationTime == 30)
	{	GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_dash, GameSettings::masterVolume);
		GetManager().lock()->AddObjectFromCereal("ClearZanzo");
		position.y = -4000;
	}
	else if (animationTime == 50)
	{
		position = Vector3(100, 400, 0);
		gameObject.lock()->transform->SetLocalScale(Vector3(80, 80, 1));
		gameObject.lock()->transform->SetLocalRotation(Vector3(-180, 0, -180));
	}

}
