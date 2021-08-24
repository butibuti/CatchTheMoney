#pragma once

#include"stdafx.h"

namespace ButiEngine {
namespace Collision {
struct CollisionLayerInfo {

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(size);
		archive(offset);
		archive(level);
		archive(isCheckSame);
		archive(vec_checkOtherLayerIndex);
	}
	Vector3 size;
	Vector3 offset;
	int level;
	bool isCheckSame;
	std::vector<int> vec_checkOtherLayerIndex;
};
struct ColiisionLayerLoadData {
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(vec_collisionLayerInfo);
	}
	std::vector<CollisionLayerInfo> vec_collisionLayerInfo;
};

std::shared_ptr<ICollisionManager> CreateCollisionManager(const int arg_layerCount);

}


void OutputCereal(const Collision:: ColiisionLayerLoadData& v,  const std::string& path);

void InputCereal(Collision::ColiisionLayerLoadData& v, const std::string& path);
}