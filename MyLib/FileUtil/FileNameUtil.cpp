#include "FileNameUtil.h"

namespace Util
{
	string FileNameUtil::getExtension(string file_path, bool with_dot)
	{
		//�g���q��.�̈ʒu�����߂�
		int dot_pos = file_path.rfind(".");
		if( dot_pos == string::npos ) return "";
		
		//�g���q��.�Ȃ��Ŏ擾�������ꍇ
		if( !with_dot )dot_pos++;

		//�g���q�̕�����؂�o��
		return file_path.substr(dot_pos, file_path.size() - dot_pos);
	}

	string FileNameUtil::getName( string file_path, bool with_extension)
	{
		//�g���q��.�̈ʒu�����߂�
		int dot_pos = file_path.rfind(".");
		if( dot_pos  == -1 ) // �f�B���N�g�����̎�
		{
			//// �f�B���N�g���̖��O��Ԃ�
			int last_slash_pos = file_path.rfind("/");
			int t_pos = file_path.rfind("\\");
			if(last_slash_pos < t_pos)
			{
				last_slash_pos = t_pos;
			}
			if( last_slash_pos == -1 ) return "";// �󕶎���
			return file_path.substr( last_slash_pos + 1, file_path.size() - last_slash_pos  );
		}
		
		//�Ō�̃X���b�V���̈ʒu�����߂�
		int last_slash_pos = file_path.rfind("/");
		int t_pos = file_path.rfind("\\");
		if(last_slash_pos < t_pos)
		{
			last_slash_pos = file_path.rfind("\\");
		}
		if( last_slash_pos == -1 )//�t�@�C�����݂̂̂Ƃ�
		{
			if( with_extension ) return file_path;
			else
			{
				int extension_length = file_path.size() - dot_pos;
				return file_path.substr( 0, file_path.size() - extension_length );
			}	
		}
		//if( file_path.rfind("\\") > last_slash_pos ) last_slash_pos = file_path.rfind("\\");

		//�Ԃ�������Ɋg���q���܂߂邩�ǂ���
		if( with_extension )
		{
			return file_path.substr( last_slash_pos+1, file_path.size() - last_slash_pos );
		}
		else
		{
			int extension_length = file_path.size() - dot_pos;
			return file_path.substr( last_slash_pos+1, file_path.size() -last_slash_pos - extension_length -1 );
		}
	}

	string FileNameUtil::getDirectoryName(string file_path, bool with_slash)
	{
		//�Ō�̃X���b�V���̈ʒu�����߂�
		int last_slash_pos = file_path.rfind("/");
		int pos_t = file_path.rfind("\\");
		if(last_slash_pos < pos_t )
		{
			last_slash_pos = pos_t;
		}
		if( -1 == last_slash_pos )
		{			
			return "";
		}
	
		//�p�X���Ō��/�Ȃ��Ŏ擾�������ꍇ
		if( !with_slash ) last_slash_pos--;

		//�f�B���N�g�����̕�����؂�o��
		return file_path.substr(0, last_slash_pos + 1);
	}

	string FileNameUtil::getSingleDirectoryName(string file_path, bool with_slash)
	{
		string dir_path = getDirectoryName( file_path, false );

		//�Ō�̃X���b�V���̈ʒu�����߂�
		int last_slash_pos = dir_path.rfind("/");
		int t_pos = dir_path.rfind("\\");
		if(last_slash_pos < t_pos)
		{
			last_slash_pos = t_pos;
		}
		if( -1 == last_slash_pos )
		{			
			return "";
		}

		//�p�X���Ō��/�Ȃ��Ŏ擾�������ꍇ
		string res =  dir_path.substr( last_slash_pos + 1, dir_path.size() - last_slash_pos );
		
		if(with_slash) res = dir_path.substr( last_slash_pos, dir_path.size() - last_slash_pos );
		return res;
	}

	string FileNameUtil::cutExtension( string file_path )
	{
		//�g���q��.�̈ʒu�����߂�
		int dot_pos = file_path.rfind(".");
		int extension_length = file_path.size() - dot_pos;
		
		return file_path.substr( 0, file_path.size() - extension_length );
	}

	string FileNameUtil::getParentDirectoryName( string file_path, bool with_slash)
	{
		string path = getDirectoryName( file_path );
		return getDirectoryName( file_path, with_slash );
	}

}