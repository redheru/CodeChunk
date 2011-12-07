#include "FileSystemUtil.h"

namespace Util{

	 // ������ő吔
	const int FileSystemUtil::MAX_STRING = 1024;

	//void FileSystemUtil::getFileNameInDirectory(vector<string>& into, string dirPath, string ext)
	//{
	//	HANDLE dFind;
	//	TCHAR  dir[MAX_STRING];
	//	WIN32_FIND_DATA lp;

	//	// �t�@�C��������������
	//	_stprintf_s(dir, _T("%s%s"), dirPath.c_str() , ext.c_str());

	//	// �t�@�C������
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

	//	// �f�B���N�g��������������
	//	_stprintf_s(dir, _T("%s%s"), dirPath.c_str(), _T("*"));

	//	// �f�B���N�g������
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

	//// ���^�[��
	//  return;
	//}

	void FileSystemUtil::getFileNameInDirectory(vector<string>& into, string dirPath, string ext, bool recursion)
	{
		HANDLE dFind;
		TCHAR  dir[MAX_STRING];
		WIN32_FIND_DATA lp;

		// �g���q�̃G���[�`�F�b�N
		if(ext.length() > 0)
		{
			if(ext[0] == '.')
				ext = "*" + ext; 
		}else{
			_tprintf(_T("no extension"));
			return;
		}

		// �f�B���N�g���p�X�̃G���[�`�F�b�N
		if(dirPath.length() > 0)
		{
			if(dirPath[dirPath.length()-1] != '\\')
				dirPath.append("\\");
		}else{
			_tprintf(_T("no directory path"));
			return;
		}

		// �t�@�C��������������
		_stprintf_s(dir, _T("%s%s"), dirPath.c_str() , ext.c_str());

		// �t�@�C������
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

		if(recursion) //�f�B���N�g���̌����͍ċA�I�Ɍ�������ꍇ�̂ݍs��
		{
			// �f�B���N�g��������������
			_stprintf_s(dir, _T("%s%s"), dirPath.c_str(), _T("*"));

			// �f�B���N�g������
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

		// ���^�[��
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

		//�t�@�C���t���p�X����
		file = dirPath + fileName;
		//_stprintf_s(file, _T("%s%s"), dirPath, fileName);

		// �t�@�C������
		into.push_back(file);
		//_tprintf(_T("File -- %s\n"), file.c_str());

		// ���^�[��
		return;
	}	


	void FileSystemUtil::getDirName(vector<string>& into, string dirPath, string ext, string dirName, bool recursion)
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
		//_tprintf(_T("Dir -- %s\n"), dir.c_str());

		// �T�u�f�B���N�g���ċA����
		getFileNameInDirectory(into, dir, ext, recursion);

		// ���^�[��
		return;
	}

	//string FileSystemUtil::toString(WCHAR src[])
	//{
	//	char dst[MAX_STRING];
	//	size_t wLen = 0;
	//	errno_t err = 0;

	//	//���P�[���w��
	//	setlocale(LC_ALL,"japanese");
	//	//�ϊ�
	//	err = wcstombs_s(&wLen, dst, sizeof(dst), src, _TRUNCATE);

	//	string dstString = dst;
	//	return dstString;
	//}

}
