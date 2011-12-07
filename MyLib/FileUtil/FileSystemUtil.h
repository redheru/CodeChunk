#ifndef FILESYSTEMUTIL_H
#define FILESYSTEMUTIL_H

#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <locale.h>

//C++
#include <vector>
#include <string>

typedef std::basic_string<TCHAR> tstring;

namespace Util
{

using namespace std;

class FileSystemUtil
{
public:

	// �萔�錾
	static const int MAX_STRING; // ������ő吔

	// �֐��v���g�^�C�v
	//static void getFileNameInDirectory(vector<string>& into, string dirPath, string ext = "*");
	static void getFileName(vector<string>& into, string dirPath, string ext, string fileName);
	static void getDirName(vector<string>& into, string dirPath, string ext, string dirName, bool recursion);

	static void getFileNameInDirectory(vector<string>& into, string dirPath, string ext = "*", bool recursion = false);
	static void getFileNameInDirectory(vector<string>& into, string dirPath, vector<string> ext, bool recursion = false);


	//static string toString(WCHAR src[]);

}; //class FileSystemUtil

}//namespace Util

#endif
