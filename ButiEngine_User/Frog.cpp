#include "stdafx_u.h"
#include "Frog.h"
#include"SitaSentan.h"
#include"SitaTyuukan.h"
#include"GameSettings.h"

void ButiEngine::Frog::OnUpdate()
{
}

void ButiEngine::Frog::OnSet()
{
}

void ButiEngine::Frog::Start()
{
	CreateSita();
}

void ButiEngine::Frog::OnShowUI()
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::Frog::Clone()
{
	return ObjectFactory::Create<Frog>();
}
void ButiEngine::Frog::CreateSita()
{
	wkp_sita_tyuukan = GetManager().lock()->AddObjectFromCereal("Sita_tyuukan");
	wkp_sita_sentan = GetManager().lock()->AddObjectFromCereal("Sita_sentan");

	auto tyuukan = wkp_sita_tyuukan.lock()->GetGameComponent<SitaTyuukan>();
	tyuukan->StoreFrog(gameObject);
	tyuukan->StoreSentan(wkp_sita_sentan);

	wkp_sita_sentan.lock()->GetGameComponent<SitaSentan>()->StoreFrog(gameObject);

	wkp_sita_sentan.lock()->transform->SetBaseTransform(gameObject.lock()->transform);
	wkp_sita_sentan.lock()->transform->SetLocalScale(Vector3(1, 1, 1));
	float sitaLength = GameSettings::blockSize * 50;
	Vector3 sentanPosition = Vector3::Zero;
	sentanPosition.x = sitaLength / gameObject.lock()->transform->GetWorldScale().x;
	sentanPosition.z = 0.01f;
	wkp_sita_sentan.lock()->transform->SetLocalPosition(sentanPosition);
}
