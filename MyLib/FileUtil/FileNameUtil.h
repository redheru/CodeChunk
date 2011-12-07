#ifndef FILENAMEUTIL_H
#define FILENAMEUTIL_H

#include <string>

namespace Util
{

using namespace std;


class FileNameUtil
{
public:

	static string getExtension(string file_path, bool with_dot = true);
	static string getName( string file_path, bool with_extension = true );
	static string getDirectoryName(string file_path, bool with_slash = false );
	static string getSingleDirectoryName(string file_path, bool with_slash = false);
	static string cutExtension( string file_path );
	static string getParentDirectoryName( string file_path, bool with_slash = false );

}; //class FileNameUtil

} //namespace Util

#endif

