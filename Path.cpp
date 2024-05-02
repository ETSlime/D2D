#include "Path.h"
#include "String.h"

bool Path::ExistFile(std::string path)
{
	return ExistFile(String::ToWstring(path));
}

bool Path::ExistFile(std::wstring path)
{
	DWORD fileValue = GetFileAttributes(path.c_str());

	//ﾆﾄﾀﾏ ﾁｸﾀ鄙ｩｺﾎ ﾃｼﾅｩ
	return fileValue < 0xFFFFFFFF;
	//ﾆﾄﾀﾏﾀﾌ ﾁｸﾀ酩ﾒｰ豼・True ｾｻｰ豼・False
}

//GetFIleAttributes() ->ﾆﾄﾀﾏﾀﾌｳｪ ﾆ嶸ﾇ ｼﾓｼｺﾀｻ ｾﾋｾﾆｺｸｴﾂ ﾇﾔｼ・

bool Path::ExistDirectory(std::string path)
{
	return ExistDirectory(String::ToWstring(path));
}

bool Path::ExistDirectory(std::wstring path)
{
	DWORD attribute = GetFileAttributes(path.c_str());

	BOOL temp = (attribute != INVALID_FILE_ATTRIBUTES &&
		(attribute & FILE_ATTRIBUTE_DIRECTORY));

	//INVALID_FILE_ATRIBUTES =0xFFFFFFF ﾆﾄﾀﾏｼﾓｼｺﾀｻ ｺﾒｷｯｿﾀｴﾂｵ･ ｽﾇﾆﾐ , FILE_ATTRIBUTE_DIRECTORY ｵｺﾅ荳ｮ ﾃｼﾅｩ
	return temp == TRUE;
}

std::string Path::Combine(std::string path1, std::string path2)
{
	return path1 + path2;
}

std::wstring Path::Combine(std::wstring path1, std::wstring path2)
{
	return path1 + path2;
}

std::string Path::Combine(std::vector<std::string> paths)
{
	std::string temp = "";
	for (std::string path : paths)
		temp += path;

	return temp;
}

std::wstring Path::Combine(std::vector<std::wstring> paths)
{
	std::wstring temp = L"";
	for (std::wstring path : paths)
		temp += path;

	return temp;
}

std::string Path::GetDirectoryName(std::string path)
{
	String::Replace(&path, "\\", "/");
	size_t index = path.find_last_of('/');


	return path.substr(0, index + 1);
}

std::wstring Path::GetDirectoryName(std::wstring path)
{
	String::Replace(&path, L"\\", L"/");
	size_t index = path.find_last_of('/');

	return path.substr(0, index + 1);
}

std::string Path::GetExtension(std::string path)
{
	String::Replace(&path, "\\", "/");
	size_t index = path.find_last_of('.');

	return path.substr(index + 1, path.length());;
}

std::wstring Path::GetExtension(std::wstring path)
{
	String::Replace(&path, L"\\", L"/");
	size_t index = path.find_last_of('.');

	return path.substr(index + 1, path.length());;
}

std::string Path::GetFileName(std::string path)
{
	String::Replace(&path, "\\", "/");
	size_t index = path.find_last_of('/');

	return path.substr(index + 1, path.length());
}

std::wstring Path::GetFileName(std::wstring path)
{
	String::Replace(&path, L"\\", L"/");
	size_t index = path.find_last_of('/');

	return path.substr(index + 1, path.length());
}

std::string Path::GetFileNameWithoutExtension(std::string path)
{
	std::string fileName = GetFileName(path);

	size_t index = fileName.find_last_of('.');
	return fileName.substr(0, index);
}

std::wstring Path::GetFileNameWithoutExtension(std::wstring path)
{
	std::wstring fileName = GetFileName(path);

	size_t index = fileName.find_last_of('.');
	return fileName.substr(0, index);
}

const WCHAR* Path::ImageFilter = L"Image\0*.png;*.bmp;*.jpg";
const WCHAR* Path::ShaderFilter = L"HLSL file\0*.hlsl";
const WCHAR* Path::TextFilter = L"Text file\0*.txt";
const WCHAR* Path::TileFilter = L"Tile file\0*.png;*.bmp;*.jpg;*.json;*.data";
const WCHAR* Path::TMapFilter = L"TMap file\0*.tmap";

