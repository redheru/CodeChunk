#include "FileNameUtil.h"

namespace Util
{
	string FileNameUtil::getExtension(string file_path, bool with_dot)
	{
		//拡張子の.の位置を求める
		int dot_pos = file_path.rfind(".");
		if( dot_pos == string::npos ) return "";
		
		//拡張子を.なしで取得したい場合
		if( !with_dot )dot_pos++;

		//拡張子の部分を切り出す
		return file_path.substr(dot_pos, file_path.size() - dot_pos);
	}

	string FileNameUtil::getName( string file_path, bool with_extension)
	{
		//拡張子の.の位置を求める
		int dot_pos = file_path.rfind(".");
		if( dot_pos  == -1 ) // ディレクトリ名の時
		{
			//// ディレクトリの名前を返す
			int last_slash_pos = file_path.rfind("/");
			int t_pos = file_path.rfind("\\");
			if(last_slash_pos < t_pos)
			{
				last_slash_pos = t_pos;
			}
			if( last_slash_pos == -1 ) return "";// 空文字列
			return file_path.substr( last_slash_pos + 1, file_path.size() - last_slash_pos  );
		}
		
		//最後のスラッシュの位置を求める
		int last_slash_pos = file_path.rfind("/");
		int t_pos = file_path.rfind("\\");
		if(last_slash_pos < t_pos)
		{
			last_slash_pos = file_path.rfind("\\");
		}
		if( last_slash_pos == -1 )//ファイル名のみのとき
		{
			if( with_extension ) return file_path;
			else
			{
				int extension_length = file_path.size() - dot_pos;
				return file_path.substr( 0, file_path.size() - extension_length );
			}	
		}
		//if( file_path.rfind("\\") > last_slash_pos ) last_slash_pos = file_path.rfind("\\");

		//返す文字列に拡張子を含めるかどうか
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
		//最後のスラッシュの位置を求める
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
	
		//パスを最後の/なしで取得したい場合
		if( !with_slash ) last_slash_pos--;

		//ディレクトリ名の部分を切り出す
		return file_path.substr(0, last_slash_pos + 1);
	}

	string FileNameUtil::getSingleDirectoryName(string file_path, bool with_slash)
	{
		string dir_path = getDirectoryName( file_path, false );

		//最後のスラッシュの位置を求める
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

		//パスを最後の/なしで取得したい場合
		string res =  dir_path.substr( last_slash_pos + 1, dir_path.size() - last_slash_pos );
		
		if(with_slash) res = dir_path.substr( last_slash_pos, dir_path.size() - last_slash_pos );
		return res;
	}

	string FileNameUtil::cutExtension( string file_path )
	{
		//拡張子の.の位置を求める
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