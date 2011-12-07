// �C���N���[�h�w�b�_
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

// �萔�錾
#define MAX_STRING 1024                             // ������ő吔

#ifndef UNICODE

// �֐��v���g�^�C�v
void GetFileNameInDirectory(vector<string>& into, string dirPath, string ext);
void getFileName(vector<string>& into, string dirPath, string ext, string fileName);
void getDirName(vector<string>& into, string dirPath, string ext, string dirName);

void GetFileNameInDirectory(vector<string>& into, string dirPath, string ext = "*")
{
	HANDLE dFind;
	TCHAR  dir[MAX_STRING];
	WIN32_FIND_DATA lp;

	// �t�@�C��������������
	_stprintf_s(dir, _T("%s%s"), dirPath.c_str() , ext.c_str());

	// �t�@�C������
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

	// �f�B���N�g��������������
	_stprintf_s(dir, _T("%s%s"), dirPath.c_str(), _T("*"));

	// �f�B���N�g������
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

// ���^�[��
  return;
}
void getFileName(vector<string>& into, string dirPath, string ext, string fileName)
{
	string file;
	//TCHAR file[MAX_STRING];

	//�t�@�C���t���p�X����
	file = dirPath + fileName;
	//_stprintf_s(file, _T("%s%s"), dirPath, fileName);

	// �t�@�C������
	into.push_back(file);
	_tprintf(_T("File -- %s\n"), file.c_str());

	// ���^�[��
	return;
}	


void getDirName(vector<string>& into, string dirPath, string ext, string dirName)
{
	string dir;
	//TCHAR dir[MAX_STRING];

	// �J�����g�A�e�f�B���N�g���X�L�b�v
	if (dirName == ("..") || dirName == (".")) return;
	//if (_tcscmp(dirName,_T("..")) == 0 || _tcscmp(dirName,_T(".")) == 0) return;

	//�t�@�C���t���p�X����
	dir = dirPath + dirName + "\\";
	//_stprintf_s(dir, _T("%s%s\\"), dirPath, dirName);

	// �t�@�C������
	_tprintf(_T("Dir -- %s\n"), dir.c_str());

	// �T�u�f�B���N�g���ċA����
	GetFileNameInDirectory(into, dir, ext);

	// ���^�[��
	return;
}


#else

// �֐��v���g�^�C�v
void GetFileNameInDirectory(vector<tstring>& into, tstring dirPath, tstring ext);
void getFileName(vector<tstring>& into, tstring dirPath, tstring ext, tstring fileName);
void getDirName(vector<tstring>& into, tstring dirPath, tstring ext, tstring dirName);

void GetFileNameInDirectory(vector<tstring>& into, tstring dirPath, tstring ext= _T("*"))
{
	HANDLE dFind;
	TCHAR  dir[MAX_STRING];
	WIN32_FIND_DATA lp;

	// �t�@�C��������������
	_stprintf_s(dir, _T("%s%s"), dirPath.c_str(), ext.c_str());

	// �t�@�C������
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

	// �f�B���N�g��������������
	_stprintf_s(dir, _T("%s%s"), dirPath.c_str(), _T("*"));

	// �f�B���N�g������
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

// ���^�[��
  return;
}
void getFileName(vector<tstring>& into, tstring dirPath, tstring ext, tstring fileName)
{
	tstring file;

	//�t�@�C���t���p�X����
	//file = dirPath + fileName;
	file.append(dirPath.append(fileName));
	//_stprintf_s(file, _T("%s%s"), dirPath.c_str(), fileName.c_str());

	// �t�@�C������
	into.push_back(file);
	_tprintf(_T("File -- %s\n"), file.c_str());

	// ���^�[��
	return;
}	


