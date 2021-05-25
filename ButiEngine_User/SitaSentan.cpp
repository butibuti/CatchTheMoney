#include "stdafx_u.h"
#include "SitaSentan.h"
#include"Frog.h"
#include"GameSettings.h"
#include"Player.h"

void ButiEngine::SitaSentan::OnUpdate()
{
	Vector3 scale = gameObject.lock()->transform->GetLocalScale();
	scale.y = Frog::GetSitaLength() / (GameSettings::blockSize * 50);
	gameObject.lock()->transform->SetLocalScale(scale);
}

void ButiEngine::SitaSentan::OnSet()
{
}

void ButiEngine::SitaSentan::Start()
{
}

void ButiEngine::SitaSentan::OnShowUI()
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::SitaSentan::Clone()
{
	return ObjectFactory::Create<SitaSentan>();
}

void ButiEngine::SitaSentan::Move()
{
	Vector3 sentanPosition = Vector3::Zero;
	sentanPosition.x = Frog::GetSitaLength() / abs(wkp_frog.lock()->transform->GetWorldScale().x);
	sentanPosition.z = 0.01f;
	gameObject.lock()->transform->SetLocalPosition(sentanPosition);
}
