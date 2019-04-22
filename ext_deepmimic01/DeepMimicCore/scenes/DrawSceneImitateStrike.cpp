#include "DrawSceneImitateStrike.h"
#include "SceneImitateStrike.h"
#include <iostream>
#include "render/DrawCharacter.h"
// #include "DrawKinTree.h"
#include "render/DrawUtil.h"


cDrawSceneImitateStrike::cDrawSceneImitateStrike()
{
	mDrawKinChar = false;
}

cDrawSceneImitateStrike::~cDrawSceneImitateStrike()
{
}

void cDrawSceneImitateStrike::BuildScene(std::shared_ptr<cSceneSimChar>& out_scene) const
{
	out_scene = std::shared_ptr<cSceneImitateStrike>(new cSceneImitateStrike());
}


void cDrawSceneImitateStrike::DrawCharacters() const
{
	if (mDrawKinChar)
	{
		DrawKinCharacters();
	}
	cDrawSceneSimChar::DrawCharacters();
	// cDrawSceneSimChar::DrawHeading();
	cDrawSceneImitateStrike::DrawTargetPosition();
}

void cDrawSceneImitateStrike::DrawKinCharacters() const
{
	const auto& kin_char = GetKinChar();
	DrawKinCharacter(kin_char);

}

void cDrawSceneImitateStrike::DrawTargetPosition() const
{
	const cSceneImitateStrike* scene=  dynamic_cast<const cSceneImitateStrike*>(mScene.get());

	
	const auto& kin_char = scene->GetKinChar();
	double sphere_r = 0.2;
	tVector sphere_col = tVector(0, 0.8, 0, 0.5);
	tVector trans = tVector(scene->mGoal(0), scene->mGoal(1), scene->mGoal(2), 0.0);


	tMatrix origin_trans0 = kin_char->BuildOriginTrans0();
	tMatrix heading_trans0 = cMathUtil::InvRigidMat(origin_trans0);


	cDrawUtil::SetColor(sphere_col);
	cDrawUtil::PushMatrixView();
	cDrawUtil::MultMatrixView(heading_trans0);
	cDrawUtil::Translate(trans);
	cDrawUtil::DrawSphere(sphere_r);
	cDrawUtil::PopMatrixView();

	// sphere_col = tVector(0.8, 0., 0, 0.5);
	// cDrawUtil::SetColor(sphere_col);
	// cDrawUtil::PushMatrixView();
	// // cDrawUtil::MultMatrixView(heading_trans0);
	// cDrawUtil::Translate(trans);
	// cDrawUtil::DrawSphere(sphere_r);
	// cDrawUtil::PopMatrixView();


	// printf("%s\n", "*********");
	// tMatrix trans_mat = cMathUtil::TranslateMat(trans);
	// std::cout << origin_trans0 << std::endl;

	// std::cout << heading_trans0 * trans_mat << std::endl;

	// std::cout << trans_mat * origin_trans0 << std::endl;

}

