#include"stdafx.h"
#include"CSVData.h"
#include"Helper/StringHelper.h"

ButiEngine::CSVReader::CSVReader()
{
}

ButiEngine::CSVReader::~CSVReader()
{
}

std::shared_ptr< ButiEngine::CSVData>  ButiEngine::CSVReader::GetMatrix(const std::string& filePath)
{
	std::ifstream ifs(filePath);
	std::string source = std::string(std::istreambuf_iterator<char>(ifs),
		std::istreambuf_iterator<char>());

	auto splitedSource = StringHelper::Split(source, "\n");

	auto spS= StringHelper::Split(splitedSource.at(0), ",");

	auto output =std::make_shared< CSVData>(spS.size(),splitedSource.size());

	for (unsigned int i = 0; i < splitedSource.size(); i++) {
		auto row = splitedSource.at(i);
		auto nums = StringHelper::Split(row, ",");
		for (unsigned int j = 0; j < nums.size(); j++) {
			output->data[j][i] =std::stoi( nums.at(j));
		}
	}

	return output;
}
