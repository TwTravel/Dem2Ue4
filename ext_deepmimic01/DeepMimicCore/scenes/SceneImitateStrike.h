#pragma once

#include "scenes/SceneImitate.h"


class cSceneImitateStrike : virtual public cSceneImitate
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW

	cSceneImitateStrike();
	virtual ~cSceneImitateStrike();
	virtual std::string GetName() const;
	// virtual void Init();


	virtual double CalcReward(int agent_id) const;
	virtual void RecordGoal(int agent_id, Eigen::VectorXd& out_goal) const;
	virtual int GetGoalSize(int agent_id) const;
    virtual void SetGoal(int agent_id, const Eigen::VectorXd& out_goal);
	virtual void BuildGoalOffsetScale(int agent_id, Eigen::VectorXd& out_offset, Eigen::VectorXd& out_scale) const;
	virtual void BuildGoalNormGroups(int agent_id, Eigen::VectorXi& out_groups) const;
	tVector mGoal;

protected:
	virtual void ResetCharacters();
	virtual void UpdateCharacters(double timestep);
	void SetHitState(double h);
	// virtual void UpdateCharacters(double timestep);



};