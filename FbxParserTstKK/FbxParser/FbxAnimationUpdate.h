#pragma once
#include <fbxsdk.h>
#include <vector>
using namespace std;
//#include "FbxAnimationData.pb.h"
class FbxTransMatrix
{
public:
 // FbxVector3* translation_;
 // FbxVector3* rotation_;
//	::msg::FbxVector3* scale_;
//	::msg::FbxVector4* quaternion_;
vector<double> & translation(){ return translation_; };
vector<double> & rotation(){ return rotation_; };
vector<double> translation_;
vector<double> rotation_;
vector<double> scale_;
vector<double> quaternion_;
};

class FbxSkeletonNode
{
public:
 vector<FbxTransMatrix>	 local_transition_matrix_;
 vector<FbxTransMatrix>  global_transition_matrix_;
 FbxTransMatrix& local_transition_matrix(int k){ return local_transition_matrix_[k]; };
 string name(){ return  _nodename; };
 string _nodename;
};


class FbxSkeletonNodeData
{
public:
 vector <FbxSkeletonNode>skeleton_nodes_;
};

class FbxAnimationData
{
public:
	vector<double >frametimes_;
	vector<FbxSkeletonNode > skeleton_nodes_;
	vector<string> name_;
	vector<int> frames_num_;
	int skeleton_nodes_size(){ return skeleton_nodes_.size(); };

	FbxSkeletonNode & skeleton_nodes(int k){ return skeleton_nodes_[k]; };
};

class FbxAnimationUpdate
{
public:
	FbxAnimationUpdate();
	~FbxAnimationUpdate();

	void FbxAnimationUpdateWithPb(const FbxString& fbxFile, const FbxString& AnimationPbFile);
//protected:
	void FbxAnimationCover(FbxScene* lFbxScene, const FbxString& AnimationPbFile);
	void FbxAnimationCoverAA(FbxAnimLayer* pAnimLayer, FbxArray<FbxNode *> &FbxSkeletonArray);
//	void FbxAnimationCover(FbxAnimLayer* pAnimLayer, FbxArray<FbxNode *> FbxSkeletonArray, const msg::FbxAnimationData& fbxAnimationData);
};

