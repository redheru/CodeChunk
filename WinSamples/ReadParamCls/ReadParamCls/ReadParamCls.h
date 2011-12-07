#ifndef _READ_PARAM_CLS_H_
#define _READ_PARAM_CLS_H_

#include "../NinoUtil/StringUtil.hpp"

#include <vector>
#include <string>
#include <fstream>
#include <iostream>


using namespace std;

struct PsdData
{
	//情報
	string name;
	int dataNum;
	string type;
	bool isJoint;

	//データ
	vector<double> base;
	vector< vector<double> > data;

}; //struct PsdData

class PsdFile
{
public:
	PsdFile(){}
	PsdFile(string file_name)
	{
		this->Load( file_name );
	}

// >member
private:
	vector<PsdData> psdDatas;
	int frameNum;
	int fps;
	string fileName;

	//map
	map<string, int> psdDataIndex;
	map<int, string> psdDataName;

// >setter
public:
	void AddPsdData( PsdData& d )
	{
		this->psdDatas.push_back(d);
	}
	void setFileName( string file_name ){ this->fileName = file_name; }
	void setFps( int fps ) { this->fps = fps; }
	void setFrameNum( int frame_num ) { this->frameNum = frame_num; }

// >getter
	PsdData getPsdData( int index ) { return this->psdDatas[index]; }
	PsdData getPsdData( string name )
	{
		return getPsdData( this->psdDataIndex[name] );
	}
	vector<double> getPsdOneData( string psd_data_name, int frame )
	{
		return psdDatas[ psdDataIndex[psd_data_name] ].data[frame];
	}
	vector<double> getPsdOneData( int psd_data_index, int frame )
	{
		return psdDatas[psd_data_index].data[frame];
	}

	string getFileName( ) { return this->fileName; }
	int getFps() { return this->fps; }
	int getFrameNum() { return this->frameNum; }

	int getPsdDatasNum() { return (int)this->psdDatas.size(); }	

// >method
public:

	static void test()
	{
		PsdFile p;
		p.Load("test2.psd");
		p.Write("test2_out.psd");
		
		PsdFile p2;
		p2.Load("test2_out.psd");
		p2.Write("test2_out_out.psd");
	}

	////////////////////////////////////////////////////////
	// Write
	////////////////////////////////////////////////////////

	void Write()
	{
		Write( this->fileName );
	}
	void Write(string file_name)
	{
		printf("[PsdFile Write] %s\n", file_name.c_str() );
		ofstream ofs( file_name.c_str() );
		ofs << "# Puppet Sensor Data(.psd)" << endl;
		ofs << endl;
		
		//共通事項
		ofs << "################################" << endl;				
		ofs << "###SECTION CommonInformation" << endl;
		ofs << endl;	

		ofs << "##fps " << this->fps << endl;
		ofs << "##frameNum " << this->frameNum << endl;
	
		ofs << endl;	
		ofs << "###ENDSECTION" << endl;
		ofs << "################################" << endl;
		
		ofs << endl;
		ofs << endl;


		//データの定義
		ofs << "################################" << endl;				
		ofs << "###SECTION PsdData" << endl;
		ofs << endl;
		
		for(int i = 0; i < (int)this->psdDatas.size(); i++)
		{
			PsdData& d = psdDatas[i];
			ofs << "##name " << d.name << endl;
			ofs << "##dataNum " << d.dataNum << endl;
			ofs << "##type " << d.type << endl;
			ofs << "##isJoint " << d.isJoint << endl;
			ofs << endl;
		}
		
		ofs << endl;	
		ofs << "###ENDSECTION" << endl;		
		ofs << "################################" << endl;
		
		ofs << endl;
		ofs << endl;


		//データ本体
		ofs << "################################" << endl;				
		ofs << "###SECTION Data" << endl;
		ofs << endl;

		for(int i = 0; i < (int)this->psdDatas.size(); i++)
		{
			PsdData& d = psdDatas[i];

			//エラーチェック（データの数）
			if( d.dataNum != (int)d.data[0].size() || this->frameNum != (int)d.data.size() )
			{
				printf("[Write psd file] format error.\n");
				return;
			}

			//baseデータ出力
			ofs << "##Data " << d.name << endl;
			ofs << "##Base " << " ";
			for(int j = 0; j < (int)d.base.size()-1; j++)
			{
				ofs << d.base[j] << ", ";
			}
			ofs << d.base[(int)d.base.size()-1] << endl;

			//データ出力
			for(int j = 0; j < (int)d.data.size(); j++)
			{
				int k = 0;
				for(; k < (int)d.data[j].size()-1; k++)
				{
					ofs << d.data[j][k] << ", ";
				}
				ofs << d.data[j][k] <<endl;
			}
			ofs << endl;
		}
		ofs << endl;	
		ofs << "###ENDSECTION" << endl;		
		ofs << "################################" << endl;
		
		printf("[PsdFile Write] finished.\n");
	}

	////////////////////////////////////////////////////////
	// Load
	////////////////////////////////////////////////////////
	
