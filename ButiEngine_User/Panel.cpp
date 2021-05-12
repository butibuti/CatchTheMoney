#include "stdafx_u.h"
#include "Panel.h"
#include"GameSettings.h"
#include"ShakeComponent.h"
#include"Player.h"

void ButiEngine::Panel::OnUpdate()
{
	StorePlayer();
	if (gameObject.lock()->GetGameComponent<TransformAnimation>())
	{
		animation = true;
	}
	else if(animation)
	{
		//GetManager().lock()->GetGameObject("Screen").lock()->GetGameComponent<ShakeComponent>()->ShakeStart(20.0f);
		animation = false;
	}
	OnChangeGravity(false);
	scroll = false;
}

void ButiEngine::Panel::OnSet()
{
}

void ButiEngine::Panel::Start()
{
	Vector3 scale;
	scale.x = GameSettings::panelWidth;
	scale.y = GameSettings::panelHeight;
	scale.z = 1.0f;

	wkp_drawObjectGravity = GetManager().lock()->AddObjectFromCereal("Gravity", ObjectFactory::Create<Transform>(Vector3(0, 0, 0.3f), Vector3::Zero, scale));
	wkp_drawObjectGravity.lock()->transform->SetBaseTransform(gameObject.lock()->transform, true);
	wkp_drawObjectFrame = GetManager().lock()->AddObjectFromCereal("PanelFrame", ObjectFactory::Create<Transform>(Vector3(0, 0, -1.0f), Vector3::Zero, scale));
	wkp_drawObjectFrame.lock()->transform->SetBaseTransform(gameObject.lock()->transform, true);

	gravity = 0.0f;

	se_cancel = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/GravityCancel.wav");
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

void ButiEngine::Panel::OnChangeGravity(bool arg_scroll)
{
	if (arg_scroll)
	{
		scroll = true; 
	}
	
	if (scroll) { return; }
	prevGravity = currentGravity;
	currentGravity = gravity;
	if (gravity != 0)
	{
		wkp_drawObjectGravity.lock()->transform->SetLocalPosition(Vector3(0, 0, 0.5f));
		Vector3 scale = wkp_drawObjectGravity.lock()->transform->GetLocalScale();
		if (scale.y > 0 == gravity > 0)
		{
			scale.y *= -1;
			wkp_drawObjectGravity.lock()->transform->SetLocalScale(scale);
		}
	}
	else
	{
		wkp_drawObjectGravity.lock()->transform->SetLocalPosition(Vector3(0, 0, 1000));
	}

	bool playerGrab = false;
	if (wkp_player.lock() && wkp_player.lock()->GetGameComponent<Player>()->GetHoldCore().lock())
	{
		playerGrab = true;
	}
	if (prevGravity != currentGravity && playerGrab && !arg_scroll)
	{
		if (vec_gravityCoreNums.size() >= 2 && currentGravity == 0)
		{
			//GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_cancel, 0.1f);
		}
		prevGravity = currentGravity;
	}
}

void ButiEngine::Panel::SetDrawObjectSky(bool arg_back)
{
	Vector3 scale;
	scale.x = GameSettings::panelWidth;
	scale.y = GameSettings::panelHeight;
	scale.z = 1.0f;

	std::string name = "Sky_Front";
	if (arg_back)
	{
		name = "Sky_Back";
	}

	wkp_drawObjectSky = GetManager().lock()->AddObjectFromCereal(name, ObjectFactory::Create<Transform>(Vector3(0, 0, 1.0f), Vector3::Zero, scale));
	wkp_drawObjectSky.lock()->transform->SetBaseTransform(gameObject.lock()->transform, true);
}

void ButiEngine::Panel::AddTransformAnimation(int arg_frame)
{
	Vector3 targetPos;
	targetPos.x = (panelNum - GameSettings::panelCount / 2) * GameSettings::panelWidth + GameSettings::panelWidth * 0.5f;

	auto anim = gameObject.lock()->AddGameComponent<TransformAnimation>();
	anim->SetTargetTransform(gameObject.lock()->transform->Clone());
	anim->GetTargetTransform()->SetWorldPosition(targetPos);
	anim->SetSpeed(1.0f / arg_frame);
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

void ButiEngine::Panel::StorePlayer()
{
	if (!wkp_player.lock())
	{
		wkp_player = GetManager().lock()->GetGameObject("Player");
	}
}
