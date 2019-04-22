import json
import pprint
import numpy as np
with open('humanoid3d_ext_input.txt') as f:
	data = json.load(f)

joint_param = dict( (joint["Name"], (joint["AttachX"], joint["AttachY"], joint["AttachZ"])) for joint in data["Skeleton"]["Joints"])

body_param = dict( (body["Name"],body["Mass"] ) for body in data["BodyDefs"])

shape_param = dict( (shape["Name"], ((shape["AttachX"], shape["AttachY"], shape["AttachZ"]), (shape["Param0"], shape["Param1"], shape["Param2"]))) for shape in data["DrawShapeDefs"] if "joint" not in shape["Name"])

# pprint.pprint(joint_param)
# pprint.pprint(body_param)
# pprint.pprint(shape_param)
print(body_param)




jChestHeight = 0.236151
jNeckHeight = 0.35
jClavicleHeight = 0.2935
jClavicleOffSetX = -0.02405
jClavicleOffsetZ = 0.05
jClavicleLength =  0.13311
jUpperArmLength = 0.274788
jForeArmLength = 0.258947
jHipOffsetZ = 0.084887
jUpperLegLength = 0.421546
jLowerLegLength = 0.40987


ChestMass = 14.0
AnkleMass = 1.0
ClavicleMass = 1.0
ElbowMass = 1.0
HipMass = 4.5
KneeMass = 3.0
ShoulderMass = 1.5
WristMass = 0.5
NeckMass = 2.0
RootMass = 6.0

MassList = [('root', RootMass), ("chest", ChestMass), ("neck", NeckMass),
("right_hip", HipMass), ("right_knee", KneeMass), ("right_ankle", AnkleMass),
("right_clavicle", ClavicleMass), ("right_shoulder", ShoulderMass), ("right_elbow", ElbowMass), ("right_wrist", WristMass),
("left_hip", HipMass), ("left_knee", KneeMass), ("left_ankle", AnkleMass),
("left_clavicle", ClavicleMass), ("left_shoulder", ShoulderMass), ("left_elbow", ElbowMass), ("left_wrist", WristMass)]







SkeletonJoints = data["Skeleton"]["Joints"]
SkeletonJoints[1]["AttachY"] = jChestHeight  
SkeletonJoints[2]["AttachY"] = jNeckHeight
SkeletonJoints[3]["AttachZ"] = jHipOffsetZ
SkeletonJoints[4]["AttachY"] = -jUpperLegLength 
SkeletonJoints[5]["AttachY"] = -jLowerLegLength
SkeletonJoints[6]["AttachX"], SkeletonJoints[6]["AttachY"], SkeletonJoints[6]["AttachZ"] = jClavicleOffSetX, jClavicleHeight, jClavicleOffsetZ
SkeletonJoints[7]["AttachZ"] = jClavicleLength
SkeletonJoints[8]["AttachY"] = -jUpperArmLength
SkeletonJoints[9]["AttachY"] = -jForeArmLength

SkeletonJoints[10]["AttachZ"] = -jHipOffsetZ
SkeletonJoints[11]["AttachY"] = -jUpperLegLength 
SkeletonJoints[12]["AttachY"] = -jLowerLegLength
SkeletonJoints[13]["AttachX"], SkeletonJoints[13]["AttachY"], SkeletonJoints[13]["AttachZ"] = jClavicleOffSetX, jClavicleHeight, -jClavicleOffsetZ
SkeletonJoints[14]["AttachZ"] = -jClavicleLength
SkeletonJoints[15]["AttachY"] = -jUpperArmLength
SkeletonJoints[16]["AttachY"] = -jForeArmLength




bNeckOffset = 0.12
bChestRadius = np.floor((jNeckHeight - bNeckOffset) * 100 / 2.0) / 100
bChestHeight = jNeckHeight - bNeckOffset - bChestRadius 
bRootHeight = jChestHeight / 3.0
bRootRadius = np.sqrt(jHipOffsetZ **2 + bRootHeight ** 2) - 0.02