	void Load(string file_name)
	{
#ifdef IS_DISPLAY_PSD_FILE_LOAD
		printf("[PsdFile Load] %s\n", file_name.c_str() );
#endif
		fileName = file_name;
		ifstream ifs( file_name.c_str() );
		if(!ifs)
		{
			printf( "cannot open file : %s\n", file_name.c_str() );
			return;
		}
		
		string line;
		while( getline( ifs, line ) )
		{
			vector<string> tokens = StringUtil::Tokenize(line, ", ");
			
			//空行
			if( 0 == tokens.size() ) continue;

			//各セクションを読み込んでいく	
			if("###SECTION" == tokens[0])
			{
				#ifdef IS_DISPLAY_PSD_FILE_LOAD
				printf( "[PsdFile Load] read section : %s\n", tokens[1].c_str() );
				#endif
				if( "CommonInformation" == tokens[1] )
				{
					ReadCommonInformation(ifs);
				}
				else if( "PsdData" == tokens[1] )
				{
					ReadPsdData(ifs);
				}
				else if( "Data" == tokens[1] )
				{
					ReadData(ifs);
				}
			}	
		}
		#ifdef IS_DISPLAY_PSD_FILE_LOAD
		printf("[PsdFile Load] finished.\n");
		#endif
	}

private:
	//
	//Loadの補助関数
	//
	void ReadCommonInformation( ifstream& ifs )
	{
		string line;
		
		// get fps
		this->fps = atoi( ReadUntilMatched(ifs, "##fps")[1].c_str() );
		this->frameNum = atoi( ReadUntilMatched(ifs, "##frameNum")[1].c_str() );
		ReadUntilMatched( ifs, "###ENDSECTION" );
	}

	void ReadPsdData( ifstream& ifs )
	{
		string line;
		
		while( getline( ifs, line) )
		{
			vector<string> tokens = StringUtil::Tokenize( line, ", " );
			
			if( 0 == tokens.size() ) continue;
			
			if( "##name" == tokens[0] )
			{
					
				PsdData temp_psd;

				temp_psd.name = tokens[1];
				temp_psd.dataNum = atoi( ReadUntilMatched(ifs, "##dataNum")[1].c_str() );
				temp_psd.type = ReadUntilMatched(ifs, "##type")[1];
				temp_psd.isJoint = atoi( ReadUntilMatched(ifs, "##isJoint")[1].c_str() );
				
				this->psdDatas.push_back( temp_psd );
				this->psdDataIndex.insert( map<string, int>::value_type( temp_psd.name, (int)psdDatas.size()-1 ));
				this->psdDataName.insert( map<int, string>::value_type( (int)psdDatas.size()-1, temp_psd.name ));
				
			}

			else if( "###ENDSECTION" == tokens[0] )
			{
				return;
			}
		}

		//ここに到達するとエラー
		printf("[Load PsdFile] format error.\n");	
	}

	void ReadData(ifstream& ifs)
	{
		string line;
		while( getline( ifs, line) )
		{
			vector<string> tokens = StringUtil::Tokenize( line, ", " );
			if( 0 == tokens.size() ) continue;

			if( "##Data" == tokens[0] )
			{
				string data_name = tokens[1];
				
				PsdData* d = NULL;
				//名前が一致するPsdDataを探す
				for( int i = 0; i < (int)this->psdDatas.size(); i++)
				{
					if( this->psdDatas[i].name == data_name )
					{
						d = &psdDatas[i];
						break;
					}
				}
				if( NULL == d ) printf( "[Load PsdFile] format error.\n" );

				//baseデータの読み込み
				vector<string> base_string = ReadUntilMatched( ifs, "##Base" );
				for(int i = 1; i < (int)base_string.size(); i++)
				{
					d->base.push_back( atof( base_string[i].c_str() ) );
				}
				
				//データの読み込み
				d->data.resize(this->frameNum);
				for(int i = 0; i < this->frameNum; i++) // foreach PsdData
				{
					getline( ifs, line );
					vector<string> tokens = StringUtil::Tokenize( line, ", " );

					vector<double> one_data;
					one_data.resize( d->dataNum );
					for(int j = 0; j < d->dataNum; j++) 
					{
						one_data[j] = atof( tokens[j].c_str() );
					}
					d->data[i] = one_data;
				}

			}

			else if( "###ENDSECTION" == tokens[0] )
			{
				return;
			}
		}
	}

	// 最初のtokenがqueryとマッチするまでファイルを読んで、
	// マッチしたらtokensを返す
	vector<string> ReadUntilMatched( ifstream& ifs, string query )
	{
		string line;
		while( getline( ifs, line ) )
		{
			vector<string> tokens = StringUtil::Tokenize( line, ", " );
			if( 0 == tokens.size() ) continue;
			if( query == tokens[0] ) return tokens;
		}
		printf("[Load PsdFile] format error.\n");
	}

	//
	// Writeの補助関数
	//


}; //class PsdFile


#endif //_READ_PARAM_CLS_H_