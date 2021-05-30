#include "stdafx_u.h"
#include "SitaSentan.h"
#include"Frog.h"
#include"GameSettings.h"
#include"Player.h"
#include"StageManager.h"
#include"SitaTyuukan.h"
#include "MobiusLoop.h"
#include "PauseManager.h"
#include "Header/GameObjects/DefaultGameComponent/SpliteAnimationComponent.h"

void ButiEngine::SitaSentan::OnUpdate()
{
	if (PauseManager::IsPause()) return;
	SetScale();
	SetX();
	gameObject.lock()->transform->SetWorldPostionY(wkp_frog.lock()->transform->GetWorldPosition().y);
	SetZ();
	Animation();
}

void ButiEngine::SitaSentan::OnSet()
{
}

void ButiEngine::SitaSentan::Start()
{
	animationFrame = 0;
	initScale = gameObject.lock()->transform->GetWorldScale();
	shp_spriteAnimation = gameObject.lock()->GetGameComponent<SpliteAnimationComponent>();
	shp_mobiusLoop = gameObject.lock()->GetGameComponent<MobiusLoop>();
}

void ButiEngine::SitaSentan::OnShowUI()
{
	GUI::Text("%f", gameObject.lock()->transform->GetWorldPosition().x);
	GUI::Text("%f", gameObject.lock()->transform->GetWorldPosition().y);
	GUI::Text("%f", gameObject.lock()->transform->GetWorldPosition().z);
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::SitaSentan::Clone()
{
	return ObjectFactory::Create<SitaSentan>();
}

void ButiEngine::SitaSentan::SetZ()
{
	gameObject.lock()->transform->SetWorldPostionZ(wkp_frog.lock()->transform->GetWorldPosition().z + 0.1f);
}

void ButiEngine::SitaSentan::SetAnim(int arg_anim)
{
	shp_spriteAnimation->SetVarticalAnim(arg_anim);
	shp_mobiusLoop->GetRight().lock()->GetGameComponent<SpliteAnimationComponent>()->SetVarticalAnim(arg_anim);
	shp_mobiusLoop->GetLeft().lock()->GetGameComponent<SpliteAnimationComponent>()->SetVarticalAnim(arg_anim);
}

void ButiEngine::SitaSentan::SetScale()
{
	Vector3 scale = gameObject.lock()->transform->GetLocalScale();
	Vector3 frogScale = wkp_frog.lock()->transform->GetWorldScale();
	if (scale.x < 0 != frogScale.x < 0)
	{
		scale.x *= -1;
	}
	if (scale.y < 0 == frogScale.y < 0)
	{
		scale.y *= -1;
	}
	gameObject.lock()->transform->SetLocalScale(scale);
}

void ButiEngine::SitaSentan::SetX()
{
	if (StageManager::GetMode() == GameMode::Edit)
	{
		float frogScaleX = wkp_frog.lock()->transform->GetWorldScale().x;
		float x = wkp_frog.lock()->transform->GetWorldPosition().x + GameSettings::blockSize * 50;
		if (frogScaleX < 0)
		{
			x = wkp_frog.lock()->transform->GetWorldPosition().x - GameSettings::blockSize * 50;
		}

		if (wkp_frog.lock()->GetGameComponent<Frog>()->IsGrabbed())
		{
			x = wkp_frog.lock()->transform->GetWorldPosition().x;
		}
		wkp_frog.lock()->GetGameComponent<Frog>()->GetSitatyuukan().lock()->GetGameComponent<SitaTyuukan>()->Move();
		gameObject.lock()->transform->SetWorldPostionX(x);
		wkp_frog.lock()->GetGameComponent<Frog>()->GetSitatyuukan().lock()->GetGameComponent<SitaTyuukan>()->Move();
	}
}

void ButiEngine::SitaSentan::Animation()
{
	animationFrame++;
	const int ANIMATION_RATE = 10;
	if (animationFrame < ANIMATION_RATE) return;

	animationFrame = 0;
	shp_spriteAnimation->UpdateHorizontalAnim(1);
	shp_mobiusLoop->GetRight().lock()->GetGameComponent<SpliteAnimationComponent>()->UpdateHorizontalAnim(1);
	shp_mobiusLoop->GetLeft().lock()->GetGameComponent<SpliteAnimationComponent>()->UpdateHorizontalAnim(1);
}
