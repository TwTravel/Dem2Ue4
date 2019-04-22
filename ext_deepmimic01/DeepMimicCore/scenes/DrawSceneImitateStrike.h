#pragma once

#include "scenes/DrawSceneImitate.h"

class cDrawSceneImitateStrike : virtual public cDrawSceneImitate
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	
	cDrawSceneImitateStrike();
	virtual ~cDrawSceneImitateStrike();

	

protected:
	virtual void BuildScene(std::shared_ptr<cSceneSimChar>& out_scene) const;
	virtual void DrawCharacters() const;
	virtual void DrawKinCharacters() const;
	virtual void DrawTargetPosition() const;



};
