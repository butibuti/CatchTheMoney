#include "stdafx_u.h"
#include "Panel.h"
#include"GameSettings.h"

void ButiEngine::Panel::OnUpdate()
{
}

void ButiEngine::Panel::OnSet()
{
}

void ButiEngine::Panel::Start()
{
	Vector3 position;
	position.z = -0.1f;
	Vector3 scale;
	scale.x = GameSettings::panelWidth;
	scale.y = GameSettings::panelHeight;
	wkp_drawObject = GetManager().lock()->AddObjectFromCereal("PanelForDraw", ObjectFactory::Create<Transform>(position, Vector3::Zero, scale));
	wkp_drawObject.lock()->transform->SetBaseTransform(gameObject.lock()->transform, true);
}

void ButiEngine::Panel::OnShowUI()
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::Panel::Clone()
{
	return ObjectFactory::Create<Panel>();
}
