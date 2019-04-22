#pragma once

#include "scenes/SceneImitate.h"


class cSceneImitateTargetHeading : virtual public cSceneImitate
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW

	cSceneImitateTargetHeading();
	virtual ~cSceneImitateTargetHeading();
	virtual std::string GetName() const;
	virtual void ParseArgs(const std::shared_ptr<cArgParser>& parser);
	virtual void Init();

	virtual const std::shared_ptr<cKinCharacter>& GetKinChar() const;
	virtual const std::vector<std::shared_ptr<cKinCharacter>>& GetKinChars() const;

	virtual double CalcReward(int agent_id) const;
	virtual void RecordGoal(int agent_id, Eigen::VectorXd& out_goal) const;
	virtual int GetGoalSize(int agent_id) const;
    virtual void SetGoal(int agent_id, const Eigen::VectorXd& out_goal);
	virtual void BuildGoalOffsetScale(int agent_id, Eigen::VectorXd& out_offset, Eigen::VectorXd& out_scale) const;
	virtual void BuildGoalNormGroups(int agent_id, Eigen::VectorXi& out_groups) const;
	Eigen::Vector2d mSceneGoal;



protected:

	std::vector<std::string> mMotionFiles;
	std::vector<std::shared_ptr<cKinCharacter>> mKinMultiChar;
	int count = 0;

	Eigen::VectorXd mJointWeights;

	bool mEnableRandRotReset;
	bool mSyncCharRootPos;
	bool mSyncCharRootRot;
	bool mEnableRootRotFail;
	double mHoldEndFrame;


	virtual void BuildKinChars();
	virtual bool BuildKinCharacters(std::vector<std::shared_ptr<cKinCharacter>>& out_char) const;
	virtual void UpdateCharacters(double timestep);
	virtual void UpdateKinMultiChar(double timestep);

	virtual void ResetCharacters();
	virtual void ResetKinMultiChar();
	virtual void SyncKinCharRoot();

	virtual double CalcRewardImitateTargetHeading(const cSimCharacter& sim_char, const std::vector<std::shared_ptr<cKinCharacter>>& ref_chars) const;







};