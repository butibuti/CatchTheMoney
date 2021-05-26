#include "stdafx_u.h"
#include "SitaSentan.h"
#include"Frog.h"
#include"GameSettings.h"
#include"Player.h"
#include"StageManager.h"

void ButiEngine::SitaSentan::OnUpdate()
{
	SetX();
	gameObject.lock()->transform->SetWorldPostionY(wkp_frog.lock()->transform->GetWorldPosition().y);
	SetZ();
}

void ButiEngine::SitaSentan::OnSet()
{
}

void ButiEngine::SitaSentan::Start()
{
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
	gameObject.lock()->transform->SetWorldPostionZ(wkp_frog.lock()->transform->GetWorldPosition().z + 0.01f);
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
		gameObject.lock()->transform->SetWorldPostionX(x);
	}
}
