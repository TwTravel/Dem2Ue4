#include "FbxAnimationUpdate.h"
#include "FbxCommonTool.h"
//#include "FbxDisplayTool.h"
#include <iostream>
#include <fstream>
#define RADIANTODEGREE(x) x *180.0/3.14159265358979


FbxAnimationUpdate::FbxAnimationUpdate()
{
}


FbxAnimationUpdate::~FbxAnimationUpdate()
{
}

void FbxAnimationUpdate::FbxAnimationUpdateWithPb(const FbxString& fbxFile, const FbxString& AnimationPbFile)
{
	FbxManager* lFbxSdkManager = nullptr;
	FbxScene* lFbxScene = nullptr;

	FbxCommonTool::GetInstance()->InitializeSdkObjects(lFbxSdkManager, lFbxScene);

	bool lResult = true;
	if (fbxFile.IsEmpty() || AnimationPbFile.IsEmpty())
	{
		lResult = false;
		FBXSDK_printf("\n\nUsage: ImportScene <FBX file name>\n\n");
	}
	else
	{
		lResult = FbxCommonTool::GetInstance()->LoadScene(lFbxSdkManager, lFbxScene, fbxFile.Buffer());

		FbxAnimationCover(lFbxScene, AnimationPbFile);

		FbxString OutputFbxFile = fbxFile.Left(fbxFile.GetLen() - 4) + FbxString("_Cover") + FbxString(".fbx");
		lResult = FbxCommonTool::GetInstance()->SaveScene(lFbxSdkManager, lFbxScene, OutputFbxFile);
	}

	FbxCommonTool::GetInstance()->DestroySdkObjects(lFbxSdkManager, lResult);
}

void FbxAnimationUpdate::FbxAnimationCover(FbxScene* lFbxScene, const FbxString& AnimationPbFile)
{
	 if (lFbxScene == nullptr)
		return;

	/*msg::FbxAnimationStack fbxAnimationStackData;
	std::fstream input(AnimationPbFile.Buffer(), std::ios::in | std::ios::binary);
	if (!input) {
		std::cout << AnimationPbFile << ": File not found.  Creating a new file." << std::endl;
	}
	else if (!fbxAnimationStackData.ParseFromIstream(&input)) {
		std::cerr << "Failed to parse pb file" << std::endl;
		return;
	}*/

	//递归遍历存储骨骼点到数组中
	FbxArray<FbxNode *> FbxSkeletonArray;
	FbxCommonTool::GetInstance()->NodeExtractWithDepth(lFbxScene, FbxSkeletonArray);
	printf("array size %i\n", FbxSkeletonArray.Size());

	for (int i = 0; i < lFbxScene->GetSrcObjectCount<FbxAnimStack>(); i++)
	{
		FbxAnimStack* pAnimStack = lFbxScene->GetSrcObject<FbxAnimStack>(i);

		if (pAnimStack->GetMemberCount<FbxAnimLayer>() > 0)
		{
			FbxAnimLayer* pAnimLayer = pAnimStack->GetMember<FbxAnimLayer>(0); //暂时只考虑单层layer的情况，多层混合比较复杂，暂不考虑
		//	const msg::FbxAnimationData& fbxAnimationData = fbxAnimationStackData.animations(0);

		//	FbxAnimationCover(pAnimLayer, FbxSkeletonArray, fbxAnimationData);
			FbxAnimationCoverAA( pAnimLayer, FbxSkeletonArray);
		}
	}
}

FbxAMatrix MatArray[100][100];

