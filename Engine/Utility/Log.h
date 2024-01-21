#pragma once
#include <Windows.h>
#include <string>
#include <format>

/// <summary>
/// メッセージを出力
/// </summary>
/// <param name="message">出力するメッセージ</param>
void Log(const std::string& message);

/// <summary>
/// stringをwstringに変換
/// </summary>
/// <param name="str">string文字列</param>
/// <returns>wstring文字列</returns>
std::wstring ConvertString(const std::string& str);

/// <summary>
/// wstringをstringに変換
/// </summary>
/// <param name="str">wstring文字列</param>
/// <returns>string文字列</returns>
std::string ConvertString(const std::wstring& str);
