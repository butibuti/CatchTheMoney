#include"stdafx.h"
#include "Resource_RealTimeMesh.h"

void ButiEngine::Resource_RealTimeMesh::SetBackupData(std::shared_ptr<BackupDataBase> arg_backupData)
{
    backupData = arg_backupData;
}

UINT ButiEngine::Resource_RealTimeMesh::GetVertexCount()
{
    return verticesCount;
}

UINT ButiEngine::Resource_RealTimeMesh::GetIndexCount()
{
    return indexCount;
}


const ButiEngine::BackupDataBase& ButiEngine::Resource_RealTimeMesh::GetBackUpdata() const
{
    return *backupData.get();
}

std::shared_ptr<ButiEngine::BackupDataBase> ButiEngine::Resource_RealTimeMesh::GetBackupData_Row()
{
    return backupData;
}

bool ButiEngine::Resource_RealTimeMesh::GetPosRayCast(Vector3* arg_p_pos, Vector3* arg_p_normal, const Line& arg_line)
{
    return backupData->IsHitRay(arg_line, arg_p_pos,arg_p_normal);
}

