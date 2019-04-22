#include "SceneBuilder.h"

#include <assert.h>

#include "DrawSceneKinChar.h"
#include "DrawSceneImitate.h"
#include "DrawSceneImitateStrike.h"
#include "DrawSceneImitateTargetHeading.h"
#include "SceneImitate.h"
#include "SceneImitateTargetHeading.h"
#include "SceneImitateStrike.h"

void cSceneBuilder::BuildScene(const std::string& scene_name, std::shared_ptr<cScene>& out_scene)
{
	if (scene_name == "")
	{
		printf("No scene specified\n");
		assert(false);
	}
	else if (scene_name == "kin_char")
	{
		out_scene = std::shared_ptr<cSceneKinChar>(new cSceneKinChar());
	}
	else if (scene_name == "imitate")
	{
		out_scene = std::shared_ptr<cSceneImitate>(new cSceneImitate());
	}
	else if (scene_name == "imitate_target_heading")
	{
		out_scene = std::shared_ptr<cSceneImitateTargetHeading>(new cSceneImitateTargetHeading());
	}
	else if (scene_name == "imitate_strike")
	{
		out_scene = std::shared_ptr<cSceneImitateStrike>(new cSceneImitateStrike());

	}
	else
	{
		printf("Unsupported scene: %s\n", scene_name.c_str());
		assert(false);
	}
}

void cSceneBuilder::BuildDrawScene(const std::string& scene_name, std::shared_ptr<cScene>& out_scene)
{
	if (scene_name == "")
	{
		printf("No scene specified\n");
		assert(false);
	}
	else if (scene_name == "kin_char")
	{
		out_scene = std::shared_ptr<cDrawSceneKinChar>(new cDrawSceneKinChar());
	}
	else if (scene_name == "imitate")
	{
		out_scene = std::shared_ptr<cDrawSceneImitate>(new cDrawSceneImitate());
	}
	else if (scene_name == "imitate_target_heading")
	{
		out_scene = std::shared_ptr<cDrawSceneImitateTargetHeading>(new cDrawSceneImitateTargetHeading());
	}
	else if (scene_name == "imitate_strike")
	{
		out_scene = std::shared_ptr<cDrawSceneImitateStrike>(new cDrawSceneImitateStrike());
	}
	else
	{
		printf("Unsupported draw scene: %s\n", scene_name.c_str());
		assert(false);
	}
}