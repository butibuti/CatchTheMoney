#include "stdafx_u.h"
#include "SquareParticleEmitter.h"

const float circleLength = 360.0f / 16.0f;


ButiEngine::Vector3 GetMobiusPoint(float arg_time,float arg_radius) {
    ButiEngine::Vector3 output;

    output.x = -(arg_radius * cos(arg_time)+2) * sin(2 * arg_time);
    output.y = (arg_radius * cos(arg_time)+2) * cos(2 * arg_time);
    output.z = arg_radius * sin(arg_time);

    return output;
}

void ButiEngine::SquareParticleEmitter::OnUpdate(){
    if (isEdit) {
        EditUpdate();
    }
    else {
        for (int i = 0; i < increase; i++)
        {
            auto currentScale = vec_frameParts[i].shp_object->transform->GetLocalScale().x;
            vec_frameParts[i].shp_object->transform->SetLocalScale(Vector3((currentScale-0) * 0.5f));
        }
    }
}

void ButiEngine::SquareParticleEmitter::OnSet()
{
}

void ButiEngine::SquareParticleEmitter::Roll(const float arg_degrees)
{
    rotation += arg_degrees;
    if (rotation < 0) {
        rotation += 360.0f;
    }
    else if (rotation > 360.0f) {
        rotation -= 360.0f;
    }

    leftUpEdge = GetMobiusPoint(MathHelper::ToRadian(rotation + -circleLength), -1) * radius;
    leftDownEdge = GetMobiusPoint(MathHelper::ToRadian(rotation + -circleLength + 180), -1) * radius;
    rightUpEdge = GetMobiusPoint(MathHelper::ToRadian(rotation + circleLength), -1) * radius;
    rightDownEdge = GetMobiusPoint(MathHelper::ToRadian(rotation + circleLength + 180), -1) * radius;
}

void ButiEngine::SquareParticleEmitter::SetRotation(const float arg_degrees)
{
    if (rotation == arg_degrees) {
        return;
    }
    rotation = arg_degrees;
    if (rotation < 0) {
        rotation += 360.0f;
    }
    else if (rotation > 360.0f) {
        rotation -= 360.0f;
    }

    leftUpEdge = GetMobiusPoint(MathHelper::ToRadian(rotation + -circleLength), -1) * radius;
    leftDownEdge = GetMobiusPoint(MathHelper::ToRadian(rotation + -circleLength + 180), -1) * radius;
    rightUpEdge = GetMobiusPoint(MathHelper::ToRadian(rotation + circleLength), -1) * radius;
    rightDownEdge = GetMobiusPoint(MathHelper::ToRadian(rotation + circleLength + 180), -1) * radius;
}

void ButiEngine::SquareParticleEmitter::Start()
{
    auto screen = GetManager().lock()->GetGameObject("Screen").lock();
    for (int i = 0; i < increase; i++) {
        auto framePartObj = GetManager().lock()->AddObjectFromCereal("FramePart").lock();
        framePartObj->transform->SetBaseTransform(screen->transform);
        FramePart framePart;
        framePart.mode = (MoveMode)(i/(increase/5)) ;
        framePart.time = (1.0f / (increase / 5)) * i-(int)framePart.mode;
        framePart.shp_object = framePartObj;
        auto t = (1.0f / (increase / 2)) * i - ((int)framePart.mode /2);
        framePart.size = sizeMin+(sizeMax-sizeMin)*t;
        vec_frameParts.push_back(framePart);
    }



}

