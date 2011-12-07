

#include <string>
#include <cout>
#include <fstream>

#include <Util/Util.h> //トークン管理とか

using namespace std;

void main()
{
	string fileName = "test.txt"

	ifstream ifs( file_name.c_str() );
	if(!ifs)
	{
		cout << "cannot open file : " << file_name << endl;
		return;
	}
	
	string line;
	while( getline( ifs, line ) )
	{
		vector<string> tokens = StringUtil::Tokenize(line, ", ");
		
		//空行
		if( 0 == tokens.size()) continue;

		//コメント行
		if(tokens[0].at(0) == '%') continue;

		//各セクションを読み込んでいく
		if("#InputType" == tokens[0])
		{//コントローラの入力の登録
			


		}
		else if("#CommandType" == tokens[0])
		{//コマンドの登録
		


		}
	}
}