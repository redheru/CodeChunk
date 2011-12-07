
#include "StringUtil.h"
#include <stdlib.h>
#include <string>
#include <vector>
#include <cstring>
#include <fstream>
#include <iostream>


namespace Util
{
	std::string Trim(const std::string& str,  const char* delim=" \t\r\n")
	{
		using std::string;
		const int p1(str.find_first_not_of(delim));
		if(p1 == string::npos) return string();
	  
		const int p2(str.find_last_not_of(delim));
		return str.substr(p1, p2 - p1 + 1);
	}


	char* StringUtil::toCharP(std::string from)
	{
		using std::string;
		using std::vector;

		char * writable = new char[from.size() + 1];
		std::copy(from.begin(), from.end(), writable);
		writable[from.size()] = '\0';
		return writable;
	}

	void StringUtil::Tokenize( std::string target, std::string drop_delimiter, std::vector<std::string>& dst)
	{
		using std::string;
		using std::vector;

		dst.clear();

		char* str = toCharP(target);
		const char* delim = drop_delimiter.c_str(); //デリミタ
		char* token = NULL;

		token = strtok( str, delim );
		while ( token != NULL )
		{
			dst.push_back(Trim(token));
			token = strtok( NULL, delim );
		}
	}

	std::vector<std::string> StringUtil::Tokenize( std::string target, std::string drop_delimiter )
	{
		using std::string;
		using std::vector;

		vector<string> vec;
		StringUtil::Tokenize(target, drop_delimiter, vec);
		return vec;
	}
	std::string StringUtil::UnTokenize( std::vector<std::string> tokens, std::string delim )
	{
		using std::string;
		using std::vector;

		string res;
		for(unsigned int i = 0; i < tokens.size(); i++)
		{
			res =  res + tokens[i] + delim;
		}
		return res;
	}


	void StringUtil::AddWithoutDuplicatoin( std::vector<std::string>& target_vec, std::string add_str )
	{
		using std::string;
		using std::vector;

		// 重複しているかをチェック
		bool duplication = false;
		for(unsigned int i = 0; i < target_vec.size(); i++)
		{
			if( target_vec[i] == add_str )
			{
				duplication = true;
				break;
			}
		}
		// 重複していなければ追加
		if( !duplication ) target_vec.push_back( add_str );
	}

	std::vector<std::string> StringUtil::CombineStringVec( std::vector<std::string> vec1, std::vector<std::string> vec2 )
	{
		using std::string;
		using std::vector;

		// vec2のstringをvec1に追加して返す
		for(unsigned int i = 0; i < vec2.size(); i++)
		{
			string add_name = vec2[i];
			
			// 重複しているかをチェック
			bool duplication = false;
			for(unsigned int j = 0; j < vec1.size(); j++)
			{
				if( add_name == vec1[j] ) 
				{
					duplication = true;
					break;
				}
			}

			// 重複していなければ返り値の配列に追加
			if(!duplication) vec1.push_back( add_name );
		}
		return vec1;
	}



	//return_object[n] = n行目の文字列
	std::vector<std::string> StringUtil::GetStringVecFromFile( std::string file_name )
	{
		using std::ifstream;
		using std::string;
		using std::vector;


		vector<string> res;
		ifstream ifs( file_name.c_str() );
		
		string line;
		while( getline( ifs, line ) ) res.push_back( line );
		return res;
	}
	std::vector< std::vector<std::string> > StringUtil::TokenizeFromFile( std::string file_name, std::string drop_delimiter)
	{
		using std::string;
		using std::vector;

		vector< vector<string> > res;
		vector< string > lines = StringUtil::GetStringVecFromFile( file_name );
		for(unsigned int i = 0; i < lines.size(); i++)
		{
			res.push_back( StringUtil::Tokenize( lines[i], drop_delimiter) );
		}
		return res;
	}
	

	int StringUtil::toInt( std::string from )
	{
		return atoi( StringUtil::toCharP( from ) );
	}

	double StringUtil::toDouble( std::string from )
	{
		return atof( StringUtil::toCharP( from ) );
	}

	//void StringUtil::test()
	//{
	//	using std::cin;
	//	using std::string;
	//	using std::vector;

	//	string str("1,2,3,,5,6, 7,\"\",\8");
	//	//string str("");
	//	vector<string> tokens = StringUtil::Tokenize( str, " ," );
	//	for(unsigned int i = 0; i < (int)tokens.size(); i++)
	//	{
	//		printf( "token %d = %s \n", i , tokens[i].c_str() );
	//	}

	//	printf( "[class StringUtil test] finish. type 'q' to quit.\n" );
	//	string stop;
	//	cin >> stop;
	//	if( "q" == stop )exit(0);
	//}

} //namespace Util