void Path::OpenFileDialog(std::wstring file, const WCHAR* filter, std::wstring folder, std::function<void(std::wstring)> func, HWND hwnd)
{
	WCHAR name[255];
	wcscpy_s(name, file.c_str());

	std::wstring tempFolder = folder;

	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = name;
	ofn.lpstrFileTitle = (LPWSTR)L"ass";
	ofn.nMaxFile = 255;
	ofn.lpstrInitialDir = tempFolder.c_str();
	ofn.Flags = OFN_NOCHANGEDIR;

	if (GetOpenFileName(&ofn) == TRUE)
	{
		if (func != NULL)
		{
			std::wstring loadName = name;
			String::Replace(&loadName, L"\\", L"/");

			func(loadName);
		}
	}
}
/*
typedef struct tagOFN {
  DWORD         lStructSize; ｱｸﾁｶﾃｼﾀﾇ ﾅｩｱ簔､, ｹ・ｮﾀﾎﾀｻ ﾀｧﾇﾘ ｻ鄙・ﾊ
  HWND          hwndOwner; ﾇﾚｵ鯊ﾇ ﾁﾖﾀﾎﾀｻ ﾁ､, ﾁﾖﾀﾎﾀﾌ ｵﾉ ﾀｩｵｵｿ・｡ ｾｻｰ豼・NULL
  HINSTANCE     hInstance;  ﾀﾎｽｺﾅﾏｽｺ ﾇﾚｵ鯊ｻ ﾁ､
  LPCTSTR       lpstrFilter; ﾆﾄﾀﾏ ﾇ・ﾄｵ鯊ｻ ｱｸｺﾐﾇﾘﾁﾙ ﾇﾊﾅﾍｵ鯊ｻ ﾁ､ ｿｩｷｯｰｳﾀﾇ ﾇﾊﾅﾍｸｦ ｵｿｽﾃｿ｡ ﾁ､ｰ｡ｴﾉ
  LPTSTR        lpstrCustomFilter; ｻ鄙・ﾚｰ｡ ｸｸｵ・ﾄｿｽｺﾅﾒ ﾇﾊﾅﾍｸｦ ﾁ､
  DWORD         nMaxCustFilter; ﾄｿｽｺﾅﾒ ﾇﾊﾅﾍﾀﾇ ｹﾛﾀﾇ ｱ貘ﾌｸｦ ﾁ､ ﾃﾖｼﾒ 40ﾀﾚ
  DWORD         nFilterIndex;  ﾇﾊﾅﾍｵ鯊ﾇ ｸﾏﾁﾟ ｻ鄙・ﾒ ﾇﾊﾅﾍﾀﾇ ﾀﾎｵｦｽｺｸｦ ﾁ､
  LPTSTR        lpstrFile; ﾆﾄﾀﾏﾀﾌｸｧ ﾁ､ｿ｡ ﾃｹｹｰｷﾎ ｳｪﾅｸｳｯ ﾆﾄﾀﾏｸ暲ｻ ﾁ､, ﾇﾊｿ萓ｻｽﾃ NULL
  DWORD         nMaxFile;
  LPTSTR        lpstrFileTitle;
  DWORD         nMaxFileTitle;
  LPCTSTR       lpstrInitialDir;
  LPCTSTR       lpstrTitle;
  DWORD         Flags;
  WORD          nFileOffset;
  WORD          nFileExtension;
  LPCTSTR       lpstrDefExt;
  LPARAM        lCustData;
  LPOFNHOOKPROC lpfnHook;
  LPCTSTR       lpTemplateName;
#if (_WIN32_WINNT >= 0x0500)
  void *        pvReserved;
  DWORD         dwReserved;
  DWORD         FlagsEx;
#endif // (_WIN32_WINNT >= 0x0500)
} OPENFILENAME, *LPOPENFILENAME;

*/

void Path::SaveFileDialog(std::wstring file, const WCHAR* filter, std::wstring folder, std::function<void(std::wstring)> func, HWND hwnd)
{
	WCHAR name[255];
	wcscpy_s(name, file.c_str());

	std::wstring tempFolder = folder;

	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = name;
	ofn.lpstrFileTitle = (LPWSTR)L"저장하기";
	ofn.nMaxFile = 255;
	ofn.lpstrInitialDir = tempFolder.c_str();
	ofn.Flags = OFN_NOCHANGEDIR;

	if (GetSaveFileName(&ofn) == TRUE)
	{
		if (func != NULL)
		{
			std::wstring saveName = name;
			String::Replace(&saveName, L"\\", L"/");

			func(saveName);
		}
	}

}

void Path::GetFiles(std::vector<std::string>* files, std::string path, std::string filter, bool bFindSubFolder)
{
	std::vector<std::wstring> wFiles;
	std::wstring wPath = String::ToWstring(path);
	std::wstring wFilter = String::ToWstring(filter);

	GetFiles(&wFiles, wPath, wFilter, bFindSubFolder);

	for (std::wstring str : wFiles)
		files->push_back(String::ToString(str));
}

//path : ../Temp/
//filter : *.txt
void Path::GetFiles(std::vector<std::wstring>* files, std::wstring path, std::wstring filter, bool bFindSubFolder)
{

	std::wstring file = path + filter;

	WIN32_FIND_DATA findData;
	HANDLE handle = FindFirstFile(file.c_str(), &findData);
	if (handle != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (findData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
			{
				if (bFindSubFolder == true && findData.cFileName[0] != '.')
				{
					std::wstring folder = path + std::wstring(findData.cFileName) + L"/";
					GetFiles(files, folder, filter, bFindSubFolder);

				}
			}
			else //ﾇﾘｴ・ｰ豺ﾎｿ｡ ｼﾓｼｺﾀﾌ ﾆ嶸｡ ｾﾆｴﾒｶｧ. ﾁ・ﾆﾄﾀﾏﾀﾏｶｧ
			{
				std::wstring fileName = path + std::wstring(findData.cFileName);
				files->push_back(fileName);
			}
		} while (FindNextFile(handle, &findData));

		FindClose(handle);
	}
}

void Path::CreateFolder(std::string path)
{
	CreateFolder(String::ToWstring(path));
}

void Path::CreateFolder(std::wstring path)
{
	if (ExistDirectory(path) == false)
		CreateDirectory(path.c_str(), NULL);
	//ｵｺﾅ荳ｮｰ｡ ﾁｸﾀ酩ﾏﾁ・ｾﾊﾀｻｰ豼・｡ CREATEDIRECTORY // ﾆ嶸・ｻｼｺ 
}

void Path::CreateFolders(std::string path)
{
	CreateFolders(String::ToWstring(path));
}

void Path::CreateFolders(std::wstring path)
{
	String::Replace(&path, L"\\", L"/");

	std::vector<std::wstring> folders;
	String::SplitString(&folders, path, L"/");

	std::wstring temp = L"";
	for (std::wstring folder : folders)
	{
		temp += folder + L"/";

		CreateFolder(temp);
	}
	//ﾁ､ｵﾈ ﾀｧﾄ｡ｿ｡ ﾆ嶸・ｻｼｺ
}
