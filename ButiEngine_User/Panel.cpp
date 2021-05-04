#include "stdafx_u.h"
#include "Panel.h"
#include"GameSettings.h"
#include"ShakeComponent.h"

void ButiEngine::Panel::OnUpdate()
{
	if (gameObject.lock()->GetGameComponent<TransformAnimation>())
	{
		animation = true;
	}
	else if(animation)
	{
		//GetManager().lock()->GetGameObject("Screen").lock()->GetGameComponent<ShakeComponent>()->ShakeStart(20.0f);
		animation = false;
	}
	SetDrawObject();
}

void ButiEngine::Panel::OnSet()
{
}

void ButiEngine::Panel::Start()
{
	Vector3 scale;
	scale.x = GameSettings::panelWidth;
	scale.y = GameSettings::panelHeight;
	wkp_drawObject = GetManager().lock()->AddObjectFromCereal("PanelForDraw", ObjectFactory::Create<Transform>(Vector3(0, 0, 0.3f), Vector3::Zero, scale));
	wkp_drawObject.lock()->transform->SetBaseTransform(gameObject.lock()->transform, true);
	gravity = 0.0f;
}

void ButiEngine::Panel::OnShowUI()
{
	GUI::SliderFloat("gravity", &gravity, -0.5f, 0.5f);
	GUI::Text("x:%f", gameObject.lock()->transform->GetWorldPosition().x);
	GUI::Text("y:%f", gameObject.lock()->transform->GetWorldPosition().y);
	GUI::Text("panelNum:%d", panelNum);
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::Panel::Clone()
{
	return ObjectFactory::Create<Panel>();
}

void ButiEngine::Panel::AddGravityCore(int arg_num, float arg_gravity)
{
	if (ContainsGravityCore(arg_num))
	{
		return;
	}
	vec_gravityCoreNums.push_back(arg_num);
	AddGravity(arg_gravity);
}

void ButiEngine::Panel::RemoveGravityCore(int arg_num, float arg_gravity)
{
	auto find = std::find(vec_gravityCoreNums.begin(), vec_gravityCoreNums.end(), arg_num);
	if (find == vec_gravityCoreNums.end())
	{
		return;
	}
	vec_gravityCoreNums.erase(find);
	AddGravity(-arg_gravity);
}

bool ButiEngine::Panel::ContainsGravityCore(int arg_num)
{
	auto find = std::find(vec_gravityCoreNums.begin(), vec_gravityCoreNums.end(), arg_num);
	return find != vec_gravityCoreNums.end();
}

void ButiEngine::Panel::ResetGravityCores()
{
	vec_gravityCoreNums.clear();
}

void ButiEngine::Panel::SetDrawObject()
{
	if (gravity != 0)
	{
		wkp_drawObject.lock()->transform->SetLocalPosition(Vector3(0, 0, 0.5f));
		Vector3 scale = wkp_drawObject.lock()->transform->GetLocalScale();
		if (scale.y > 0 == gravity > 0)
		{
			scale.y *= -1;
			wkp_drawObject.lock()->transform->SetLocalScale(scale);
		}
	}
	else
	{
		wkp_drawObject.lock()->transform->SetLocalPosition(Vector3(0, 0, 1000));
	}
}

void ButiEngine::Panel::AddTransformAnimation()
{
	Vector3 targetPos;
	targetPos.x = (panelNum - GameSettings::panelCount / 2) * GameSettings::panelWidth + GameSettings::panelWidth * 0.5f;

	auto anim = gameObject.lock()->AddGameComponent<TransformAnimation>();
	anim->SetTargetTransform(gameObject.lock()->transform->Clone());
	anim->GetTargetTransform()->SetWorldPosition(targetPos);
	anim->SetSpeed(1.0f / 10.0f);
	anim->SetEaseType(Easing::EasingType::Liner);

	if (panelNum < 0)
	{
		panelNum = GameSettings::panelCount + panelNum;
	}
	else if (panelNum >= GameSettings::panelCount)
	{
		panelNum -= GameSettings::panelCount;
	}
}
