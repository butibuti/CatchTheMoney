#include "stdafx_u.h"
#include "GrabTextWindow.h"
#include"Player.h"

void ButiEngine::GrabTextWindow::OnUpdate()
{
	StorePlayer();
	if (wkp_player.lock())
	{
		if (wkp_player.lock()->GetGameComponent<Player>()->IsHitCore())
		{
			gameObject.lock()->transform->SetWorldPosition(initPos);
		}
		else
		{
			gameObject.lock()->transform->SetWorldPostionZ(1000.0f);
		}
	}
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
	if (!wkp_player.lock())
	{
		wkp_player = GetManager().lock()->GetGameObject("Player");
	}
}
