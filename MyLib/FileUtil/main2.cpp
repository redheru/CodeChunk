//C++
#include <vector>
#include <string>
#include <iostream>

#include "FileUtil.h"
using namespace Util;

using namespace std;

void main()
{
	//TCHAR path[MAX_STRING] = _T("C:\\tmp\\*");
	//Directory(path);

	//TCHAR path[] = _T("C:\\tmp\\");
	//TCHAR ext[] = _T("*.txt");
	//GetFileNames(path, ext);

	//TCHAR path[] = _T("C:\\tmp\\");
	//TCHAR ext[] = _T("*.txt");
	//getDir(path, ext);

	//tstring path = _T("C:\\tmp\\");
	//tstring ext = _T("*.txt");
	//vector<tstring> fileList;
	//GetFileNameInDirectory(fileList, path, ext);

	//string path = "C:\\tmp\\";
	//string ext = "*.txt";
	//vector<string> fileList;
	//Util::FileSystemUtil::getFileNameInDirectory(fileList, path, ext);

	vector<string> fileList;
	string path = "C:\\tmp";
	string ext = ".txt";
	vector<string> exts;
	exts.push_back(ext);
	exts.push_back(".bvh");
	//FileSystemUtil::getFileNameInDirectory(fileList, path, ext);
	FileSystemUtil::getFileNameInDirectory(fileList, path, exts, true);
	for(unsigned int i=0; i<fileList.size(); i++)
	{
		cout << fileList[i] << endl;
	}

	return;
}