void FbxAnimationUpdate::FbxAnimationCoverAA(FbxAnimLayer* pAnimLayer, FbxArray<FbxNode *> &FbxSkeletonArray )
{
	if (pAnimLayer == nullptr || FbxSkeletonArray.Size() == 0)
		return;
	FbxAnimationData fbxAnimationData;
	
	FbxArray<FbxTime> KeyTimes;

	for (int i = 0; i < FbxSkeletonArray.Size(); ++i)
	{
		FbxNode *pFbxNode = FbxSkeletonArray[i];
		FbxAnimCurve* lAnimCurve = pFbxNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
		if (lAnimCurve == NULL)
			continue;
		int lKeyCount = lAnimCurve->KeyGetCount();

		KeyTimes.Clear();
		for (int lCount = 0; lCount < lKeyCount; lCount++)
		{
			FbxTime lKeyTime = lAnimCurve->KeyGetTime(lCount);
			KeyTimes.Add(lKeyTime);
		}

		string node_name = pFbxNode->GetName();
		printf("%02i, %s\n", i, (char*)node_name.c_str());

		vector<FbxAMatrix> transmat;
		transmat.clear();
		for (int lCount = 0; lCount < lKeyCount; lCount++)
		{
			FbxTime lKeyTime = lAnimCurve->KeyGetTime(lCount);
			//KeyTimes.Add(lKeyTime);
			//transmat.push_back(pFbxNode->EvaluateGlobalTransform(lKeyTime));
			MatArray[lCount][i] = pFbxNode->EvaluateLocalTransform(lKeyTime);
		}

		 
	}
	
	//==================================================================================================================
	//##################################################################################################################
	//return;
	
	for (int i = 0; i < FbxSkeletonArray.Size(); ++i)
	{
		FbxNode *pFbxNode = FbxSkeletonArray[i];
		FbxAnimCurve* lAnimCurve = pFbxNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
		if (lAnimCurve == NULL)
			continue;
		int lKeyCount = lAnimCurve->KeyGetCount();

		KeyTimes.Clear();

		for (int lCount = 0; lCount < lKeyCount; lCount++)
		{
			FbxTime lKeyTime = lAnimCurve->KeyGetTime(lCount);
			KeyTimes.Add(lKeyTime);
		}

		string node_name = pFbxNode->GetName();
		printf("%02i, %s\n", i, (char*)node_name.c_str());

 

			{
				FbxAnimCurve* lAnimCurveTX = pFbxNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
				FbxAnimCurve* lAnimCurveTY = pFbxNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
				FbxAnimCurve* lAnimCurveTZ = pFbxNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
				FbxAnimCurve* lAnimCurveRX = pFbxNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
				FbxAnimCurve* lAnimCurveRY = pFbxNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
				FbxAnimCurve* lAnimCurveRZ = pFbxNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);

				lAnimCurveTX->KeyModifyBegin();
				lAnimCurveTY->KeyModifyBegin();
				lAnimCurveTZ->KeyModifyBegin();
				lAnimCurveRX->KeyModifyBegin();
				lAnimCurveRY->KeyModifyBegin();
				lAnimCurveRZ->KeyModifyBegin();

				int lKeyIndexTX = 0;
				int lKeyIndexTY = 0;
				int lKeyIndexTZ = 0;
				int lKeyIndexRX = 0;
				int lKeyIndexRY = 0;
				int lKeyIndexRZ = 0;

				lAnimCurveTX->KeyClear();
				lAnimCurveTY->KeyClear();
				lAnimCurveTZ->KeyClear();
				lAnimCurveRX->KeyClear();
				lAnimCurveRY->KeyClear();
				lAnimCurveRZ->KeyClear();

				for (int lCount = 0; lCount < lKeyCount; lCount++)
				{
					FbxAMatrix  LocalTransMatrix = MatArray[lCount][i]; // fbxSkeletonNode.local_transition_matrix(lCount); //lKeyCount - 1 - 

					lKeyIndexTX = lAnimCurveTX->KeyAdd(KeyTimes[lCount]);
					lAnimCurveTX->KeySet(lKeyIndexTX, KeyTimes[lCount], LocalTransMatrix.GetT()[0]);
					lAnimCurveTX->KeySetInterpolation(lKeyIndexTX, FbxAnimCurveDef::eInterpolationLinear);  

					lKeyIndexTY = lAnimCurveTY->KeyAdd(KeyTimes[lCount]);
					lAnimCurveTY->KeySet(lKeyIndexTY, KeyTimes[lCount], LocalTransMatrix.GetT()[1]);
                    lAnimCurveTY->KeySetInterpolation(lKeyIndexTY, FbxAnimCurveDef::eInterpolationLinear);  

					lKeyIndexTZ = lAnimCurveTZ->KeyAdd(KeyTimes[lCount]);
					lAnimCurveTZ->KeySet(lKeyIndexTZ, KeyTimes[lCount], LocalTransMatrix.GetT()[2]);
					lAnimCurveTZ->KeySetInterpolation(lKeyIndexTZ, FbxAnimCurveDef::eInterpolationLinear);

					lKeyIndexRX = lAnimCurveRX->KeyAdd(KeyTimes[lCount]);
					lAnimCurveRX->KeySet(lKeyIndexRX, KeyTimes[lCount], LocalTransMatrix.GetR()[0]);
                    lAnimCurveRX->KeySetInterpolation(lKeyIndexRX, FbxAnimCurveDef::eInterpolationLinear);  

					lKeyIndexRY = lAnimCurveRY->KeyAdd(KeyTimes[lCount]);
					lAnimCurveRY->KeySet(lKeyIndexRY, KeyTimes[lCount], LocalTransMatrix.GetR()[1]);
                    lAnimCurveRY->KeySetInterpolation(lKeyIndexRY, FbxAnimCurveDef::eInterpolationLinear);

					lKeyIndexRZ = lAnimCurveRZ->KeyAdd(KeyTimes[lCount]);
					lAnimCurveRZ->KeySet(lKeyIndexRZ, KeyTimes[lCount], LocalTransMatrix.GetR()[2]);
					lAnimCurveRZ->KeySetInterpolation(lKeyIndexRZ, FbxAnimCurveDef::eInterpolationLinear);

					printf("哈哈哈 node %i,time %i, rotation %.2lf,  %.2lf,  %.2lf \n", i, lCount,
						LocalTransMatrix.GetR()[0],
						LocalTransMatrix.GetR()[1],
						LocalTransMatrix.GetR()[2]);
				}

				lAnimCurveTX->KeyModifyEnd();
				lAnimCurveTY->KeyModifyEnd();
				lAnimCurveTZ->KeyModifyEnd();
				lAnimCurveRX->KeyModifyEnd();
				lAnimCurveRY->KeyModifyEnd();
				lAnimCurveRZ->KeyModifyEnd();
 
			}
		 
	}
	/**/

	//-----------------------------------------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------------------------------------
	for (int i = 0; i < 28; i++)
	{
		printf("==============================================\n");
		for (int j = 0; j < 24; j++)
		{
			//FbxNode *pFbxNode = FbxSkeletonArray[i];
			//FbxAnimCurve* lAnimCurve = pFbxNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
			//FbxTime lKeyTime = lAnimCurve->KeyGetTime(i);
			printf("%.2lf, %.2lf,%.2lf\n", MatArray[i][j].GetR()[0], MatArray[i][j].GetR()[1], MatArray[i][j].GetR()[2]);	
		}
		printf("==============================================\n");
	}
}
/*void FbxAnimationUpdate::FbxAnimationCover(FbxAnimLayer* pAnimLayer, FbxArray<FbxNode *> FbxSkeletonArray, const msg::FbxAnimationData& fbxAnimationData)
{
	if (pAnimLayer == nullptr || FbxSkeletonArray.Size() == 0)
		return;

	for (int i = 0; i < FbxSkeletonArray.Size(); ++i)
	{
		FbxNode *pFbxNode = FbxSkeletonArray[i];
		FbxAnimCurve* lAnimCurve = pFbxNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
		if (lAnimCurve == NULL)
			continue;
		int lKeyCount = lAnimCurve->KeyGetCount();

		FbxArray<FbxTime> KeyTimes;
		for (int lCount = 0; lCount < lKeyCount; lCount++)
		{
			FbxTime lKeyTime = lAnimCurve->KeyGetTime(lCount);
			KeyTimes.Add(lKeyTime);
		}

		for (int k = 0; k < fbxAnimationData.skeleton_nodes_size(); ++k)
		{
			const msg::FbxSkeletonNode &fbxSkeletonNode = fbxAnimationData.skeleton_nodes(k);
			if (fbxSkeletonNode.name() == std::string(pFbxNode->GetName()))
			{
				FbxAnimCurve* lAnimCurveTX = pFbxNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
				FbxAnimCurve* lAnimCurveTY = pFbxNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
				FbxAnimCurve* lAnimCurveTZ = pFbxNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
				FbxAnimCurve* lAnimCurveRX = pFbxNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
				FbxAnimCurve* lAnimCurveRY = pFbxNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
				FbxAnimCurve* lAnimCurveRZ = pFbxNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);

				lAnimCurveTX->KeyModifyBegin();
				lAnimCurveTY->KeyModifyBegin();
				lAnimCurveTZ->KeyModifyBegin();
				lAnimCurveRX->KeyModifyBegin();
				lAnimCurveRY->KeyModifyBegin();
				lAnimCurveRZ->KeyModifyBegin();

				int lKeyIndexTX = 0;
				int lKeyIndexTY = 0;
				int lKeyIndexTZ = 0;
				int lKeyIndexRX = 0;
				int lKeyIndexRY = 0;
				int lKeyIndexRZ = 0;

				lAnimCurveTX->KeyClear();
				lAnimCurveTY->KeyClear();
				lAnimCurveTZ->KeyClear();
				lAnimCurveRX->KeyClear();
				lAnimCurveRY->KeyClear();
				lAnimCurveRZ->KeyClear();

				for (int lCount = 0; lCount < lKeyCount; lCount++)
				{
					const msg::FbxTransMatrix& LocalTransMatrix = fbxSkeletonNode.local_transition_matrix(lCount);

					lKeyIndexTX = lAnimCurveTX->KeyAdd(KeyTimes[lCount]);
					lAnimCurveTX->KeySet(lKeyIndexTX, KeyTimes[lCount], LocalTransMatrix.translation()[0]);

					lKeyIndexTY = lAnimCurveTY->KeyAdd(KeyTimes[lCount]);
					lAnimCurveTY->KeySet(lKeyIndexTY, KeyTimes[lCount], LocalTransMatrix.translation()[1]);

					lKeyIndexTZ = lAnimCurveTZ->KeyAdd(KeyTimes[lCount]);
					lAnimCurveTZ->KeySet(lKeyIndexTZ, KeyTimes[lCount], LocalTransMatrix.translation()[2]);

					lKeyIndexRX = lAnimCurveRX->KeyAdd(KeyTimes[lCount]);
					lAnimCurveRX->KeySet(lKeyIndexRX, KeyTimes[lCount], RADIANTODEGREE(LocalTransMatrix.rotation()[0]));

					lKeyIndexRY = lAnimCurveRY->KeyAdd(KeyTimes[lCount]);
					lAnimCurveRY->KeySet(lKeyIndexRY, KeyTimes[lCount], RADIANTODEGREE(LocalTransMatrix.rotation()[1]));

					lKeyIndexRZ = lAnimCurveRZ->KeyAdd(KeyTimes[lCount]);
					lAnimCurveRZ->KeySet(lKeyIndexRZ, KeyTimes[lCount], RADIANTODEGREE(LocalTransMatrix.rotation()[2]));
				}

				lAnimCurveTX->KeyModifyEnd();
				lAnimCurveTY->KeyModifyEnd();
				lAnimCurveTZ->KeyModifyEnd();
				lAnimCurveRX->KeyModifyEnd();
				lAnimCurveRY->KeyModifyEnd();
				lAnimCurveRZ->KeyModifyEnd();

				break;
			}
		}
	}
}*/