bWaistRadius = (jChestHeight + bChestHeight  + bRootHeight) / 2.0




bClavicleRadius = 0.025
bUpperLegRadius = 0.055
bLowerLegRadius = 0.05
bUpperArmRadius = 0.045
bForeArmRadius = 0.04



Bodies = data["BodyDefs"]
Shapes = data["DrawShapeDefs"]


for idx, entry in enumerate(MassList):	
		Bodies[idx]["Mass"] = entry[1]


Bodies[0]["AttachY"], Bodies[0]["Param0"], Bodies[0]["Param1"], Bodies[0]["Param2"]  = bRootHeight, 2 * bRootRadius, 2 * bRootRadius, 2 * bRootRadius
Shapes[0]["AttachY"], Shapes[0]["Param0"], Shapes[0]["Param1"], Shapes[0]["Param2"]  = bRootHeight, 2 * bRootRadius, 2 * bRootRadius, 2 * bRootRadius

Bodies[1]["AttachY"], Bodies[1]["Param0"], Bodies[1]["Param1"], Bodies[1]["Param2"]  = bChestHeight, 2 * bChestRadius, 2 * bChestRadius, 2 * bChestRadius
Shapes[1]["AttachY"], Shapes[1]["Param0"], Shapes[1]["Param1"], Shapes[1]["Param2"]  = bChestHeight, 2 * bChestRadius, 2 * bChestRadius, 2 * bChestRadius



bUpperLegLengthParam = np.floor((jUpperLegLength - 2 * bUpperLegRadius) * 100) / 100 - 0.03
Bodies[3]["AttachY"], Bodies[3]["Param0"], Bodies[3]["Param1"], Bodies[3]["Param2"]  = -jUpperLegLength / 2.0, 2 * bUpperLegRadius, bUpperLegLengthParam, 2 * bUpperLegRadius
Shapes[3]["AttachY"], Shapes[3]["Param0"], Shapes[3]["Param1"], Shapes[3]["Param2"]  = -jUpperLegLength / 2.0, 2 * bUpperLegRadius, bUpperLegLengthParam, 2 * bUpperLegRadius
Bodies[10]["AttachY"], Bodies[10]["Param0"], Bodies[10]["Param1"], Bodies[10]["Param2"]  = -jUpperLegLength / 2.0, 2 * bUpperLegRadius, bUpperLegLengthParam, 2 * bUpperLegRadius
Shapes[10]["AttachY"], Shapes[10]["Param0"], Shapes[10]["Param1"], Shapes[10]["Param2"]  = -jUpperLegLength / 2.0, 2 * bUpperLegRadius, bUpperLegLengthParam, 2 * bUpperLegRadius

bLowerLegLengthParam = np.floor((jLowerLegLength - 2 * bLowerLegRadius) * 100 ) / 100 - 0.03
Bodies[4]["AttachY"], Bodies[4]["Param0"], Bodies[4]["Param1"], Bodies[4]["Param2"]  = -jLowerLegLength / 2.0, 2 * bLowerLegRadius, bLowerLegLengthParam, 2 * bLowerLegRadius
Shapes[4]["AttachY"], Shapes[4]["Param0"], Shapes[4]["Param1"], Shapes[4]["Param2"]  = -jLowerLegLength / 2.0, 2 * bLowerLegRadius, bLowerLegLengthParam, 2 * bLowerLegRadius
Bodies[11]["AttachY"], Bodies[11]["Param0"], Bodies[11]["Param1"], Bodies[11]["Param2"]  = -jLowerLegLength / 2.0, 2 * bLowerLegRadius, bLowerLegLengthParam, 2 * bLowerLegRadius
Shapes[11]["AttachY"], Shapes[11]["Param0"], Shapes[11]["Param1"], Shapes[11]["Param2"]  = -jLowerLegLength / 2.0, 2 * bLowerLegRadius, bLowerLegLengthParam, 2 * bLowerLegRadius

