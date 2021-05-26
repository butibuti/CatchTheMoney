#include "stdafx_u.h"
#include "GrabTextWindow.h"
#include"Player.h"

void ButiEngine::GrabTextWindow::OnUpdate()
{
	const float MAX_SCALE = 240;
	const float MIN_SCALE = 0;
	StorePlayer();
	if (wkp_target.lock())
	{
		auto player = wkp_target.lock()->GetGameComponent<Player>();
		bool noGrab = (!player->GetHoldCore().lock() && !player->GetHoldFrog().lock());
		if (noGrab && player->IsHitCore() ||
			noGrab && player->IsHitFrog())
		{
			//gameObject.lock()->transform->SetWorldPosition(initPos);
			currentScale = MAX_SCALE;
		}
		else
		{
			//gameObject.lock()->transform->SetWorldPostionZ(1000.0f);
			currentScale = MIN_SCALE;
		}
	}

	previousScale = previousScale * (1.0f - 0.3f) + currentScale * 0.3f;

	gameObject.lock()->transform->SetLocalScale(Vector3(previousScale, previousScale, 1));
}

void ButiEngine::GrabTextWindow::OnSet()
{
}

void ButiEngine::GrabTextWindow::Start()
{
	initPos = gameObject.lock()->transform->GetWorldPosition();
	wkp_b = GetManager().lock()->AddObjectFromCereal("B");
	wkp_b.lock()->transform->SetBaseTransform(gameObject.lock()->transform);
	wkp_b.lock()->transform->SetLocalPosition(Vector3(0, 0, -0.1f));

	previousScale = 0;
	currentScale = 0;
}

void ButiEngine::GrabTextWindow::OnShowUI()
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::GrabTextWindow::Clone()
{
	return ObjectFactory::Create<GrabTextWindow>();
}

void ButiEngine::GrabTextWindow::StorePlayer()
{
	if (!wkp_target.lock())
	{
		wkp_target = GetManager().lock()->GetGameObject("Player");
	}
}
