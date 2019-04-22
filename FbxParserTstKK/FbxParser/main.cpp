#include "ModelReconstruct.h"
#include "FbxCommonTool.h"
#include "FbxAnimationUpdate.h"
#include <iostream> 
using namespace std;

FbxParser *parser;
bool gSupportVBO;

int main(int argc, char **argv)
{
	FbxCommonTool FbxTool;
	FbxManager* lFbxSdkManager = nullptr;
	FbxScene* lFbxScene = nullptr;

	
	FbxTool.InitializeSdkObjects(lFbxSdkManager, lFbxScene);
	FbxTool.LoadScene(lFbxSdkManager, lFbxScene, "d:\\test00\\bb.fbx");
	//FbxTool.LoadScene(lFbxSdkManager, lFbxScene, "c:\\tt\\ZombieResident_Alert_Idle_1.fbx");// "d:\\test00\\bb.fbx");
	//FbxTool.LoadScene(lFbxSdkManager, lFbxScene, "c:\\tt\\aa2.fbx");

	FbxAnimationUpdate fnn;
	//fnn.FbxAnimationCover(lFbxScene, "d:\\test00\\bb.fbx");
	//ZombieResident_Alert_MoveWalk.fbx
	fnn.FbxAnimationCover(lFbxScene, "");
	FbxTool.SaveScene(lFbxSdkManager, lFbxScene, "c:\\tt\\aa2.fbx");

	/*	parser = new FbxParser(FbxString(argv[1]));
	bool loadResult = parser->LoadScene();		//load scene
	if (loadResult) 
	{
		parser->DisplayGlobalLightSettings(&parser->GetFbxScene()->GetGlobalSettings());		//Display global light settings
		parser->DisplayHierarchy(parser->GetFbxScene());		//Display hierarchy of model
		parser->DisplayContent(parser->GetFbxScene());	//Display content
		parser->DisplayPose(parser->GetFbxScene());
	}
	else 
	{
		FBXSDK_printf("error: load scene failed, check if the file exists.\n\n");
		system("pause");
		exit(1);
	}

	if (parser) 
	{
		gSupportVBO = InitOpenGL(argc, argv);	//Initialize the environment of OpenGL
		RunOpenGL();
	}
	system("pause");*/
}