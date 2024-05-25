#pragma once
#include "Util.h"
class String
{
public:
	static void SplitString
	(std::vector<std::string>* result, std::string origin, std::string tok);

	static void SplitString
	(std::vector<std::wstring>* result, std::wstring origin, std::wstring tok);

	static bool StartWith(std::string str, std::string comp);
	static bool StartWith(std::wstring str, std::wstring comp);

	static bool Contain(std::string str, std::string comp);
	static bool Contain(std::wstring str, std::wstring comp);

	static void Replace(std::string* str, std::string comp, std::string rep);
	static void Replace(std::wstring* str, std::wstring comp, std::wstring rep);

	static std::string ToString(DirectX::XMFLOAT3 vec3, int precision = 5);
	static std::wstring ToWstring(DirectX::XMFLOAT3 vec3, int precision = 5);

	static std::wstring ToWstring(std::string str);
	static std::string ToString(std::wstring str);
};