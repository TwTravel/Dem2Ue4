#include "DrawGround.h"
#include "DrawUtil.h"
#include "sim/GroundPlane.h"
#include "sim/GroundVar3D.h"

void cDrawGround::BuildMesh(const cGround* ground, cDrawMesh* out_mesh)
{
	cGround::eClass ground_class = ground->GetGroundClass();
	switch (ground_class)
	{
	case cGround::eClassPlane:
		BuildMeshPlane(ground, out_mesh);
		break;
	case cGround::eClassVar3D:
	case cGround::eClassHills3D:
	case cGround::eClassTrail3D:
	case cGround::eClassObstacles3D:
		BuildMeshVar3D(ground, out_mesh);
		break;
	case cGround::eClassDynamicObstacles3D:
	//case cGround::eClassConveyor3D:
		BuildMeshDynamicObstacles3D(ground, out_mesh);
		break;
	default:
		assert(false); // unsupported ground type
		break;
	}
}

void cDrawGround::BuildMeshPlane(const cGround* ground, cDrawMesh* out_mesh)
{
	const cGroundPlane* ground_plane = reinterpret_cast<const cGroundPlane*>(ground);
	const tVector& center = ground_plane->GetPrevCenter();

	const tVector tex_size = tVector(0.5, 0.5, 0, 0);
	const int num_faces = 1;
	const int verts_per_face = 6;
	const int vert_size = 3;
	const int norm_size = 3;
	const int coord_size = 2;
	const double w = 200;

	tVector size = tVector(w, 0, w, 0);
	tVector ground_origin = ground->GetPos();

	tVector a = tVector(center[0] - 0.5 * size[0], ground_origin[1], center[2] - 0.5 * size[2], 0);
	tVector b = tVector(center[0] - 0.5 * size[0], ground_origin[1], center[2] + 0.5 * size[2], 0);
	tVector c = tVector(center[0] + 0.5 * size[0], ground_origin[1], center[2] + 0.5 * size[2], 0);
	tVector d = tVector(center[0] + 0.5 * size[0], ground_origin[1], center[2] - 0.5 * size[2], 0);

	tVector min_coord = a - ground_origin;
	tVector max_coord = c - ground_origin;
	min_coord[0] /= tex_size[0];
	min_coord[1] = min_coord[2] / tex_size[1];
	max_coord[0] /= tex_size[0];
	max_coord[1] = max_coord[2] / tex_size[1];

	tVector coord_a = tVector(min_coord[0], min_coord[1], 0, 0);
	tVector coord_b = tVector(min_coord[0], max_coord[1], 0, 0);
	tVector coord_c = tVector(max_coord[0], max_coord[1], 0, 0);
	tVector coord_d = tVector(max_coord[0], min_coord[1], 0, 0);
	
	const int vert_len = num_faces * verts_per_face * vert_size;
	const int norm_len = num_faces * verts_per_face * norm_size;
	const int coord_len = num_faces * verts_per_face * coord_size;
	const int idx_len = num_faces * verts_per_face;

	float vert_data[vert_len] =
	{
		static_cast<float>(a[0]), static_cast<float>(a[1]), static_cast<float>(a[2]),
		static_cast<float>(b[0]), static_cast<float>(b[1]), static_cast<float>(b[2]),
		static_cast<float>(c[0]), static_cast<float>(c[1]), static_cast<float>(c[2]),
		static_cast<float>(c[0]), static_cast<float>(c[1]), static_cast<float>(c[2]),
		static_cast<float>(d[0]), static_cast<float>(d[1]), static_cast<float>(d[2]),
		static_cast<float>(a[0]), static_cast<float>(a[1]), static_cast<float>(a[2])
	};

	float norm_data[norm_len] = 
	{
		0.f, 1.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 1.f, 0.f
	};

	float coord_data[coord_len] =
	{
		static_cast<float>(coord_a[0]), static_cast<float>(coord_a[1]),
		static_cast<float>(coord_b[0]), static_cast<float>(coord_b[1]),
		static_cast<float>(coord_c[0]), static_cast<float>(coord_c[1]),
		static_cast<float>(coord_c[0]), static_cast<float>(coord_c[1]),
		static_cast<float>(coord_d[0]), static_cast<float>(coord_d[1]),
		static_cast<float>(coord_a[0]), static_cast<float>(coord_a[1])
	};

	int idx_data[idx_len];
	for (int i = 0; i < idx_len; ++i)
	{
		idx_data[i] = i;
	}

	cMeshUtil::BuildDrawMesh(vert_data, vert_len, norm_data, norm_len, coord_data, coord_len, idx_data, idx_len, out_mesh);
}

