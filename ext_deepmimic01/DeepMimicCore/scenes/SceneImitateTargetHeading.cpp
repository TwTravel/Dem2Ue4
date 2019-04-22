#include "SceneImitateTargetHeading.h"
#include "SceneImitate.h"
#include <iostream>


cSceneImitateTargetHeading::cSceneImitateTargetHeading()
{
	mEnableRandRotReset = false;
	mSyncCharRootPos = true;
	mSyncCharRootRot = false;
	mEnableRootRotFail = false;
	mHoldEndFrame = 0;
	mSceneGoal<< 0.5, 0.5;

}

cSceneImitateTargetHeading::~cSceneImitateTargetHeading()
{
}

std::string cSceneImitateTargetHeading::GetName() const
{
	return "Imitate_Target_Heading";
}


const std::shared_ptr<cKinCharacter>& cSceneImitateTargetHeading::GetKinChar() const
{
	return mKinChar;
}

const std::vector<std::shared_ptr<cKinCharacter>>& cSceneImitateTargetHeading::GetKinChars() const
{
	return mKinMultiChar;
}


void cSceneImitateTargetHeading::ParseArgs(const std::shared_ptr<cArgParser>& parser)
{
	cRLSceneSimChar::ParseArgs(parser);
	parser->ParseBool("enable_rand_rot_reset", mEnableRandRotReset);
	parser->ParseBool("sync_char_root_pos", mSyncCharRootPos);
	parser->ParseBool("sync_char_root_rot", mSyncCharRootRot);
	parser->ParseBool("enable_root_rot_fail", mEnableRootRotFail);
	parser->ParseDouble("hold_end_frame", mHoldEndFrame);
	parser->ParseStrings("motion_files", mMotionFiles);

	for(size_t i =0; i<mMotionFiles.size(); ++i)
	{
		std::cout << mMotionFiles[i] << std::endl;
	}

}


void cSceneImitateTargetHeading::Init()
{
	for(size_t i=0; i<mKinMultiChar.size(); ++i)
	{
		mKinMultiChar[i].reset();

	}
	mKinChar.reset();





	BuildKinChars();
	int i = mRand.RandInt(0, mKinMultiChar.size());
	// printf("%s with the motion %i. \n", "Init all characters", i);
	mKinChar = mKinMultiChar[i];
	cRLSceneSimChar::Init();
	InitJointWeights();
}


double cSceneImitateTargetHeading::CalcReward(int agent_id) const
{

	const cSimCharacter* sim_char = GetAgentChar(agent_id);
	bool fallen = HasFallen(*sim_char);

	double r = 0;
	int max_id = 0;
	if (!fallen)
	{
		r = CalcRewardImitateTargetHeading(*sim_char, mKinMultiChar);
	}
	return r;
}


double cSceneImitateTargetHeading::CalcRewardImitateTargetHeading(const cSimCharacter& sim_char, const std::vector<std::shared_ptr<cKinCharacter>>& ref_chars) const
{	

	double i_reward = 0;
	double reward = 0;
	double temp_r = 0;
	for (size_t i = 0; i<mKinMultiChar.size(); ++i)
	{
        temp_r = cSceneImitate::CalcRewardImitate(sim_char, *mKinMultiChar[i]);
		i_reward = std::max(i_reward, temp_r);
		// printf("%s %lu's reward is %.4f\n", "Motion", i, temp_r);
	}

	double g_scale = 2.5;
	double limit_vel = 1.0;
	double err_scale = 1.0;

	// std::cout << "mSceneGoal\n" << mSceneGoal << std::endl; 

	tVector dir(mSceneGoal(0), 0.0, mSceneGoal(1), 0.0);

	tVector unit_dir = dir / dir.norm();


	tVector com_vel0_world = sim_char.CalcCOMVel();
	double g_diff = std::max(0.0, limit_vel - com_vel0_world.dot(unit_dir));
	double g_err = g_diff * g_diff;
	double g_reward = exp(-err_scale * g_scale * g_err);

	double i_w = 0.7;
	double g_w = 0.3;
	reward = i_w * i_reward + g_w * g_reward;

	return reward;
} 


void cSceneImitateTargetHeading::BuildKinChars()
{

	bool succ = BuildKinCharacters(mKinMultiChar);
	if (!succ)
	{
		printf("Failed to build kin characters\n");
		assert(false);
	}
}

bool cSceneImitateTargetHeading::BuildKinCharacters(std::vector<std::shared_ptr<cKinCharacter>>& out_multi_char) const
{
	bool succ = false;
	for (size_t i = 0; i < mMotionFiles.size(); ++i)
	{
		auto kin_char = std::shared_ptr<cKinCharacter>(new cKinCharacter());
		const cSimCharacter::tParams& sim_char_params = mCharParams[0];
		cKinCharacter::tParams kin_char_params;

		kin_char_params.mID = i;
		kin_char_params.mCharFile = sim_char_params.mCharFile;
		kin_char_params.mOrigin = sim_char_params.mInitPos;
		kin_char_params.mLoadDrawShapes = false;
		kin_char_params.mMotionFile = mMotionFiles[i];

		succ = kin_char->Init(kin_char_params);

		if (succ)
		{
			out_multi_char.push_back(kin_char);
			printf("Motion %i 's duration is %.5f\n", i, kin_char->GetMotionDuration() );
		}
		else
		{
			return false;
		}

	}

	
	return succ;
}


