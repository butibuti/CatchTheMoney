#pragma once
#include"../Common/ButiMath.h"
namespace ButiEngine {


	struct ShaderVariable
	{
		Matrix4x4 World;
		Matrix4x4 View;
		Matrix4x4 Projection;
		Matrix4x4 MVP;
		ShaderVariable() {
			memset(this, 0, sizeof(256));
		};
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(World);
			archive(View);
			archive(Projection);
			archive(MVP);
		}
		bool ShowUI() { return false; }
	};


	struct Fog {

		Vector4 fogColor;
		Vector4 cameraPos;
		Vector2 fogCoord;
		Vector2 pixelScale;
		Vector4 worldAnimationParam;
		Matrix4x4 shadowVP;
		float Time=0.0;
		Fog() {
			memset(this,0,sizeof(256));
		}

		bool ShowUI() { return false; }

		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(fogColor);
			archive(cameraPos);
			archive(fogCoord);
			archive(worldAnimationParam);
			archive(shadowVP);
			archive(Time);
		}

	};

	struct ObjectInformation {
		Vector4 lightDir;
		Vector4 color=Vector4(1.0f,1.0f,1.0f,1.0f);
		Vector2 Tiling=Vector2(1.0f,1.0f);
		Vector2 OffSet;
		Vector4 ExInfo;
		ObjectInformation() {
			memset(this, 0, sizeof(256));
		};
		~ObjectInformation() {

		}

		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(lightDir);
			archive(color);
			archive(Tiling);
			archive(OffSet);
			archive(ExInfo);
		}

		bool ShowUI() { 
			bool output = false;
			if (GUI::DragFloat4("LightDirection", lightDir, 0.01f, -500.0f, 500.0f)) {
				output= true;
			}
			if (GUI::DragFloat4("Color", color, 0.01f, -500.0f, 500.0f)) {
				output = true;
			}
			if (GUI::DragFloat2("Tiling",Tiling, 0.01f, -500.0f, 500.0f)) {
				output = true;
			}
			if (GUI::DragFloat2("Offset",OffSet, 0.01f, -500.0f, 500.0f)) {
				output = true;
			}
			if (GUI::DragFloat4("ExInfo",ExInfo, 0.01f, -500.0f, 500.0f)) {
				output = true;
			}
			return output;
		}
	};
	struct GausVariable {

		GausVariable() {
			memset(this, 0, sizeof(256));
		}

		Vector4 gausOffset[16];
		float GaussianDistribution(const Vector2& pos, float rho);
		
		void CalcGaus(const int width, const int height, const Vector2& dir, const float deviation);
		bool ShowUI() {
			static float deviation=0.0f;
			static Vector2 dir = Vector2(1,0);
			static Vector2 scale = Vector2(480,270);
			bool output = false;
			GUI::Text("Deviation");
			if (GUI::DragFloat("##deviation", deviation, 0.1f, 0.0f, 1000.0f)) {
				output = true;
			}
			GUI::Text("Direction");
			if (GUI::DragFloat2("##dir", dir, 0.1f, 0.0f, 1000.0f)) {
				output = true;
			}
			GUI::Text("Scale");
			if (GUI::DragFloat2("##scale", scale, 0.1f, 0.0f, 1000.0f)) {
				output = true;
			}

			if (output) {
				CalcGaus(scale.x, scale.y, dir, deviation);
			}

			return output;
		}
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(gausOffset);
		}
	};

	struct MaterialVariable {
		Vector4 emissive=Vector4(1.0f,1.0f,1.0f,1.0f);
		Vector4 diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		Vector4 ambient = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		Vector4 specular = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		MaterialVariable() {
			memset(this, 0, sizeof(256));
		};
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(emissive);
			archive(diffuse);
			archive(ambient);
			archive(specular);
		}
		bool ShowUI() { 

			bool ret = false;
			if (GUI::DragFloat4("Emissive", &emissive.x, 0.01f, 0.0f, 1.0f)) {
				ret = true;
			}
			if (GUI::DragFloat4("Diffuse", &diffuse.x, 0.01f, 0.0f, 1.0f)) {
				ret = true;
			}
			if (GUI::DragFloat4("Specular", &specular.x, 0.01f, 0.0f, 1.0f)) {
				ret = true;
			}
			if (GUI::DragFloat4("Ambient", &ambient.x, 0.01f, 0.0f, 1.0f)) {
				ret= true;
			}
			return ret; 
		}
	};

	struct TestGSVariable {
		Vector4 pushPower;
		float bottom = 0;
		TestGSVariable() {
			memset(this, 0, sizeof(256));
		}
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(bottom);
			archive(pushPower);
		}

		bool ShowUI() { 

			bool ret = false;
			if (GUI::DragFloat("Time", &pushPower.w, 0.02f, 0.0f, 10.0f)) {

				ret = true;
			}
			if (GUI::DragFloat("FloorHight", &bottom, 0.02f, -100.0f, 100.0f)) {

				ret = true;
			}
			return ret;
		}
	};
	struct ParticleParameter {
		Vector4 startColor=Vector4(0,0,0,1);
		Vector4 endColor = Vector4(1, 1, 1, 1);
		float time=0.0f ;
		float power=0.0f ;
		int noise=0 ;
		float outer=20.0f;
		float center=1.0f;
		float size=1.0f;
		float minSize=0.1f;
		float rotationPase = 5.0f;

		ParticleParameter() {
			memset(this, 0, sizeof(256));
		}

		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(startColor);
			archive(endColor);
			archive(time);
			archive(power);
			archive(noise);
			archive(outer);
			archive(center);
			archive(size);
			archive(minSize);
			archive(rotationPase);
		}

		bool ShowUI();
	};
}
