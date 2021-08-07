#include"stdafx.h"
//#include"../../Header/GameParts/Application.h"
#include "Util.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include"stb/stb_image_write.h"


void ButiEngine::Util::WStringtoMultiByte(const std::wstring & src, std::string & dest)
{
	size_t i;
	char *pMBstr = new char[src.length() * MB_CUR_MAX + 1];
	wcstombs_s(
		&i,
		pMBstr,
		src.length() * MB_CUR_MAX + 1,
		src.c_str(),
		_TRUNCATE	//���ׂĕϊ��ł��Ȃ�������؂�̂�
	);
	dest = pMBstr;
	delete[] pMBstr;
}

void ButiEngine::Util::ToPNG(const std::string& arg_filePath, void* arg_data, const int width, const int height, const int stride, const int size)
{
	stbi_write_png(arg_filePath.c_str(), width, height, size, arg_data, stride);
}

std::string ButiEngine::Util::WStringToString(std::wstring oWString)

{
	// wstring �� SJIS
	int iBufferSize = WideCharToMultiByte(CP_OEMCP, 0, oWString.c_str()
		, -1, (char *)NULL, 0, NULL, NULL);

	// �o�b�t�@�̎擾
	CHAR* cpMultiByte = new CHAR[iBufferSize];

	// wstring �� SJIS
	WideCharToMultiByte(CP_OEMCP, 0, oWString.c_str(), -1, cpMultiByte
		, iBufferSize, NULL, NULL);

	// string�̐���
	std::string oRet(cpMultiByte, cpMultiByte + iBufferSize - 1);

	// �o�b�t�@�̔j��
	delete[] cpMultiByte;

	// �ϊ����ʂ�Ԃ�
	return(oRet);
}

std::wstring ButiEngine::Util::StringToWString(std::string oString) {
	// SJIS �� wstring
	int iBufferSize = MultiByteToWideChar(CP_ACP, 0, oString.c_str()
		, -1, (wchar_t*)NULL, 0);

	// �o�b�t�@�̎擾
	wchar_t* cpUCS2 = new wchar_t[iBufferSize];

	// SJIS �� wstring
	MultiByteToWideChar(CP_ACP, 0, oString.c_str(), -1, cpUCS2
		, iBufferSize);

	// string�̐���
	std::wstring oRet(cpUCS2, cpUCS2 + iBufferSize - 1);

	// �o�b�t�@�̔j��
	delete[] cpUCS2;

	// �ϊ����ʂ�Ԃ�
	return(oRet);
}



void ButiEngine::ThrowButiException_Runtime(const std::wstring & message1, const std::wstring & message2, const std::wstring & message3)
{//throw runtime_error function
	std::wstring outputMessage = message1;
	outputMessage += L"\n";
	outputMessage += message2;
	outputMessage += L"\n";
	outputMessage += message3;
	std::string output;

	Util::WStringtoMultiByte(outputMessage, output);
	throw std::runtime_error(output);
}

void ButiEngine::ThrowButiException_Runtime(const std::string & meesage1, const std::string & message2, const std::string & message3)
{//throw runtime_error function
	std::string outputMessage = meesage1;
	outputMessage += "\n";
	outputMessage += message2;
	outputMessage += "\n";
	outputMessage += message3;
	throw std::runtime_error(outputMessage);
}

std::shared_ptr< std::random_device> ButiEngine::ButiRandom::shp_rnd_device = nullptr;
std::shared_ptr<std::mt19937>ButiEngine::ButiRandom::shp_mt = nullptr;// 
std::shared_ptr< std::uniform_int_distribution<>> ButiEngine::ButiRandom::shp_randRange = nullptr;
void ButiEngine::ButiRandom::Initialize()
{
	shp_rnd_device = std::make_shared<std::random_device>();
	shp_mt = std::make_shared<std::mt19937>((*shp_rnd_device)());
	shp_randRange = std::make_shared< std::uniform_int_distribution<>>(0, 1);
}