void cSceneImitateTargetHeading::UpdateCharacters(double timestep)
{
	count++;
	// printf("Updating: %i, %.8f, \n", int(count / 20), timestep * 20);
	UpdateKinMultiChar(timestep);
	cRLSceneSimChar::UpdateCharacters(timestep);
}

void cSceneImitateTargetHeading::UpdateKinMultiChar(double timestep)
{

	for (size_t i=0; i<mKinMultiChar.size(); ++i)
	{
		const auto& kin_char = mKinMultiChar[i];
		double prev_phase = kin_char->GetPhase();
		kin_char->Update(timestep);
		double curr_phase = kin_char->GetPhase();

		if (curr_phase < prev_phase)
		{
			const auto& sim_char = GetCharacter();
			SyncKinCharNewCycle(*sim_char, *kin_char);
		}

	}

}

void cSceneImitateTargetHeading::ResetCharacters()
{
	int i = mRand.RandInt(0, mKinMultiChar.size());
	// printf("%lu\n", mKinMultiChar.size());
	// printf("%s with the motion %i. \n", "Reset all characters", i);
	mKinChar.reset();
	mKinChar = mKinMultiChar[i];
	cRLSceneSimChar::ResetCharacters();

	ResetKinMultiChar();
	if (EnableSyncChar())
	{
		SyncCharacters();
	}
}

void cSceneImitateTargetHeading::ResetKinMultiChar()
{
	double rand_time = CalcRandKinResetTime();

	const cSimCharacter::tParams& char_params = mCharParams[0];
	double rand_theta = mRand.RandDouble(-M_PI, M_PI);
	// const auto& kin_char = GetKinChar();

	for(size_t i =0; i<mKinMultiChar.size(); ++i)
	{
		const auto& kin_char = mKinMultiChar[i];
		kin_char->Reset();
		kin_char->SetOriginRot(tQuaternion::Identity());
		kin_char->SetOriginPos(char_params.mInitPos); // reset origin
		kin_char->SetTime(rand_time);
		kin_char->Pose(rand_time);

		if (EnabledRandRotReset())
		{
			
			kin_char->RotateOrigin(cMathUtil::EulerToQuaternion(tVector(0, rand_theta, 0, 0)));
		}

	}

	
}

// void cSceneImitateTargetHeading::RecordGoal(int agent_id, Eigen::VectorXd& out_goal) const
// {
// 	const auto& ctrl = GetController(agent_id);
// 	ctrl->RecordGoal(out_goal);
// }

// int cSceneImitateTargetHeading::GetGoalSize(int agent_id) const
// {
// 	const auto& ctrl = GetController(agent_id);
// 	return ctrl->GetGoalSize();
// }

// void cSceneImitateTargetHeading::BuildGoalOffsetScale(int agent_id, Eigen::VectorXd& out_offset, Eigen::VectorXd& out_scale) const
// {
// 	const auto& ctrl = GetController(agent_id);
// 	ctrl->BuildGoalOffsetScale(out_offset, out_scale);
// }

// void cSceneImitateTargetHeading::BuildGoalNormGroups(int agent_id, Eigen::VectorXi& out_groups) const
// {
// 	const auto& ctrl = GetController(agent_id);
// 	ctrl->BuildGoalNormGroups(out_groups);
// }


void cSceneImitateTargetHeading::SyncKinCharRoot()
{
	const auto& sim_char = GetCharacter();
	tVector sim_root_pos = sim_char->GetRootPos();
	double sim_heading = sim_char->CalcHeading();

	for (size_t i=0; i<mKinMultiChar.size(); ++i)
	{

		const auto& kin_char = mKinMultiChar[i];
		double kin_heading = kin_char->CalcHeading();

		tQuaternion drot = tQuaternion::Identity();
		if (mSyncCharRootRot)
		{
			drot = cMathUtil::AxisAngleToQuaternion(tVector(0, 1, 0, 0), sim_heading - kin_heading);
		}

		kin_char->RotateRoot(drot);
		kin_char->SetRootPos(sim_root_pos);

	}
	
}












void cSceneImitateTargetHeading::RecordGoal(int agent_id, Eigen::VectorXd& out_goal) const
{
	out_goal = mSceneGoal;	
}


int cSceneImitateTargetHeading::GetGoalSize(int agent_id) const
{
	return 2;
}

void cSceneImitateTargetHeading::BuildGoalOffsetScale(int agent_id, Eigen::VectorXd& out_offset, Eigen::VectorXd& out_scale) const
{
	int goal_size = GetGoalSize(agent_id);
	out_offset = Eigen::VectorXd::Zero(goal_size);
	out_scale = Eigen::VectorXd::Ones(goal_size);
}

void cSceneImitateTargetHeading::BuildGoalNormGroups(int agent_id, Eigen::VectorXi& out_groups) const
{
	int goal_size = GetGoalSize(agent_id);
	int gNormGroupSingle = 0;
	out_groups = gNormGroupSingle * Eigen::VectorXi::Ones(goal_size);
}


void cSceneImitateTargetHeading::SetGoal(int agent_id, const Eigen::VectorXd& out_goal)
{
	printf("%s\n", " In the Target Heading set goal.");
	int goal_size = GetGoalSize(agent_id);
	if(out_goal.size() == goal_size)
	{
		mSceneGoal = out_goal / out_goal.norm();
	}
	else
	{
		printf("Wrong goal size.\n");
		assert(false);
	}
}







