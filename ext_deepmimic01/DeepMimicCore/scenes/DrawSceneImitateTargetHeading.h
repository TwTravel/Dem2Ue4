#pragma once

#include "scenes/DrawSceneImitate.h"

class cDrawSceneImitateTargetHeading : virtual public cDrawSceneImitate
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	
	cDrawSceneImitateTargetHeading();
	virtual ~cDrawSceneImitateTargetHeading();

	

protected:
	virtual void BuildScene(std::shared_ptr<cSceneSimChar>& out_scene) const;
	virtual void DrawCharacters() const;
	virtual void DrawKinCharacters() const;
	virtual void DrawTargetHeading() const;

	virtual const std::vector<std::shared_ptr<cKinCharacter>>& GetKinChars() const;


};
