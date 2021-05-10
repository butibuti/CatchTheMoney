#pragma once
#include<map>
#include"Bone.h"
namespace ButiEngine {
	struct LarpData {
		LarpData(BYTE* arg_data);
		LarpData();

		CubicBezierCurve2D rotationBezier;
		CubicBezierCurve2D translationXBezier;
		CubicBezierCurve2D translationYBezier;
		CubicBezierCurve2D translationZBezier;
	};
	struct Pose {
		Pose(){}
		Pose(const Vector3& arg_position, const Vector3& arg_rotation,const Vector3& arg_scale);
		Vector3 position;
		Quat rotation;
		Vector3 scale;
	};

	struct IKActive {
		std::wstring boneName;
		bool isActive;
	};

	struct MotionKeyFrameData {
		MotionKeyFrameData(){}
		MotionKeyFrameData( const UINT arg_endFrame, const Vector3& arg_position, const Vector3& arg_rotation,const Vector3& arg_scale=Vector3(1,1,1));
		MotionKeyFrameData(const UINT arg_endFrame, const Pose& arg_pose);

		UINT endFrame=0;
		Pose pose;
		LarpData larp;
	};


	class IMotionTimeLine :public IObject {
	public:
		void Initialize()override {}
		void PreInitialize()override {}
		virtual void Start() = 0;
		virtual void FrameSet(const float) = 0;
		virtual void Reset() = 0;
		virtual UINT GetEndFrame() = 0;
		virtual void PreStart() = 0;
		virtual void SetBone(std::shared_ptr<Bone> arg_shp_bone) = 0;
		virtual void SetMotionData(const MotionKeyFrameData& arg_motionData) = 0;
		virtual void SetMotionData(const std::vector<MotionKeyFrameData>& arg_motionDatas)=0;
		virtual void LocalPoseSet(std::shared_ptr<Transform> arg_parentBone) = 0;
		virtual std::string GetContentsName() = 0;
	};
	class BoneMotionTimeLine :public IMotionTimeLine {
	public:
		BoneMotionTimeLine();
		~BoneMotionTimeLine();
		void SetBone(std::shared_ptr<Bone> arg_shp_bone)override;
		void PreStart()override;
		void Reset()override;
		void SetMotionData(const MotionKeyFrameData& arg_motionData) override;
		void SetMotionData(const std::vector<MotionKeyFrameData>& arg_motionDatas) override;
		void Start() override;
		void FrameSet(const float frame)override;
		UINT GetEndFrame() override;
		void LocalPoseSet(std::shared_ptr<Transform> arg_parentBone)override;
		void SetBoneName(const std::string& arg_name);
		std::string GetContentsName()override;
	private:
		bool isActive=false;
		Quat initRotate;
		Vector3 initPosition;
		std::vector< MotionKeyFrameData > vec_data;
		std::vector< MotionKeyFrameData >::iterator nowMotionItr;
		std::vector< MotionKeyFrameData >::iterator befMotionItr;
		std::shared_ptr<Bone> bone;
		std::string boneName;
	};
	class ModelAnimation :public IObject
	{
	public:
		void Initialize() override {}
		void PreInitialize() override {}
		void Update(const float arg_frame);
		void IKTest();
		void Reset();
		void PreMotionStart(std::shared_ptr<Transform> arg_parentBoneTransform);
		std::shared_ptr<IMotionTimeLine> AddMotionTimeLine( std::shared_ptr<IMotionTimeLine> arg_motion);
		bool SetLoop(const bool arg_isLoop);
		void SetBoneDrawObj(std::shared_ptr<IBoneObject> arg_shp_boneDrawObj);
		float GetFrame()const;
	private:
		bool isActive = true;
		bool isRoop = false;
		float frame = 0.0f;
		UINT endFrame = 0;
		

		std::vector<std::shared_ptr<IMotionTimeLine>> vec_timeLines;
		std::shared_ptr<IBoneObject> shp_boneDrawObj;
	};

}