void ButiEngine::SquareParticleEmitter::OnShowUI()
{
    GUI::ColorEdit4("particleColorBaseMin", colorMin);
    GUI::ColorEdit4("particleColorBaseMax", colorMax);
    GUI::ColorEdit4("particleColorPaseMin", colorPaseMin);
    GUI::ColorEdit4("particleColorPaseMax", colorPaseMax);
    GUI::DragFloat("speedMin", speedMin, 0.01f, 0.0f, 10.0f);
    GUI::DragFloat("speedMax", speedMax, 0.01f, 0.0f, 10.0f);
    GUI::DragFloat("lifeMin", lifeMin, 0.01f, 0.0f, 10.0f);
    GUI::DragFloat("lifeMax", lifeMax, 0.01f, 0.0f, 10.0f);
    GUI::DragFloat("sizeMin", sizeMin, 0.01f, 0.0f, 10.0f);
    GUI::DragFloat("sizeMax", sizeMax, 0.01f, 0.0f, 10.0f);
    GUI::DragFloat("accelMin", accelMin, 0.01f, 0.0f, 10.0f);
    GUI::DragFloat("accelMax", accelMax, 0.01f, 0.0f, 10.0f);
    GUI::DragFloat("thickNess", thickness, 0.01f, 0.0f, 10.0f);
    GUI::DragFloat("radius", radius, 0.01f, 0.0f, 100.0f);
    GUI::DragFloat("rotation", rotation, 0.01f, 0.0f, 100.0f);
    GUI::DragInt("increase", increase, 1, 0, 10);
}

void ButiEngine::SquareParticleEmitter::SetIsEdit(const bool arg_isEdit)
{
    isEdit = arg_isEdit;
}

void ButiEngine::SquareParticleEmitter::EditUpdate()
{
    Vector3 thicknessOffset = Vector3(0, 0, thickness);
    for (int i = 0; i < increase; i++)
    {
        auto currentScale = vec_frameParts[i].shp_object->transform->GetLocalScale().x;
        vec_frameParts[i].shp_object->transform->SetLocalScale(Vector3((vec_frameParts[i].size - currentScale) * 0.5f));
        vec_frameParts[i].shp_object->transform->SetWorldRotation(Matrix4x4());
        switch (vec_frameParts[i].mode)
        {
        case MoveMode::Move_TopLineFront:
        {
            vec_frameParts[i].time += speedMin;
            auto t = (rotation - circleLength) * (1 - (vec_frameParts[i].time)) + (rotation ) * (vec_frameParts[i].time);
            vec_frameParts[i].shp_object->transform->SetLocalPosition(GetMobiusPoint(MathHelper::ToRadian(t + 180), -1) * radius + thicknessOffset);

        }
        break;
        case MoveMode::Move_TopLineBack:
        {
            vec_frameParts[i].time += speedMin;
            auto t = (rotation ) * (1 - (vec_frameParts[i].time)) + (rotation + circleLength) * (vec_frameParts[i].time);
            vec_frameParts[i].shp_object->transform->SetLocalPosition(GetMobiusPoint(MathHelper::ToRadian(t + 180), -1) * radius + thicknessOffset);

        }
        break;
        case MoveMode::Move_LeftLine:
        {
            vec_frameParts[i].time += speedMin;
            auto t = (vec_frameParts[i].time);
            vec_frameParts[i].shp_object->transform->SetLocalPosition(rightDownEdge + (rightUpEdge - rightDownEdge) * t + thicknessOffset);

        }
        break;
        case MoveMode::Move_BottomLine:
        {

            vec_frameParts[i].time += speedMin;
            auto time = vec_frameParts[i].time;
            auto t = (rotation + circleLength) * (1 - time) + (rotation - circleLength) * (time);
            vec_frameParts[i].shp_object->transform->SetLocalPosition(GetMobiusPoint(MathHelper::ToRadian(t), -1) * radius + thicknessOffset);

        }
        break;
        case MoveMode::Move_RightLine:
        {
            vec_frameParts[i].time += speedMin;
            auto t = (vec_frameParts[i].time);
            vec_frameParts[i].shp_object->transform->SetLocalPosition(leftUpEdge + (leftDownEdge - leftUpEdge) * t + thicknessOffset);

        }
        break;
        }

        if (vec_frameParts[i].time > 1.0f) {
            vec_frameParts[i].time = 0.0f;
            vec_frameParts[i].mode = (MoveMode)((vec_frameParts[i].mode + 1) % 5);
        }
    }
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::SquareParticleEmitter::Clone()
{
    return ObjectFactory::Create<SquareParticleEmitter>();
}

void ButiEngine::SquareParticleEmitter::ParticleInformationSet(Particle3D& arg_refParticle)
{
    float t = 0;
}
