#include "DrawSceneImitateTargetHeading.h"
#include "SceneImitateTargetHeading.h"
#include <iostream>
#include "render/DrawCharacter.h"
// #include "DrawKinTree.h"
#include "render/DrawUtil.h"


cDrawSceneImitateTargetHeading::cDrawSceneImitateTargetHeading()
{
	mDrawKinChar = false;
}

cDrawSceneImitateTargetHeading::~cDrawSceneImitateTargetHeading()
{
}

void cDrawSceneImitateTargetHeading::BuildScene(std::shared_ptr<cSceneSimChar>& out_scene) const
{
	out_scene = std::shared_ptr<cSceneImitateTargetHeading>(new cSceneImitateTargetHeading());
}


void cDrawSceneImitateTargetHeading::DrawCharacters() const
{
	if (mDrawKinChar)
	{
		DrawKinCharacters();
	}
	cDrawSceneSimChar::DrawCharacters();
	cDrawSceneImitateTargetHeading::DrawTargetHeading();
}

void cDrawSceneImitateTargetHeading::DrawKinCharacters() const
{
	const auto& kin_multi_char = GetKinChars();
	for (size_t i=0; i< kin_multi_char.size(); ++i)
	{
		DrawKinCharacter(kin_multi_char[i]);
	}

}

const std::vector<std::shared_ptr<cKinCharacter>>& cDrawSceneImitateTargetHeading::GetKinChars() const
{
	const cSceneImitateTargetHeading* scene = dynamic_cast<const cSceneImitateTargetHeading*>(mScene.get());
	return scene->GetKinChars();
}

void cDrawSceneImitateTargetHeading::DrawTargetHeading() const
{
	const cSceneImitateTargetHeading* scene=  dynamic_cast<const cSceneImitateTargetHeading*>(mScene.get());
	for (int i = 0; i < mScene->GetNumChars(); ++i)
	{
		const auto& character = mScene->GetCharacter(i);
		double arrow_size = 0.2;
		tVector arrow_col = tVector(0, 0.8, 0, 0.5);

		tMatrix origin_trans = character->BuildOriginTrans();
		tMatrix heading_trans = cMathUtil::InvRigidMat(origin_trans);
		tVector root_pos = character->GetRootPos();
		cDrawUtil::SetColor(arrow_col);
		tVector start = tVector(root_pos[0] + 0.5, 0.2, root_pos[2] + 0, 0);
		tVector end = tVector(root_pos[0] + 0.5 + scene->mSceneGoal(0), 0.2, root_pos[2]+ scene->mSceneGoal(1), 0);

		cDrawUtil::PushMatrixView();
		// cDrawUtil::MultMatrixView();
		cDrawUtil::DrawArrow2D(start, end, arrow_size);
		cDrawUtil::PopMatrixView();
	}

}

