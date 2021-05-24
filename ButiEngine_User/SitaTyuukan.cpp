#include "stdafx_u.h"
#include "SitaTyuukan.h"
#include "GameSettings.h"

void ButiEngine::SitaTyuukan::OnUpdate()
{
	Vector3 frogPos = wkp_frog.lock()->transform->GetWorldPosition();
	Vector3 sentanPos = wkp_sentan.lock()->transform->GetWorldPosition();
	Vector3 frogScale = wkp_frog.lock()->transform->GetWorldScale();

	//sita no sentan ga kaeru no mae ni aru ka
	float difference = sentanPos.x - frogPos.x;
	if (frogScale.x < 0 != difference < 0)
	{
		if (difference < 0)
		{
			sentanPos.x += GameSettings::windowWidth;
		}
		else if (difference > 0)
		{
			sentanPos.x -= GameSettings::windowWidth;
		}
	}

	Vector3 pos = Vector3::Zero;
	pos.x = (frogPos.x + sentanPos.x) * 0.5f;
	pos.y = frogPos.y;
	pos.z = sentanPos.z + 0.01f;
	gameObject.lock()->transform->SetWorldPosition(pos);

	Vector3 scale = Vector3::Zero;
	scale.x = abs(frogPos.x - sentanPos.x);
	scale.y = 16.0f;
	scale.z = 1.0f;
	gameObject.lock()->transform->SetLocalScale(scale);
}

void ButiEngine::SitaTyuukan::OnSet()
{
}

void ButiEngine::SitaTyuukan::Start()
{
}

void ButiEngine::SitaTyuukan::OnShowUI()
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::SitaTyuukan::Clone()
{
	return ObjectFactory::Create<SitaTyuukan>();
}
