#ifndef STRINGUTIL_H
#define STRINGUTIL_H

#include <string>
#include <vector>

namespace Util
{

class StringUtil
{
public:
	static char* toCharP(std::string from);
	static void Tokenize( std::string target, std::string drop_delimiter, std::vector<std::string>& dst);
	static std::vector<std::string> Tokenize( std::string target, std::string drop_delimiter );
	static std::string UnTokenize( std::vector<std::string> tokens, std::string delim );
	static void AddWithoutDuplicatoin( std::vector<std::string>& target_vec, std::string add_str );
	static std::vector<std::string> CombineStringVec( std::vector<std::string> vec1, std::vector<std::string> vec2 );
	//return_object[n] = nçsñ⁄ÇÃï∂éöóÒ
	static std::vector<std::string> GetStringVecFromFile( std::string file_name );
	static std::vector< std::vector<std::string> > TokenizeFromFile( std::string file_name, std::string drop_delimiter);
	static int toInt( std::string from );
	static double toDouble( std::string from );
	
	//static void test();
}; // class StringUtil

} //namespace Util

#endif