void cDrawGround::BuildMeshVar3D(const cGround* ground, cDrawMesh* out_mesh)
{
	assert(ground->GetGroundClass() == cGround::eClassVar3D
		|| ground->GetGroundClass() == cGround::eClassHills3D
		|| ground->GetGroundClass() == cGround::eClassTrail3D
		|| ground->GetGroundClass() == cGround::eClassObstacles3D);
	const tVector tex_size = tVector(0.5, 0.5, 0, 0);
	const int verts_per_face = 6;
	const int vert_size = 3;
	const int norm_size = 3;
	const int coord_size = 2;
	const double norm_threshold = 0.8;

	const cGroundVar3D* ground_var = reinterpret_cast<const cGroundVar3D*>(ground);
	int num_slabs = ground_var->GetNumSlabs();
	int num_faces = 0;
	for (int s = 0; s < num_slabs; ++s)
	{
		const auto& slab = ground_var->GetSlab(s);
		int res_x = slab.mResX;
		int res_z = slab.mResZ;
		int curr_num_faces = (res_x - 1) * (res_z - 1);
		assert(curr_num_faces > 0);
		num_faces += curr_num_faces;
	}

	std::vector<float> vert_data(num_faces * verts_per_face * vert_size);
	std::vector<float> norm_data(num_faces * verts_per_face * norm_size);
	std::vector<float> coord_data(num_faces * verts_per_face * coord_size);
	std::vector<int> idx_data(num_faces * verts_per_face);

	int face_count = 0;
	for (int s = 0; s < num_slabs; ++s)
	{
		const auto& slab = ground_var->GetSlab(s);
		int res_x = slab.mResX;
		int res_z = slab.mResZ;

		for (int j = 0; j < res_z - 1; ++j)
		{
			for (int i = 0; i < res_x - 1; ++i)
			{
				tVector vert0 = slab.GetVertex(i, j);
				tVector vert1 = slab.GetVertex(i, j + 1);
				tVector vert2 = slab.GetVertex(i + 1, j + 1);
				tVector vert3 = slab.GetVertex(i + 1, j);

				int flags0 = slab.GetFlags(i, j);
				int flags1 = slab.GetFlags(i, j + 1);
				int flags2 = slab.GetFlags(i + 1, j + 1);
				int flags3 = slab.GetFlags(i + 1, j);

				tVector coord0 = tVector(vert0[0] / tex_size[0], vert0[2] / tex_size[1], 0, 0);
				tVector coord1 = tVector(vert1[0] / tex_size[0], vert1[2] / tex_size[1], 0, 0);
				tVector coord2 = tVector(vert2[0] / tex_size[0], vert2[2] / tex_size[1], 0, 0);

				tVector coord3 = tVector(vert2[0] / tex_size[0], vert2[2] / tex_size[1], 0, 0);
				tVector coord4 = tVector(vert3[0] / tex_size[0], vert3[2] / tex_size[1], 0, 0);
				tVector coord5 = tVector(vert0[0] / tex_size[0], vert0[2] / tex_size[1], 0, 0);

				tVector normal0 = (vert1 - vert0).cross3(vert2 - vert0).normalized();
				tVector normal1 = (vert3 - vert2).cross3(vert0 - vert2).normalized();

				double norm_y0 = std::abs(normal0[1]);
				double norm_y1 = std::abs(normal1[1]);

				if (((flags0 & (1 << cTerrainGen3D::eVertFlagEnableTex)) == 0)
					|| ((flags1 & (1 << cTerrainGen3D::eVertFlagEnableTex)) == 0)
					|| ((flags2 & (1 << cTerrainGen3D::eVertFlagEnableTex)) == 0))
				{
					coord0.setZero();
					coord1.setZero();
					coord2.setZero();
				}
				
				if (((flags2 & (1 << cTerrainGen3D::eVertFlagEnableTex)) == 0)
					|| ((flags3 & (1 << cTerrainGen3D::eVertFlagEnableTex)) == 0)
					|| ((flags0 & (1 << cTerrainGen3D::eVertFlagEnableTex)) == 0))
				{
					coord3.setZero();
					coord4.setZero();
					coord5.setZero();
				}
				
				const tVector* verts[] = { &vert0, &vert1, &vert2, &vert2, &vert3, &vert0 };
				const tVector* norms[] = { &normal0, &normal0, &normal0, &normal1, &normal1, &normal1 };
				const tVector* coords[] = { &coord0, &coord1, &coord2, &coord3, &coord4, &coord5 };

				for (int v = 0; v < verts_per_face; ++v)
				{
					int vert_offset = ((face_count + j * (res_x - 1) + i) * verts_per_face + v) * vert_size;
					int norm_offset = ((face_count + j * (res_x - 1) + i) * verts_per_face + v) * norm_size;
					int coord_offset = ((face_count + j * (res_x - 1) + i) * verts_per_face + v) * coord_size;

					const tVector* curr_vert = verts[v];
					const tVector* curr_norm = norms[v];
					const tVector* curr_coord = coords[v];

					for (int k = 0; k < vert_size; ++k)
					{
						vert_data[vert_offset + k] = static_cast<float>((*curr_vert)[k]);
					}
					for (int k = 0; k < norm_size; ++k)
					{
						norm_data[norm_offset + k] = static_cast<float>((*curr_norm)[k]);
					}
					for (int k = 0; k < coord_size; ++k)
					{
						coord_data[coord_offset + k] = static_cast<float>((*curr_coord)[k]);
					}
				}
			}
		}

		face_count += (res_x - 1) * (res_z - 1);
	}
	assert(face_count == num_faces);

	for (int i = 0; i < idx_data.size(); ++i)
	{
		idx_data[i] = i;
	}

	cMeshUtil::BuildDrawMesh(vert_data.data(), static_cast<int>(vert_data.size()),
							norm_data.data(), static_cast<int>(norm_data.size()),
							coord_data.data(), static_cast<int>(coord_data.size()),
							idx_data.data(), static_cast<int>(idx_data.size()), 
							out_mesh);
}
void cDrawGround::BuildMeshDynamicObstacles3D(const cGround* ground, cDrawMesh* out_mesh)
{
	BuildMeshPlane(ground, out_mesh);
}