bClavicleLengthParam = np.floor((jClavicleLength - 2 * bClavicleRadius) * 100) / 100 - 0.03
Bodies[6]["AttachX"], Bodies[6]["AttachZ"], Bodies[6]["Param0"], Bodies[6]["Param1"], Bodies[6]["Param2"]  = -0.01, jClavicleLength / 2.0, 2 * bClavicleRadius, bClavicleLengthParam, 2 * bClavicleRadius
Shapes[6]["AttachX"], Shapes[6]["AttachZ"], Shapes[6]["Param0"], Shapes[6]["Param1"], Shapes[6]["Param2"]  = -0.01, jClavicleLength / 2.0, 2 * bClavicleRadius, bClavicleLengthParam, 2 * bClavicleRadius
Bodies[13]["AttachX"], Bodies[13]["AttachZ"], Bodies[13]["Param0"], Bodies[13]["Param1"], Bodies[13]["Param2"]  = -0.01, -jClavicleLength / 2.0, 2 * bClavicleRadius, bClavicleLengthParam, 2 * bClavicleRadius
Shapes[13]["AttachX"], Shapes[13]["AttachZ"], Shapes[13]["Param0"], Shapes[13]["Param1"], Shapes[13]["Param2"]  = -0.01, -jClavicleLength / 2.0, 2 * bClavicleRadius, bClavicleLengthParam, 2 * bClavicleRadius


bUpperArmLengthParam = np.floor((jUpperArmLength - 2 * bUpperArmRadius) * 100) / 100 - 0.03
Bodies[7]["AttachY"], Bodies[7]["Param0"], Bodies[7]["Param1"], Bodies[7]["Param2"]  = -jUpperArmLength / 2.0, 2 * bUpperArmRadius, bUpperArmLengthParam, 2 * bUpperArmRadius
Shapes[7]["AttachY"], Shapes[7]["Param0"], Shapes[7]["Param1"], Shapes[7]["Param2"]  = -jUpperArmLength / 2.0, 2 * bUpperArmRadius, bUpperArmLengthParam, 2 * bUpperArmRadius
Bodies[14]["AttachY"], Bodies[14]["Param0"], Bodies[14]["Param1"], Bodies[14]["Param2"]  = -jUpperArmLength / 2.0, 2 * bUpperArmRadius, bUpperArmLengthParam, 2 * bUpperArmRadius
Shapes[14]["AttachY"], Shapes[14]["Param0"], Shapes[14]["Param1"], Shapes[14]["Param2"]  = -jUpperArmLength / 2.0, 2 * bUpperArmRadius, bUpperArmLengthParam, 2 * bUpperArmRadius

bForeArmLengthParam = np.floor((jForeArmLength - 2 * bForeArmRadius) * 100 ) / 100 - 0.03
Bodies[8]["AttachY"], Bodies[8]["Param0"], Bodies[8]["Param1"], Bodies[8]["Param2"]  = -jForeArmLength / 2.0, 2 * bForeArmRadius, bForeArmLengthParam, 2 * bForeArmRadius
Shapes[8]["AttachY"], Shapes[8]["Param0"], Shapes[8]["Param1"], Shapes[8]["Param2"]  = -jForeArmLength / 2.0, 2 * bForeArmRadius, bForeArmLengthParam, 2 * bForeArmRadius
Bodies[15]["AttachY"], Bodies[15]["Param0"], Bodies[15]["Param1"], Bodies[15]["Param2"]  = -jForeArmLength / 2.0, 2 * bForeArmRadius, bForeArmLengthParam, 2 * bForeArmRadius
Shapes[15]["AttachY"], Shapes[15]["Param0"], Shapes[15]["Param1"], Shapes[15]["Param2"]  = -jForeArmLength / 2.0, 2 * bForeArmRadius, bForeArmLengthParam, 2 * bForeArmRadius











with open('humanoid3d_ext.txt', 'w') as json_file:
  json.dump(data, json_file)

