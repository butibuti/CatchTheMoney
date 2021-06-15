#include "stdafx_u.h"
#include "GrabTextWindow.h"
#include"Player.h"
#include"GameSettings.h"
#include"StageManager.h"

void ButiEngine::GrabTextWindow::OnUpdate()
{
	const float MAX_SCALE = 150;
	const float MIN_SCALE = 0;
	StorePlayer();
	if (wkp_target.lock())
	{
		auto core = shp_player->GetHoldCore().lock();
		auto goal = shp_player->GetHoldGoal().lock();
		bool noGrab = (!core && !goal);
		if (StageManager::GetMode()==GameMode::Chara&&(
			noGrab && shp_player->IsHitCore() ||
			noGrab && shp_player->IsHitGoal() ||
			noGrab && shp_player->IsHitSita()))
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

	auto yRatio= (wkp_target.lock()->transform->GetWorldPosition().y+ GameSettings::windowHeight*0.5f)/(float)GameSettings::windowHeight ;

	yRatio -= 1.0f;

	auto position = MathHelper::GetMobiusPoint(MathHelper::ToRadian(0), yRatio) * 125.0f;

	position *= wkp_screen.lock()->transform->GetMatrix();

	vpMatrix = shp_camera->GetViewProjectionMatrix();

	position *= vpMatrix;

	position.z = 0.0f;

	position.x -= 100.0f;
	gameObject.lock()->transform->SetLocalPosition(position);

	const float LERP_SCALE = 0.3f;
	previousScale = previousScale * (1.0f - LERP_SCALE) + currentScale * LERP_SCALE;

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

	shp_camera = GetCamera("Camera");
	wkp_screen= GetManager().lock()->GetGameObject("Screen").lock();
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
		shp_player = wkp_target.lock()->GetGameComponent<Player>();
	}
}
