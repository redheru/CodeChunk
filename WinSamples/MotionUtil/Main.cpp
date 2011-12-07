#include "MotionUtil.h"
#include "MotionConcatenation.h"

using namespace Motion;


int main()
{
	//テスト用
	string fileName1 = "C:/lib/data/walk/69/69_01.bvh";
	string fileName2 = "C:/lib/data/walk/69/69_01.bvh";
	DrawMocapData mocap1(fileName1);
	DrawMocapData mocap2(fileName2);
	DrawMocapData dst;

	PoseSimilarity min;
	int index = 0;
	for(int i=0; i< mocap2.getFrameNum(); i++)
	{
		PoseSimilarity tmp = MotionUtil::getSimilarity(mocap1, mocap2, 74 , i);
		if(min > tmp){	
			min = tmp;
			index = i;
		}
		cout << i << " : "<<tmp.poseDiff << ", " << tmp.rootDiff << ", " << tmp.velDiff << endl;
	}

	cout << index << " : " << min.poseDiff << ", " << min.rootDiff << ", " << min.velDiff << endl;

	//MotionConcatenation::Concatenation2(dst, mocap1, mocap2, 50, 119);
	//dst.WriteBvh("C:/lib/data/walk/69/69_01_edited.bvh");

	return 0;
}