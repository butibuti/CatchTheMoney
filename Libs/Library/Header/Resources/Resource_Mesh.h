#pragma once
#include"Vertex.h"
namespace ButiEngine {
	class GraphicDevice;
	class Resource_Mesh : public Resource,public IResource_Mesh
	{
	public:
		virtual void SetGraphicDevice(std::shared_ptr<GraphicDevice> arg_graphicDevice) = 0;
		void Initialize()override {}
		void PreInitialize()override {}
		void SetBackupData(std::shared_ptr<BackupDataBase> arg_backupData);
		virtual void Draw() = 0;
		UINT GetVertexCount();
		UINT GetIndexCount();
		bool GetPosRayCast(Vector3* arg_p_pos, Vector3* arg_p_normal, const Line& arg_line);
		const BackupDataBase& GetBackUpdata()const;
	protected:
		UINT verticesCount;
		UINT indexCount;
		UINT stride;
		bool canAccessWrite;
		bool isDataRefresh = true;

		std::shared_ptr<BackupDataBase> backupData;


	};

}