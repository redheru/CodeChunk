// インクルードヘッダ
#include <windows.h>
#include <stdio.h>
#include <tchar.h>

//C++
#include <vector>
#include <string>
#include <iostream>

//#include "FileNameUtil.h"
//#include "FileSystemUtil.h"
#include <FileUtil/FileUtil.h>
using namespace Util;

using namespace std;
typedef std::basic_string<TCHAR> tstring;

// 定数宣言
#define MAX_STRING 1024                             // 文字列最大数

#ifndef UNICODE

// 関数プロトタイプ
void GetFileNameInDirectory(vector<string>& into, string dirPath, string ext);
void getFileName(vector<string>& into, string dirPath, string ext, string fileName);
void getDirName(vector<string>& into, string dirPath, string ext, string dirName);

void GetFileNameInDirectory(vector<string>& into, string dirPath, string ext = "*")
{
	HANDLE dFind;
	TCHAR  dir[MAX_STRING];
	WIN32_FIND_DATA lp;

	// ファイル検索条件合成
	_stprintf_s(dir, _T("%s%s"), dirPath.c_str() , ext.c_str());

	// ファイル検索
	if ((dFind = FindFirstFile(dir, &lp)) != INVALID_HANDLE_VALUE) {
		if ((lp.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY) {
			getFileName(into, dirPath, ext, lp.cFileName);
		}
		while(FindNextFile(dFind, &lp) != 0) {
			if ((lp.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY) {
				getFileName(into, dirPath, ext, lp.cFileName);
			}
		}
		FindClose(dFind);
	}

	// ディレクトリ検索条件合成
	_stprintf_s(dir, _T("%s%s"), dirPath.c_str(), _T("*"));

	// ディレクトリ検索
	if ((dFind = FindFirstFile(dir, &lp)) == INVALID_HANDLE_VALUE) return;
	if ((lp.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY) {
		getDirName(into, dirPath, ext, lp.cFileName);
	}
	while(FindNextFile(dFind, &lp) != 0) {
		if ((lp.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY) {
			getDirName(into, dirPath, ext, lp.cFileName);
		}
	}
	FindClose(dFind);

// リターン
  return;
}
void getFileName(vector<string>& into, string dirPath, string ext, string fileName)
{
	string file;
	//TCHAR file[MAX_STRING];

	//ファイルフルパス合成
	file = dirPath + fileName;
	//_stprintf_s(file, _T("%s%s"), dirPath, fileName);

	// ファイル処理
	into.push_back(file);
	_tprintf(_T("File -- %s\n"), file.c_str());

	// リターン
	return;
}	


void getDirName(vector<string>& into, string dirPath, string ext, string dirName)
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
	_tprintf(_T("Dir -- %s\n"), dir.c_str());

	// サブディレクトリ再帰処理
	GetFileNameInDirectory(into, dir, ext);

	// リターン
	return;
}


#else

// 関数プロトタイプ
void GetFileNameInDirectory(vector<tstring>& into, tstring dirPath, tstring ext);
void getFileName(vector<tstring>& into, tstring dirPath, tstring ext, tstring fileName);
void getDirName(vector<tstring>& into, tstring dirPath, tstring ext, tstring dirName);

void GetFileNameInDirectory(vector<tstring>& into, tstring dirPath, tstring ext= _T("*"))
{
	HANDLE dFind;
	TCHAR  dir[MAX_STRING];
	WIN32_FIND_DATA lp;

	// ファイル検索条件合成
	_stprintf_s(dir, _T("%s%s"), dirPath.c_str(), ext.c_str());

	// ファイル検索
	if ((dFind = FindFirstFile(dir, &lp)) != INVALID_HANDLE_VALUE) {
		if ((lp.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY) {
			getFileName(into, dirPath, ext, lp.cFileName);
		}
		while(FindNextFile(dFind, &lp) != 0) {
			if ((lp.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY) {
				getFileName(into, dirPath, ext, lp.cFileName);
			}
		}
		FindClose(dFind);
	}

	// ディレクトリ検索条件合成
	_stprintf_s(dir, _T("%s%s"), dirPath.c_str(), _T("*"));

	// ディレクトリ検索
	if ((dFind = FindFirstFile(dir, &lp)) == INVALID_HANDLE_VALUE) return;
	if ((lp.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY) {
		getDirName(into, dirPath, ext, lp.cFileName);
	}
	while(FindNextFile(dFind, &lp) != 0) {
		if ((lp.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY) {
			getDirName(into, dirPath, ext, lp.cFileName);
		}
	}
	FindClose(dFind);

// リターン
  return;
}
void getFileName(vector<tstring>& into, tstring dirPath, tstring ext, tstring fileName)
{
	tstring file;

	//ファイルフルパス合成
	//file = dirPath + fileName;
	file.append(dirPath.append(fileName));
	//_stprintf_s(file, _T("%s%s"), dirPath.c_str(), fileName.c_str());

	// ファイル処理
	into.push_back(file);
	_tprintf(_T("File -- %s\n"), file.c_str());

	// リターン
	return;
}	


void getDirName(vector<tstring>& into, tstring dirPath, tstring ext, tstring dirName)
{
	tstring dir;

	// カレント、親ディレクトリスキップ
	if ((dirName.compare(_T("..")) == 0) || (dirName.compare(_T(".")) == 0)) return;

	//ファイルフルパス合成
	dir.append(dirPath.append(dirName.append(_T("\\"))));
	//_stprintf_s(dir, _T("%s%s\\"), dirPath, dirName);

	// ファイル処理
	_tprintf(_T("Dir -- %s\n"), dir.c_str());

	// サブディレクトリ再帰処理
	GetFileNameInDirectory(into, dir, ext);

	// リターン
	return;
}


#endif

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

//void Directory(TCHAR path[]);
//bool GetFileNames(TCHAR dirPath[], TCHAR ext[]);

//void getDir(TCHAR dirPath[], TCHAR ext[]);
//void dirJob(TCHAR dirPath[], TCHAR ext[], TCHAR dirName[]);
//void fileJob(TCHAR dirPath[], TCHAR ext[], TCHAR fileName[]);


//void Directory(TCHAR path[])
//{
//	TCHAR subpath[MAX_STRING*2];
//	TCHAR temp[MAX_STRING];
//	WIN32_FIND_DATA lp;
//
//	_tcscpy_s(temp, path);
//
//	HANDLE h = FindFirstFile(temp, &lp);
//
//	temp[_tcslen(temp)-1]='\0';//*を消す
//	do
//	{
//		if((lp.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
//			&& _tcscmp(lp.cFileName,_T(".."))!=0 && _tcscmp(lp.cFileName, _T("."))!=0)
//		{//親ディレクトリ("..")またはカレントディレクトリ(".")以外のディレクトリを発見したとき
//			//_stprintf_s(subpath, _T("%S%S\\*"), temp, lp.cFileName);//subpathにディレクトリのフルパスが入る
//			_stprintf_s(subpath, _T("%s%s\\*"), temp, lp.cFileName);//subpathにディレクトリのフルパスが入る
//
//			//ディレクトリパスの表示
//			TCHAR dirpath[MAX_STRING*2];	
//			_tcscpy_s(dirpath, subpath);
//			dirpath[_tcslen(dirpath)-1]='\0';//*を消す
//			_tprintf(_T("Dir -- %s\n"), dirpath);
//
//			Directory(subpath);//ディレクトリ内を更に探索
//		}
//		if((lp.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)!=FILE_ATTRIBUTE_DIRECTORY)
//		{
//           // lp.cFilenameでファイル名が分かる
//           // wsprintf(出力文字列,"%s%s",temp,lp.cFileName);とすれば
//           // ファイルのフルパスが分かる
//			TCHAR filepath[MAX_STRING*2];
//			_stprintf_s(filepath, _T("%s%s"), temp, lp.cFileName);
//			_tprintf(_T("File -- %s\n"), filepath);//フルパスを表示
//
//			//ファイル名だけ表示
//			//_tprintf(_T("File -- %s\n"), lp.cFileName);
//
//           //こちらはファイルサイズ
//           //size+=lp.nFileSizeHigh*MAXDWORD+lp.nFileSizeLow ;			
//		}
//	}while(FindNextFile(h,&lp));//次のファイルの検索
//
//	FindClose(h);
//}
//
//bool GetFileNames(TCHAR dirPath[], TCHAR ext[] = _T("*"))
//{
//	HANDLE hFind;
//	TCHAR subpath[MAX_STRING*2];
//	TCHAR temp[MAX_STRING];
//	WIN32_FIND_DATA lp;
//
//	//_tcscpy_s(temp, path);
//	_stprintf_s(temp, _T("%s%s"), dirPath, ext);//ディレクトリのパスと拡張子の指定をつなぐ
//
//
//	//最初のファイル検索
//	if( (hFind = FindFirstFile(temp, &lp)) == INVALID_HANDLE_VALUE)
//	{//検索は成功したか？
//		printf("検索失敗\n");
//		return false;
//	}
//
//	//temp[_tcslen(temp)-1]='\0';//*を消す
//	do
//	{
//		if((lp.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
//			&& _tcscmp(lp.cFileName,_T(".."))!=0 && _tcscmp(lp.cFileName, _T("."))!=0)
//		{//親ディレクトリ("..")またはカレントディレクトリ(".")以外のディレクトリを発見したとき
//			_stprintf_s(subpath, _T("%s%s\\"), dirPath, lp.cFileName);//dirpathにカレントディレクトリまでのパスが入る
//
//			//ディレクトリパスの表示
//			//TCHAR dirpath[MAX_STRING*2];	
//			//_tcscpy_s(dirpath, subpath);
//			//dirpath[_tcslen(dirpath)-1]='\0';//*を消す
//			_tprintf(_T("Dir -- %s\n"), subpath);
//
//			_stprintf_s(subpath, _T("%s%s"), subpath, ext);//dirpathにカレントディレクトリまでのパスが入る
//			if( !GetFileNames(subpath, ext) )//ディレクトリ内を更に探索
//			{return false;}
//		}
//		if((lp.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)!=FILE_ATTRIBUTE_DIRECTORY)
//		{
//           // lp.cFilenameでファイル名が分かる
//           // wsprintf(出力文字列,"%s%s",temp,lp.cFileName);とすれば
//           // ファイルのフルパスが分かる
//			TCHAR filepath[MAX_STRING*2];
//			_stprintf_s(filepath, _T("%s%s"), dirPath, lp.cFileName);
//			_tprintf(_T("File -- %s\n"), filepath);//フルパスを表示
//
//			//ファイル名だけ表示
//			//_tprintf(_T("File -- %s\n"), lp.cFileName);
//
//           //こちらはファイルサイズ
//           //size+=lp.nFileSizeHigh*MAXDWORD+lp.nFileSizeLow ;			
//		}
//	}while(FindNextFile(hFind,&lp));//次のファイルの検索
//
//	//検索終了
//	FindClose(hFind);
//	return true;
//}
//
//void getDir(TCHAR dirPath[], TCHAR ext[] = _T("*"))
//{
//	HANDLE dFind;
//	TCHAR  dir[MAX_STRING];
//	WIN32_FIND_DATA lp;
//
//	// ファイル検索条件合成
//	_stprintf_s(dir, _T("%s%s"), dirPath, ext);
//
//	// ファイル検索
//	if ((dFind = FindFirstFile(dir, &lp)) != INVALID_HANDLE_VALUE) {
//		if ((lp.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY) {
//			fileJob(dirPath, ext, lp.cFileName);
//		}
//		while(FindNextFile(dFind, &lp) != 0) {
//			if ((lp.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY) {
//				fileJob(dirPath, ext, lp.cFileName);
//			}
//		}
//		FindClose(dFind);
//	}
//
//	// ディレクトリ検索条件合成
//	_stprintf_s(dir, _T("%s%s"), dirPath, _T("*"));
//
//	// ディレクトリ検索
//	if ((dFind = FindFirstFile(dir, &lp)) == INVALID_HANDLE_VALUE) return;
//	if ((lp.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY) {
//		dirJob(dirPath, ext, lp.cFileName);
//	}
//	while(FindNextFile(dFind, &lp) != 0) {
//		if ((lp.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY) {
//			dirJob(dirPath, ext, lp.cFileName);
//		}
//	}
//	FindClose(dFind);
//
//// リターン
//  return;
//}
//
//
//void fileJob(TCHAR dirPath[], TCHAR ext[], TCHAR fileName[])
//{
//	TCHAR file[MAX_STRING];
//
//	//ファイルフルパス合成
//	_stprintf_s(file, _T("%s%s"), dirPath, fileName);
//
//	// ファイル処理
//	_tprintf(_T("File -- %s\n"), file);
//
//	// リターン
//	return;
//}	
//
//void dirJob(TCHAR dirPath[], TCHAR ext[], TCHAR dirName[])
//{
//	TCHAR dir[MAX_STRING];
//
//	// カレント、親ディレクトリスキップ
//	if (_tcscmp(dirName,_T("..")) == 0 || _tcscmp(dirName,_T(".")) == 0) return;
//
//	//ファイルフルパス合成
//	_stprintf_s(dir, _T("%s%s\\"), dirPath, dirName);
//
//	// ファイル処理
//	_tprintf(_T("Dir -- %s\n"), dir);
//
//	// サブディレクトリ再帰処理
//	getDir(dir, ext);
//
//	// リターン
//	return;
//}
//


///*
//========================================================================
//  ディレクトリ検索(win32 コンソール) - 再帰呼び出し処理
//========================================================================
//*/
//// インクルードヘッダ
//#include <windows.h>
//#include <stdio.h>
//#include <tchar.h>
//
//// 定数宣言
//#define MAX_STRING 1025                              // 文字列最大数
//
//// 関数プロトタイプ
//VOID GetDir(LPCTSTR fld, LPCTSTR wcd);
//VOID FileJob(LPCTSTR fld, LPCTSTR wcd, LPCTSTR fnm, DWORD atr);
//VOID DirJob(LPCTSTR fld, LPCTSTR wcd, LPCTSTR fnm, DWORD atr);
//
//// グローバル変数
//  TCHAR  fld[MAX_STRING] = TEXT("C:\\tmp\\");              // 検索ディレクトリ
//  TCHAR  wcd[MAX_STRING] = TEXT("*.txt");                  // 検索ワイルドカード
//  TCHAR  log[MAX_STRING] = TEXT("C:\\tmp\\filelist.txt");  // リスト出力ファイル
//  HANDLE fno;                                        // リスト出力ハンドル
///*
//========================================================================
//  メイン
//========================================================================
//*/
//int main(int argc, char **argv)
//{
//  int i;
//
//// パラメタ取得
//  for (i = 1; i < argc; i++) {
//    switch (i) {
//      case 1:
//        lstrcpy(fld, LPCWSTR(argv[i]));  // 開始フォルダ
//        break;
//      case 2:
//        lstrcpy(wcd, LPCWSTR(argv[i]));  // ワイルドカード
//        break;
//      case 3:
//        lstrcpy(log, LPCWSTR(argv[i]));  // リスト出力ファイル
//        break;
//    }
//  }
//
//// 処理開始
//  printf("ディレクトリ検索開始...\n");
//
//// ファイルリスト出力用ファイルオープン
//  if ((fno = CreateFile(log, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
//    FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE) return -1;
//
//// ディレクトリ検索開始
//  GetDir(fld, wcd);
//
//// ファイルリスト出力用ファイルクローズ
//  CloseHandle(fno);
//
//// 処理終了
//  printf("\nディレクトリ検索終了。\n");
//  return 0;
//}
///*
//========================================================================
//  ディレクトリ取得
//========================================================================
//*/
//VOID GetDir(LPCTSTR fld, LPCTSTR wcd)
//{
//  HANDLE dno;
//  TCHAR  dir[MAX_STRING];
//  WIN32_FIND_DATA fil;
//
//// ファイル検索条件合成
//  lstrcat(lstrcpy(dir, fld), wcd);
//
//// ファイル検索
//  if ((dno = FindFirstFile(dir, &fil)) != INVALID_HANDLE_VALUE) {
//    if ((fil.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY) {
//      FileJob(fld, wcd, fil.cFileName, fil.dwFileAttributes);
//    }
//    while(FindNextFile(dno, &fil) != 0) {
//      if ((fil.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY) {
//        FileJob(fld, wcd, fil.cFileName, fil.dwFileAttributes);
//      }
//    }
//    FindClose(dno);
//  }
//
//// ディレクトリ検索条件合成
//  lstrcat(lstrcpy(dir, fld), TEXT("*"));
//
//// ディレクトリ検索
//  if ((dno = FindFirstFile(dir, &fil)) == INVALID_HANDLE_VALUE) return;
//  if ((fil.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY) {
//    DirJob(fld, wcd, fil.cFileName, fil.dwFileAttributes);
//  }
//  while(FindNextFile(dno, &fil) != 0) {
//    if ((fil.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY) {
//      DirJob(fld, wcd, fil.cFileName, fil.dwFileAttributes);
//    }
//  }
//  FindClose(dno);
//
//// リターン
//  return;
//}
///*
//========================================================================
//  ファイル処理
//========================================================================
//*/
//VOID FileJob(LPCTSTR fld, LPCTSTR wcd, LPCTSTR fnm, DWORD atr)
//{
//  TCHAR fil[MAX_STRING];
//  TCHAR rec[MAX_STRING];
//  DWORD rlen;
//
//// ファイルフルパス合成
//  lstrcat(lstrcpy(fil, fld), fnm);
//
//// ファイル処理
//  printf("ファイル名: %s - %d\n", fil, atr);
//  wsprintf(rec, TEXT("ファイル名: %s - %d\n"), fil, atr);
//  WriteFile(fno, rec, lstrlen(rec), &rlen, NULL);
//
//// リターン
//  return;
//}
///*
//========================================================================
//  ディレクトリ処理
//========================================================================
//*/
//VOID DirJob(LPCTSTR fld, LPCTSTR wcd, LPCTSTR fnm, DWORD atr)
//{
//  TCHAR dir[MAX_STRING];
//
//// カレント、親ディレクトリスキップ
//  if (_tcscmp(fnm, TEXT(".")) == 0 || _tcscmp(fnm, TEXT("..")) == 0) return;
//
//// ディレクトリフルパス合成
//  lstrcat(lstrcat(lstrcpy(dir, fld), fnm), TEXT("\\"));
//
//// ディレクトリ処理
//  //printf("フォルダ名: %s - %d\n", dir, atr);
//  printf(".");
//
//// サブディレクトリ再帰処理
//  GetDir(dir, wcd);
//
//// リターン
//  return;
//}
