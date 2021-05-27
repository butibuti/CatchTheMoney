#include "stdafx_u.h"
#include "AnimationPlayer.h"
#include"Header//GameObjects/DefaultGameComponent/SpliteAnimationComponent.h"


void ButiEngine::AnimationPlayer::OnUpdate()
{
    if (shp_timer->Update()) {
        animNum = (animNum + 1) % animMax;
        shp_anim->SetHorizontalAnim(animNum);
    }
}

void ButiEngine::AnimationPlayer::OnSet()
{
    if (!shp_anim) {
        shp_anim = gameObject.lock()->GetGameComponent<SpliteAnimationComponent>();
    }
    shp_timer = ObjectFactory::Create<RelativeTimer>(pase);
}

void ButiEngine::AnimationPlayer::Start()
{
    if (!shp_anim) {
        shp_anim = gameObject.lock()->GetGameComponent<SpliteAnimationComponent>();
    }
    shp_timer->Start();
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::AnimationPlayer::Clone()
{
    auto output = ObjectFactory::Create<AnimationPlayer>();
    output->animMax = animMax;
    output->pase = pase;
    return output;
}

void ButiEngine::AnimationPlayer::OnShowUI()
{
    GUI::DragInt("animMaxFrame", animMax, 1, 0, 100);
    GUI::DragInt("animationPase", pase, 1, 0, 100);
}
