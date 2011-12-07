#include "FileSystemUtil.h"

namespace Util{

	 // 文字列最大数
	const int FileSystemUtil::MAX_STRING = 1024;

	//void FileSystemUtil::getFileNameInDirectory(vector<string>& into, string dirPath, string ext)
	//{
	//	HANDLE dFind;
	//	TCHAR  dir[MAX_STRING];
	//	WIN32_FIND_DATA lp;

	//	// ファイル検索条件合成
	//	_stprintf_s(dir, _T("%s%s"), dirPath.c_str() , ext.c_str());

	//	// ファイル検索
	//	if ((dFind = FindFirstFile(dir, &lp)) != INVALID_HANDLE_VALUE) {
	//		if ((lp.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY) {
	//			getFileName(into, dirPath, ext, lp.cFileName);
	//			//string fileName = toString(lp.cFileName);
	//			//getDirName(into, dirPath, ext, fileName);
	//		}
	//		while(FindNextFile(dFind, &lp) != 0) {
	//			if ((lp.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY) {
	//				getFileName(into, dirPath, ext, lp.cFileName);
	//				//string fileName = toString(lp.cFileName);
	//				//getDirName(into, dirPath, ext, fileName);
	//			}
	//		}
	//		FindClose(dFind);
	//	}

	//	// ディレクトリ検索条件合成
	//	_stprintf_s(dir, _T("%s%s"), dirPath.c_str(), _T("*"));

	//	// ディレクトリ検索
	//	if ((dFind = FindFirstFile(dir, &lp)) == INVALID_HANDLE_VALUE) return;
	//	if ((lp.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY) {
	//		getDirName(into, dirPath, ext, lp.cFileName);
	//		//string dirName = toString(lp.cFileName);
	//		//getDirName(into, dirPath, ext, dirName);
	//	}
	//	while(FindNextFile(dFind, &lp) != 0) {
	//		if ((lp.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY) {
	//			getDirName(into, dirPath, ext, lp.cFileName);
	//			//string dirName = toString(lp.cFileName);
	//			//getDirName(into, dirPath, ext, dirName);
	//		}
	//	}
	//	FindClose(dFind);

	//// リターン
	//  return;
	//}

	void FileSystemUtil::getFileNameInDirectory(vector<string>& into, string dirPath, string ext, bool recursion)
	{
		HANDLE dFind;
		TCHAR  dir[MAX_STRING];
		WIN32_FIND_DATA lp;

		// 拡張子のエラーチェック
		if(ext.length() > 0)
		{
			if(ext[0] == '.')
				ext = "*" + ext; 
		}else{
			_tprintf(_T("no extension"));
			return;
		}

		// ディレクトリパスのエラーチェック
		if(dirPath.length() > 0)
		{
			if(dirPath[dirPath.length()-1] != '\\')
				dirPath.append("\\");
		}else{
			_tprintf(_T("no directory path"));
			return;
		}

		// ファイル検索条件合成
		_stprintf_s(dir, _T("%s%s"), dirPath.c_str() , ext.c_str());

		// ファイル検索
		if ((dFind = FindFirstFile(dir, &lp)) != INVALID_HANDLE_VALUE) {
			if ((lp.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY) {
				getFileName(into, dirPath, ext, lp.cFileName);
				//string fileName = toString(lp.cFileName);
				//getDirName(into, dirPath, ext, fileName);
			}
			while(FindNextFile(dFind, &lp) != 0) {
				if ((lp.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY) {
					getFileName(into, dirPath, ext, lp.cFileName);
					//string fileName = toString(lp.cFileName);
					//getDirName(into, dirPath, ext, fileName);
				}
			}
			FindClose(dFind);
		}

		if(recursion) //ディレクトリの検索は再帰的に検索する場合のみ行う
		{
			// ディレクトリ検索条件合成
			_stprintf_s(dir, _T("%s%s"), dirPath.c_str(), _T("*"));

			// ディレクトリ検索
			if ((dFind = FindFirstFile(dir, &lp)) == INVALID_HANDLE_VALUE) return;
			if ((lp.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY) {
				getDirName(into, dirPath, ext, lp.cFileName, recursion);
				//string dirName = toString(lp.cFileName);
				//getDirName(into, dirPath, ext, dirName);
			}
			while(FindNextFile(dFind, &lp) != 0) {
				if ((lp.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY) {
					getDirName(into, dirPath, ext, lp.cFileName, recursion);
					//string dirName = toString(lp.cFileName);
					//getDirName(into, dirPath, ext, dirName);
				}
			}
		}
		FindClose(dFind);

		// リターン
		return;
	}

	void FileSystemUtil::getFileNameInDirectory(vector<string>& into, string dirPath, vector<string> ext, bool recursion)
	{
		for(unsigned int i=0; i<ext.size(); i++)
		{
			vector<string> result;
			getFileNameInDirectory(result, dirPath, ext[i], recursion);
			for(unsigned int j=0; j<result.size();j++)
			{
				into.push_back(result[j]);
			}
		}
		return;
	}


	void FileSystemUtil::getFileName(vector<string>& into, string dirPath, string ext, string fileName)
	{
		string file;
		//TCHAR file[MAX_STRING];

		//ファイルフルパス合成
		file = dirPath + fileName;
		//_stprintf_s(file, _T("%s%s"), dirPath, fileName);

		// ファイル処理
		into.push_back(file);
		//_tprintf(_T("File -- %s\n"), file.c_str());

		// リターン
		return;
	}	


	void FileSystemUtil::getDirName(vector<string>& into, string dirPath, string ext, string dirName, bool recursion)
	{
		string dir;
		//TCHAR dir[MAX_STRING];

		// カレント、親ディレクトリスキップ
		if (dirName == ("..") || dirName == (".")) return;
		//if (_tcscmp(dirName,_T("..")) == 0 || _tcscmp(dirName,_T(".")) == 0) return;

		//ファイルフルパス合成
		dir = dirPath + dirName + "\\";
		//_stprintf_s(dir, _T("%s%s\\"), dirPath, dirName);

		// ファイル処理
		//_tprintf(_T("Dir -- %s\n"), dir.c_str());

		// サブディレクトリ再帰処理
		getFileNameInDirectory(into, dir, ext, recursion);

		// リターン
		return;
	}

	//string FileSystemUtil::toString(WCHAR src[])
	//{
	//	char dst[MAX_STRING];
	//	size_t wLen = 0;
	//	errno_t err = 0;

	//	//ロケール指定
	//	setlocale(LC_ALL,"japanese");
	//	//変換
	//	err = wcstombs_s(&wLen, dst, sizeof(dst), src, _TRUNCATE);

	//	string dstString = dst;
	//	return dstString;
	//}

}
