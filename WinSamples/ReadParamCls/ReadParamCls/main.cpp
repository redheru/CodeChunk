

#include <string>
#include <cout>
#include <fstream>

#include <Util/Util.h> //�g�[�N���Ǘ��Ƃ�

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
		
		//��s
		if( 0 == tokens.size()) continue;

		//�R�����g�s
		if(tokens[0].at(0) == '%') continue;

		//�e�Z�N�V������ǂݍ���ł���
		if("#InputType" == tokens[0])
		{//�R���g���[���̓��͂̓o�^
			


		}
		else if("#CommandType" == tokens[0])
		{//�R�}���h�̓o�^
		


		}
	}
}