#pragma once
#include"stdafx.h"
namespace ButiScript {
class Compiler;
}
namespace ButiEngine {
/// <summary>
/// コンパイル済みデータの読み込み
/// </summary>
/// <param name="arg_filePath">入力ファイルパス</param>
/// <param name="arg_compiler">コンパイラ(nullptrでデフォルトを使用)</param>
/// <returns>読み込んだプログラムデータ</returns>
std::shared_ptr<IResource_Script> CreateButiScriptResource(const std::string& arg_filePath ,std::shared_ptr<ButiScript::Compiler> arg_compiler=nullptr);
/// <summary>
/// プログラムのコンパイル
/// </summary>
/// <param name="arg_inputfilePath">コンパイルするファイルのパス</param>
/// <param name="arg_outputfilePath">コンパイル済みデータの出力先</param>
/// <param name="arg_compiler"></param>
void CompileButiScipt(const std::string& arg_inputfilePath, const std::string& arg_outputfilePath, std::shared_ptr<ButiScript::Compiler> arg_compiler = nullptr);
}