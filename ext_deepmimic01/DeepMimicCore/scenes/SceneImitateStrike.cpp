#include "SceneImitateStrike.h"
#include "SceneImitate.h"
#include <iostream>


cSceneImitateStrike::cSceneImitateStrike()
{
	cSceneImitate();
	mGoal<< 1.0, 0.5, 0.0, 0.0;

}

cSceneImitateStrike::~cSceneImitateStrike()
{
}

std::string cSceneImitateStrike::GetName() const
{
	return "Imitate_Strike";
}


void cSceneImitateStrike::SetHitState(double h)
{
	mGoal[3] = h;
}



double cSceneImitateStrike::CalcReward(int agent_id) const
{

	const cSimCharacter* sim_char = GetAgentChar(agent_id);
	bool fallen = HasFallen(*sim_char);
	const auto& kin_char = GetKinChar();

	tVector euler = tVector(0.0, 0.0, 0.0, 0.0);
	int parent_joint = 5;
	tVector attach_pt = tVector(0.045, -0.0225, 0.0, 0.0);

	double reward = 0.0;
	if (!fallen)
	{
		// assert(sim_char->HasDrawShapes());
		// const auto& shape_defs = sim_char->GetDrawShapeDefs();
		// size_t num_shapes = shape_defs.rows();
		// cKinTree::tDrawShapeDef curr_def = shape_defs.row(5);

		// tVector euler = cKinTree::GetDrawShapeAttachTheta(curr_def);
		// int parent_joint = cKinTree::GetDrawShapeParentJoint(curr_def);
		// tVector attach_pt = cKinTree::GetDrawShapeAttachPt(curr_def);

		tMatrix parent_world_trans = sim_char->BuildJointWorldTrans(parent_joint);
		tMatrix trans_mat = cMathUtil::TranslateMat(attach_pt);
		tMatrix rot_mat = cMathUtil::RotateMat(euler);


		tMatrix right_foot_mat = parent_world_trans * trans_mat * rot_mat;
		tVector right_foot_pos = tVector(right_foot_mat(0,3), right_foot_mat(1,3), right_foot_mat(2,3), 0.0);
		// std::cout <<right_foot_pos<< std::endl;

		tVector target_trans = tVector(mGoal(0), mGoal(1), mGoal(2), 0.0);
		tMatrix target_trans_mat = cMathUtil::TranslateMat(target_trans);
		tMatrix origin_trans0 = kin_char->BuildOriginTrans0();
		tMatrix heading_trans0 = cMathUtil::InvRigidMat(origin_trans0);
		tMatrix target_mat = heading_trans0 * target_trans_mat;
		tVector target_pos = tVector(target_mat(0, 3), target_mat(1,3), target_mat(2,3), 0.0);
		// std::cout << target_pos << std::endl;
		double g_diff = (right_foot_pos - target_pos).norm();

		double g_scale = 4.0;
		double err_scale = 1.0;
		double g_reward = 0.0;
		double hit = (g_diff < 0.2);
		if (hit)
		{
			g_reward = 1.0;
			

		} else
		{
			g_reward = exp(-err_scale * g_scale * g_diff * g_diff);

		}
		

		double i_reward = 0.0;
		i_reward = cSceneImitate::CalcRewardImitate(*sim_char, *kin_char);

		double i_w = 0.7;
		double g_w = 0.3;
		reward = i_w * i_reward + g_w * g_reward;

		// printf("%s\n", "-------==========");
		// printf("%.4f\n", i_reward);
		// printf("%.4f\n", g_reward);
		// printf("%.4f\n", reward);
		// printf("%.4f\n", hit);

	}
	return reward;
}

void cSceneImitateStrike::UpdateCharacters(double timestep)
{
	const cSimCharacter* sim_char = GetAgentChar(0);
	// assert(sim_char->HasDrawShapes());
	// const auto& shape_defs = sim_char->GetDrawShapeDefs();
	// size_t num_shapes = shape_defs.rows();
	// cKinTree::tDrawShapeDef curr_def = shape_defs.row(5);

	// tVector euler = cKinTree::GetDrawShapeAttachTheta(curr_def);
	// int parent_joint = cKinTree::GetDrawShapeParentJoint(curr_def);
	// tVector attach_pt = cKinTree::GetDrawShapeAttachPt(curr_def);


	tVector euler = tVector(0.0, 0.0, 0.0, 0.0);
	int parent_joint = 5;
	tVector attach_pt = tVector(0.045, -0.0225, 0.0, 0.0);

	tMatrix parent_world_trans = sim_char->BuildJointWorldTrans(parent_joint);
	tMatrix trans_mat = cMathUtil::TranslateMat(attach_pt);
	tMatrix rot_mat = cMathUtil::RotateMat(euler);


	tMatrix right_foot_mat = parent_world_trans * trans_mat * rot_mat;
	tVector right_foot_pos = tVector(right_foot_mat(0,3), right_foot_mat(1,3), right_foot_mat(2,3), 0.0);
	// printf("%s\n", "In Update.");
	// std::cout <<right_foot_pos<< std::endl;


	const auto& kin_char = GetKinChar();
	tVector target_trans = tVector(mGoal(0), mGoal(1), mGoal(2), 0.0);
	tMatrix target_trans_mat = cMathUtil::TranslateMat(target_trans);
	tMatrix origin_trans0 = kin_char->BuildOriginTrans0();
	tMatrix heading_trans0 = cMathUtil::InvRigidMat(origin_trans0);
	tMatrix target_mat = heading_trans0 * target_trans_mat;
	tVector target_pos = tVector(target_mat(0, 3), target_mat(1,3), target_mat(2,3), 0.0);
	// std::cout << target_pos << std::endl;
	double g_diff = (right_foot_pos - target_pos).norm();

	double hit = (g_diff < 0.2);
	mGoal[3] = hit;
	cSceneImitate::UpdateCharacters(timestep);

}







void cSceneImitateStrike::RecordGoal(int agent_id, Eigen::VectorXd& out_goal) const
{
	out_goal = mGoal;	
}


int cSceneImitateStrike::GetGoalSize(int agent_id) const
{
	return 4;
}

void cSceneImitateStrike::BuildGoalOffsetScale(int agent_id, Eigen::VectorXd& out_offset, Eigen::VectorXd& out_scale) const
{
	int goal_size = GetGoalSize(agent_id);
	out_offset = Eigen::VectorXd::Zero(goal_size);
	out_scale = Eigen::VectorXd::Ones(goal_size);
}

void cSceneImitateStrike::BuildGoalNormGroups(int agent_id, Eigen::VectorXi& out_groups) const
{
	int goal_size = GetGoalSize(agent_id);
	int gNormGroupSingle = 0;
	out_groups = gNormGroupSingle * Eigen::VectorXi::Ones(goal_size);
}


void cSceneImitateStrike::SetGoal(int agent_id, const Eigen::VectorXd& out_goal)
{
	printf("%s\n", " In the Strike set goal.");
	int goal_size = GetGoalSize(agent_id);
	if(out_goal.size() + 1 == goal_size)
	{
		mGoal[0] = out_goal[0];
		mGoal[1] = out_goal[1];
		mGoal[2] = out_goal[2]; 
		mGoal[3] = 0.0;
	}
	else
	{
		printf("Wrong goal size.\n");
		assert(false);
	}
	std::cout << mGoal << std::endl;
}

void cSceneImitateStrike::ResetCharacters()
{
	cSceneImitate::ResetCharacters();
	mGoal[3] = 0.0;
}