void getDirName(vector<tstring>& into, tstring dirPath, tstring ext, tstring dirName)
{
	tstring dir;

	// �J�����g�A�e�f�B���N�g���X�L�b�v
	if ((dirName.compare(_T("..")) == 0) || (dirName.compare(_T(".")) == 0)) return;

	//�t�@�C���t���p�X����
	dir.append(dirPath.append(dirName.append(_T("\\"))));
	//_stprintf_s(dir, _T("%s%s\\"), dirPath, dirName);

	// �t�@�C������
	_tprintf(_T("Dir -- %s\n"), dir.c_str());

	// �T�u�f�B���N�g���ċA����
	GetFileNameInDirectory(into, dir, ext);

	// ���^�[��
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
//	temp[_tcslen(temp)-1]='\0';//*������
//	do
//	{
//		if((lp.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
//			&& _tcscmp(lp.cFileName,_T(".."))!=0 && _tcscmp(lp.cFileName, _T("."))!=0)
//		{//�e�f�B���N�g��("..")�܂��̓J�����g�f�B���N�g��(".")�ȊO�̃f�B���N�g���𔭌������Ƃ�
//			//_stprintf_s(subpath, _T("%S%S\\*"), temp, lp.cFileName);//subpath�Ƀf�B���N�g���̃t���p�X������
//			_stprintf_s(subpath, _T("%s%s\\*"), temp, lp.cFileName);//subpath�Ƀf�B���N�g���̃t���p�X������
//
//			//�f�B���N�g���p�X�̕\��
//			TCHAR dirpath[MAX_STRING*2];	
//			_tcscpy_s(dirpath, subpath);
//			dirpath[_tcslen(dirpath)-1]='\0';//*������
//			_tprintf(_T("Dir -- %s\n"), dirpath);
//
//			Directory(subpath);//�f�B���N�g�������X�ɒT��
//		}
//		if((lp.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)!=FILE_ATTRIBUTE_DIRECTORY)
//		{
//           // lp.cFilename�Ńt�@�C������������
//           // wsprintf(�o�͕�����,"%s%s",temp,lp.cFileName);�Ƃ����
//           // �t�@�C���̃t���p�X��������
//			TCHAR filepath[MAX_STRING*2];
//			_stprintf_s(filepath, _T("%s%s"), temp, lp.cFileName);
//			_tprintf(_T("File -- %s\n"), filepath);//�t���p�X��\��
//
//			//�t�@�C���������\��
//			//_tprintf(_T("File -- %s\n"), lp.cFileName);
//
//           //������̓t�@�C���T�C�Y
//           //size+=lp.nFileSizeHigh*MAXDWORD+lp.nFileSizeLow ;			
//		}
//	}while(FindNextFile(h,&lp));//���̃t�@�C���̌���
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
//	_stprintf_s(temp, _T("%s%s"), dirPath, ext);//�f�B���N�g���̃p�X�Ɗg���q�̎w����Ȃ�
//
//
//	//�ŏ��̃t�@�C������
//	if( (hFind = FindFirstFile(temp, &lp)) == INVALID_HANDLE_VALUE)
//	{//�����͐����������H
//		printf("�������s\n");
//		return false;
//	}
//
//	//temp[_tcslen(temp)-1]='\0';//*������
//	do
//	{
//		if((lp.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
//			&& _tcscmp(lp.cFileName,_T(".."))!=0 && _tcscmp(lp.cFileName, _T("."))!=0)
//		{//�e�f�B���N�g��("..")�܂��̓J�����g�f�B���N�g��(".")�ȊO�̃f�B���N�g���𔭌������Ƃ�
//			_stprintf_s(subpath, _T("%s%s\\"), dirPath, lp.cFileName);//dirpath�ɃJ�����g�f�B���N�g���܂ł̃p�X������
//
//			//�f�B���N�g���p�X�̕\��
//			//TCHAR dirpath[MAX_STRING*2];	
//			//_tcscpy_s(dirpath, subpath);
//			//dirpath[_tcslen(dirpath)-1]='\0';//*������
//			_tprintf(_T("Dir -- %s\n"), subpath);
//
//			_stprintf_s(subpath, _T("%s%s"), subpath, ext);//dirpath�ɃJ�����g�f�B���N�g���܂ł̃p�X������
//			if( !GetFileNames(subpath, ext) )//�f�B���N�g�������X�ɒT��
//			{return false;}
//		}
//		if((lp.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)!=FILE_ATTRIBUTE_DIRECTORY)
//		{
//           // lp.cFilename�Ńt�@�C������������
//           // wsprintf(�o�͕�����,"%s%s",temp,lp.cFileName);�Ƃ����
//           // �t�@�C���̃t���p�X��������
//			TCHAR filepath[MAX_STRING*2];
//			_stprintf_s(filepath, _T("%s%s"), dirPath, lp.cFileName);
//			_tprintf(_T("File -- %s\n"), filepath);//�t���p�X��\��
//
//			//�t�@�C���������\��
//			//_tprintf(_T("File -- %s\n"), lp.cFileName);
//
//           //������̓t�@�C���T�C�Y
//           //size+=lp.nFileSizeHigh*MAXDWORD+lp.nFileSizeLow ;			
//		}
//	}while(FindNextFile(hFind,&lp));//���̃t�@�C���̌���
//
//	//�����I��
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
//	// �t�@�C��������������
//	_stprintf_s(dir, _T("%s%s"), dirPath, ext);
//
//	// �t�@�C������
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
//	// �f�B���N�g��������������
//	_stprintf_s(dir, _T("%s%s"), dirPath, _T("*"));
//
//	// �f�B���N�g������
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
//// ���^�[��
//  return;
//}
//
//
//void fileJob(TCHAR dirPath[], TCHAR ext[], TCHAR fileName[])
//{
//	TCHAR file[MAX_STRING];
//
//	//�t�@�C���t���p�X����
//	_stprintf_s(file, _T("%s%s"), dirPath, fileName);
//
//	// �t�@�C������
//	_tprintf(_T("File -- %s\n"), file);
//
//	// ���^�[��
//	return;
//}	
//
//void dirJob(TCHAR dirPath[], TCHAR ext[], TCHAR dirName[])
//{
//	TCHAR dir[MAX_STRING];
//
//	// �J�����g�A�e�f�B���N�g���X�L�b�v
//	if (_tcscmp(dirName,_T("..")) == 0 || _tcscmp(dirName,_T(".")) == 0) return;
//
//	//�t�@�C���t���p�X����
//	_stprintf_s(dir, _T("%s%s\\"), dirPath, dirName);
//
//	// �t�@�C������
//	_tprintf(_T("Dir -- %s\n"), dir);
//
//	// �T�u�f�B���N�g���ċA����
//	getDir(dir, ext);
//
//	// ���^�[��
//	return;
//}
//


///*
//========================================================================
//  �f�B���N�g������(win32 �R���\�[��) - �ċA�Ăяo������
//========================================================================
//*/
//// �C���N���[�h�w�b�_
//#include <windows.h>
//#include <stdio.h>
//#include <tchar.h>
//
//// �萔�錾
//#define MAX_STRING 1025                              // ������ő吔
//
//// �֐��v���g�^�C�v
//VOID GetDir(LPCTSTR fld, LPCTSTR wcd);
//VOID FileJob(LPCTSTR fld, LPCTSTR wcd, LPCTSTR fnm, DWORD atr);
//VOID DirJob(LPCTSTR fld, LPCTSTR wcd, LPCTSTR fnm, DWORD atr);
//
//// �O���[�o���ϐ�
//  TCHAR  fld[MAX_STRING] = TEXT("C:\\tmp\\");              // �����f�B���N�g��
//  TCHAR  wcd[MAX_STRING] = TEXT("*.txt");                  // �������C���h�J�[�h
//  TCHAR  log[MAX_STRING] = TEXT("C:\\tmp\\filelist.txt");  // ���X�g�o�̓t�@�C��
//  HANDLE fno;                                        // ���X�g�o�̓n���h��
///*
//========================================================================
//  ���C��
//========================================================================
//*/
//int main(int argc, char **argv)
//{
//  int i;
//
//// �p�����^�擾
//  for (i = 1; i < argc; i++) {
//    switch (i) {
//      case 1:
//        lstrcpy(fld, LPCWSTR(argv[i]));  // �J�n�t�H���_
//        break;
//      case 2:
//        lstrcpy(wcd, LPCWSTR(argv[i]));  // ���C���h�J�[�h
//        break;
//      case 3:
//        lstrcpy(log, LPCWSTR(argv[i]));  // ���X�g�o�̓t�@�C��
//        break;
//    }
//  }
//
//// �����J�n
//  printf("�f�B���N�g�������J�n...\n");
//
//// �t�@�C�����X�g�o�͗p�t�@�C���I�[�v��
//  if ((fno = CreateFile(log, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
//    FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE) return -1;
//
//// �f�B���N�g�������J�n
//  GetDir(fld, wcd);
//
//// �t�@�C�����X�g�o�͗p�t�@�C���N���[�Y
//  CloseHandle(fno);
//
//// �����I��
//  printf("\n�f�B���N�g�������I���B\n");
//  return 0;
//}
///*
//========================================================================
//  �f�B���N�g���擾
//========================================================================
//*/
//VOID GetDir(LPCTSTR fld, LPCTSTR wcd)
//{
//  HANDLE dno;
//  TCHAR  dir[MAX_STRING];
//  WIN32_FIND_DATA fil;
//
//// �t�@�C��������������
//  lstrcat(lstrcpy(dir, fld), wcd);
//
//// �t�@�C������
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
//// �f�B���N�g��������������
//  lstrcat(lstrcpy(dir, fld), TEXT("*"));
//
//// �f�B���N�g������
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
//// ���^�[��
//  return;
//}
///*
//========================================================================
//  �t�@�C������
//========================================================================
//*/
//VOID FileJob(LPCTSTR fld, LPCTSTR wcd, LPCTSTR fnm, DWORD atr)
//{
//  TCHAR fil[MAX_STRING];
//  TCHAR rec[MAX_STRING];
//  DWORD rlen;
//
//// �t�@�C���t���p�X����
//  lstrcat(lstrcpy(fil, fld), fnm);
//
//// �t�@�C������
//  printf("�t�@�C����: %s - %d\n", fil, atr);
//  wsprintf(rec, TEXT("�t�@�C����: %s - %d\n"), fil, atr);
//  WriteFile(fno, rec, lstrlen(rec), &rlen, NULL);
//
//// ���^�[��
//  return;
//}
///*
//========================================================================
//  �f�B���N�g������
//========================================================================
//*/
//VOID DirJob(LPCTSTR fld, LPCTSTR wcd, LPCTSTR fnm, DWORD atr)
//{
//  TCHAR dir[MAX_STRING];
//
//// �J�����g�A�e�f�B���N�g���X�L�b�v
//  if (_tcscmp(fnm, TEXT(".")) == 0 || _tcscmp(fnm, TEXT("..")) == 0) return;
//
//// �f�B���N�g���t���p�X����
//  lstrcat(lstrcat(lstrcpy(dir, fld), fnm), TEXT("\\"));
//
//// �f�B���N�g������
//  //printf("�t�H���_��: %s - %d\n", dir, atr);
//  printf(".");
//
//// �T�u�f�B���N�g���ċA����
//  GetDir(dir, wcd);
//
//// ���^�[��
//  return;
//}
