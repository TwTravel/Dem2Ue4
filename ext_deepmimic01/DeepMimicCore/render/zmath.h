#ifndef __ZMATH__
#define __ZMATH__
#include <string.h>
#include <math.h>
/* Matrix & Vertex */
#define M_PI 3.141592653  

#define X v[0]
#define Y v[1]
#define Z v[2]
#define W v[3]

typedef struct {
	 float v[3];
} V3;

typedef struct {
	 float v[4];
} V4;
	
void gl_MoveV3(V3 *a,V3 *b);
//void gl_MulM3V3(V3 *a,M4 *b,V3 *c);
int  gl_V3_Norm(V3 *a);
V3 gl_V3_New(float x,float y,float z);
V4 gl_V4_New(float x,float y,float z,float w);


class M4
{

public:

M4(){};
float m[4][4];

static inline void gl_M4_Id(M4 *a);
static inline int gl_M4_IsId(M4 *a);
static inline void gl_M4_Move(M4 *a,M4 *b);
static inline void gl_MulM4V3(V3 *a,M4 *b,V3 *c);
static inline void gl_M4_MulV4(V4 * a,M4 *b,V4 * c);
static inline void gl_M4_InvOrtho(M4 *a,M4 b);
static inline void gl_M4_Inv(M4 *a,M4 *b);
static inline void gl_M4_Mul(M4 *c,M4 *a,M4 *b);
static inline void gl_M4_MulLeft(M4 *c,M4 *a);
static inline void gl_M4_Transpose(M4 *a,M4 *b);
static inline void gl_M4_Rotate(M4 *c,float t,int u);
void inline printMat();
//M4	operator*(  M4& y);
//void gl_MulM4V3(V3 *a,M4 *b,V3 *c);
//void gl_M4_MulV4(V4 *a,M4 *b,V4 *c);
} ;

/*M4	M4::operator*(  M4& y)
{
 
		M4 ret = (*this);
		M4::gl_M4_MulLeft(&ret, &y);
		return ret;
	
}*/

void inline M4::printMat()
{
	printf("%.3f, %.3f, %.3f, %.3f\n" , m[0][0], m[0][1], m[0][2], m[0][3] );
	printf("%.3f, %.3f, %.3f, %.3f\n" , m[1][0], m[1][1], m[1][2], m[1][3] );
	printf("%.3f, %.3f, %.3f, %.3f\n" , m[2][0], m[2][1], m[2][2], m[2][3] );
	printf("%.3f, %.3f, %.3f, %.3f\n" , m[3][0], m[3][1], m[3][2], m[3][3] );
}

void inline gl_MoveV3(V3 *a,V3 *b)
{
	memcpy(a,b,sizeof(V3));
}

int inline gl_V3_Norm(V3 *a)
{
	float n;
	n=sqrt(a->X*a->X+a->Y*a->Y+a->Z*a->Z);
	if (n==0) return 1;
	a->X/=n;
	a->Y/=n;
	a->Z/=n;
	return 0;
}

V3  inline gl_V3_New(float x,float y,float z)
{
	 V3 a;
	 a.X=x;
	 a.Y=y;
	 a.Z=z;
	 return a;
}

V4 inline gl_V4_New(float x,float y,float z,float w)
{
  V4 a;
  a.X=x;
  a.Y=y;
  a.Z=z;
  a.W=w;
  return a;
}

//int gl_Matrix_Inv(float *r,float *m,int n);


/* ******* Gestion des matrices 4x4 ****** */

void M4::gl_M4_Id(M4 *a)
{
	int i,j;
	for(i=0;i<4;i++)
	for(j=0;j<4;j++) 
	if (i==j) a->m[i][j]=1.0; else a->m[i][j]=0.0;
}

int M4::gl_M4_IsId(M4 *a)
{
	int i,j;
	for(i=0;i<4;i++)
    for(j=0;j<4;j++) {
      if (i==j) { 
        if (a->m[i][j] != 1.0) return 0;
      } else if (a->m[i][j] != 0.0) return 0;
    }
  return 1;
}

void M4::gl_M4_Mul(M4 *c,M4 *a,M4 *b)
{
  int i,j,k;
  float s;
  for(i=0;i<4;i++)
    for(j=0;j<4;j++) {
      s=0.0;
      for(k=0;k<4;k++) s+=a->m[i][k]*b->m[k][j];
      c->m[i][j]=s;
    }
}

/* c=c*a */
void M4::gl_M4_MulLeft(M4 *c,M4 *b)
{
  int i,j,k;
  float s;
  M4 a;

  /*memcpy(&a, c, 16*sizeof(float));
  */
  a=*c;

  for(i=0;i<4;i++)
    for(j=0;j<4;j++) {
      s=0.0;
      for(k=0;k<4;k++) s+=a.m[i][k]*b->m[k][j];
      c->m[i][j]=s;
    }
}

void M4::gl_M4_Move(M4 *a,M4 *b)
{
	memcpy(a,b,sizeof(M4));
}


void M4::gl_MulM4V3(V3 *a,M4 *b,V3 *c)
{
	 a->X=b->m[0][0]*c->X+b->m[0][1]*c->Y+b->m[0][2]*c->Z+b->m[0][3];
	 a->Y=b->m[1][0]*c->X+b->m[1][1]*c->Y+b->m[1][2]*c->Z+b->m[1][3];
	 a->Z=b->m[2][0]*c->X+b->m[2][1]*c->Y+b->m[2][2]*c->Z+b->m[2][3];
}

 

void M4::gl_M4_MulV4(V4 *a,M4 *b,V4 *c)
{
	 a->X=b->m[0][0]*c->X+b->m[0][1]*c->Y+b->m[0][2]*c->Z+b->m[0][3]*c->W;
	 a->Y=b->m[1][0]*c->X+b->m[1][1]*c->Y+b->m[1][2]*c->Z+b->m[1][3]*c->W;
	 a->Z=b->m[2][0]*c->X+b->m[2][1]*c->Y+b->m[2][2]*c->Z+b->m[2][3]*c->W;
	 a->W=b->m[3][0]*c->X+b->m[3][1]*c->Y+b->m[3][2]*c->Z+b->m[3][3]*c->W;
}
	
/* transposition of a 4x4 matrix */
void M4::gl_M4_Transpose(M4 *a,M4 *b)
{
  a->m[0][0]=b->m[0][0]; 
  a->m[0][1]=b->m[1][0]; 
  a->m[0][2]=b->m[2][0]; 
  a->m[0][3]=b->m[3][0]; 

  a->m[1][0]=b->m[0][1]; 
  a->m[1][1]=b->m[1][1]; 
  a->m[1][2]=b->m[2][1]; 
  a->m[1][3]=b->m[3][1]; 

  a->m[2][0]=b->m[0][2]; 
  a->m[2][1]=b->m[1][2]; 
  a->m[2][2]=b->m[2][2]; 
  a->m[2][3]=b->m[3][2]; 

  a->m[3][0]=b->m[0][3]; 
  a->m[3][1]=b->m[1][3]; 
  a->m[3][2]=b->m[2][3]; 
  a->m[3][3]=b->m[3][3]; 
}

/* inversion of an orthogonal matrix of type Y=M.X+P */ 
void M4::gl_M4_InvOrtho(M4 *a,M4 b)
{
	int i,j;
	float s;
	for(i=0;i<3;i++)
	for(j=0;j<3;j++) a->m[i][j]=b.m[j][i];
	a->m[3][0]=0.0; a->m[3][1]=0.0; a->m[3][2]=0.0; a->m[3][3]=1.0;
	for(i=0;i<3;i++) {
		s=0;
		for(j=0;j<3;j++) s-=b.m[j][i]*b.m[j][3];
		a->m[i][3]=s;
	}
}

/* Inversion of a general nxn matrix.
   Note : m is destroyed */

int inline Matrix_Inv(float *r,float *m,int n)
{
	 int i,j,k,l;
	 float max,tmp,t;

	 /* identitée dans r */
	 for(i=0;i<n*n;i++) r[i]=0;
	 for(i=0;i<n;i++) r[i*n+i]=1;
	 
	 for(j=0;j<n;j++) {
			
			/* recherche du nombre de plus grand module sur la colonne j */
			max=m[j*n+j];
			k=j;
			for(i=j+1;i<n;i++)
				if (fabs(m[i*n+j])>fabs(max)) {
					 k=i;
					 max=m[i*n+j];
				}

      /* non intersible matrix */
      if (max==0) return 1;

			
			/* permutation des lignes j et k */
			if (k!=j) {
				 for(i=0;i<n;i++) {
						tmp=m[j*n+i];
						m[j*n+i]=m[k*n+i];
						m[k*n+i]=tmp;
						
						tmp=r[j*n+i];
						r[j*n+i]=r[k*n+i];
						r[k*n+i]=tmp;
				 }
			}
			
			/* multiplication de la ligne j par 1/max */
			max=1/max;
			for(i=0;i<n;i++) {
				 m[j*n+i]*=max;
				 r[j*n+i]*=max;
			}
			
			
			for(l=0;l<n;l++) if (l!=j) {
				 t=m[l*n+j];
				 for(i=0;i<n;i++) {
						m[l*n+i]-=m[j*n+i]*t;
						r[l*n+i]-=r[j*n+i]*t;
				 }
			}
	 }

	 return 0;
}


/* inversion of a 4x4 matrix */
//<=:   ( a = inv(b) );
void M4::gl_M4_Inv(M4 *a,M4 *b)
{
  M4 tmp;
  memcpy(&tmp, b, 16*sizeof(float));
  /*tmp=*b;*/
  Matrix_Inv(&a->m[0][0],&tmp.m[0][0],4);
}

void M4::gl_M4_Rotate(M4 *a,float t,int u)
{
	 float s,c;
	 int v,w;
   if ((v=u+1)>2) v=0;
	 if ((w=v+1)>2) w=0;
	 s=sin(t);
	 c=cos(t);
	 gl_M4_Id(a);
	 a->m[v][v]=c;	a->m[v][w]=-s;
	 a->m[w][v]=s;	a->m[w][w]=c;
}
	
																										
/* vector arithmetic */
//=======================================================
//=======================================================
inline float SIGN(float x) {
	return (x >= 0.0f) ? +1.0f : -1.0f;
}

inline float NORM(float a, float b, float c, float d) {
	return sqrt(a * a + b * b + c * c + d * d);
}

// quaternion = [w, x, y, z]
V4 inline mRot2Quat(const M4& Mat)
{
	float r11 = Mat.m[0][0];
	float r12 = Mat.m[0][1];
	float r13 = Mat.m[0][2];
	float r21 = Mat.m[1][0];
	float r22 = Mat.m[1][1];
	float r23 = Mat.m[1][2];
	float r31 = Mat.m[2][0];
	float r32 = Mat.m[2][1];
	float r33 = Mat.m[2][2];
	float q0 = (r11 + r22 + r33 + 1.0f) / 4.0f;
	float q1 = (r11 - r22 - r33 + 1.0f) / 4.0f;
	float q2 = (-r11 + r22 - r33 + 1.0f) / 4.0f;
	float q3 = (-r11 - r22 + r33 + 1.0f) / 4.0f;
	if (q0 < 0.0f) {
		q0 = 0.0f;
	}
	if (q1 < 0.0f) {
		q1 = 0.0f;
	}
	if (q2 < 0.0f) {
		q2 = 0.0f;
	}
	if (q3 < 0.0f) {
		q3 = 0.0f;
	}
	q0 = sqrt(q0);
	q1 = sqrt(q1);
	q2 = sqrt(q2);
	q3 = sqrt(q3);
	if (q0 >= q1 && q0 >= q2 && q0 >= q3) {
		q0 *= +1.0f;
		q1 *= SIGN(r32 - r23);
		q2 *= SIGN(r13 - r31);
		q3 *= SIGN(r21 - r12);
	}
	else if (q1 >= q0 && q1 >= q2 && q1 >= q3) {
		q0 *= SIGN(r32 - r23);
		q1 *= +1.0f;
		q2 *= SIGN(r21 + r12);
		q3 *= SIGN(r13 + r31);
	}
	else if (q2 >= q0 && q2 >= q1 && q2 >= q3) {
		q0 *= SIGN(r13 - r31);
		q1 *= SIGN(r21 + r12);
		q2 *= +1.0f;
		q3 *= SIGN(r32 + r23);
	}
	else if (q3 >= q0 && q3 >= q1 && q3 >= q2) {
		q0 *= SIGN(r21 - r12);
		q1 *= SIGN(r31 + r13);
		q2 *= SIGN(r32 + r23);
		q3 *= +1.0f;
	}
	else {
		printf("coding error\n");
	}
	float r = NORM(q0, q1, q2, q3);
	q0 /= r;
	q1 /= r;
	q2 /= r;
	q3 /= r;

	V4 res; 
	res.v[0] = q0;
	res.v[1] = q1;
	res.v[2] = q2;
	res.v[3] = q3;
	
	return res;
}


M4 inline GetRotateMat(double angleX, double angleY, double angleZ)
{
	angleX = angleX * M_PI / 180.0;
	angleY = angleY * M_PI / 180.0;
	angleZ = angleZ * M_PI / 180.0;

	M4 identityMat, rotZ, rotY, rotX;
	M4::gl_M4_Id(&identityMat);
	M4 retM = identityMat;
	M4::gl_M4_Rotate(&rotZ, angleZ, 2);
	M4::gl_M4_Rotate(&rotY, angleY, 1);
	M4::gl_M4_Rotate(&rotX, angleX, 0);
	M4::gl_M4_MulLeft(&retM, &rotZ);
	M4::gl_M4_MulLeft(&retM, &rotY);
	M4::gl_M4_MulLeft(&retM, &rotX);
	return retM;
}

M4 Quaternion2RotMat(float qw, float qx, float qy, float qz)
{
	//First you have to normalize the quaternion :
	const float n = 1.0f / sqrt(qx*qx + qy*qy + qz*qz + qw*qw);
	qx *= n;
	qy *= n;
	qz *= n;
	qw *= n;
	//Then you can create your matrix :
	M4 retM;
	float array[16]={
		1.0f - 2.0f*qy*qy - 2.0f*qz*qz, 2.0f*qx*qy - 2.0f*qz*qw, 2.0f*qx*qz + 2.0f*qy*qw, 0.0f,
			2.0f*qx*qy + 2.0f*qz*qw, 1.0f - 2.0f*qx*qx - 2.0f*qz*qz, 2.0f*qy*qz - 2.0f*qx*qw, 0.0f,
			2.0f*qx*qz - 2.0f*qy*qw, 2.0f*qy*qz + 2.0f*qx*qw, 1.0f - 2.0f*qx*qx - 2.0f*qy*qy, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f};
	memcpy(&retM.m[0][0], array, sizeof(float)* 16);
	
	
	//retM.printMat();

	return retM;

	
}

void DebugEuler2Quant(double angleX, double angleY, double angleZ)
{
	M4 retM = GetRotateMat( angleX,  angleY,  angleZ);
	V4 quant = mRot2Quat(retM);
	printf("%6.2f, %6.2f, %6.2f, %6.2f\n", quant.v[0], quant.v[1], quant.v[2], quant.v[3]);
	M4 invMat; 
	M4::gl_M4_Inv(&retM, &invMat);
	quant = mRot2Quat(retM);
	//printf("inv: %6.2f, %6.2f, %6.2f, %6.2f\n", quant.v[0], quant.v[1], quant.v[2], quant.v[3]);
}

void DebugMat2Quant(M4 retM)
{
	V4 quant = mRot2Quat(retM);
	printf("%6.3f, %6.3f, %6.3f, %6.3f );\n", quant.v[0], quant.v[1], quant.v[2], quant.v[3]);
}

M4 operator *( M4&a,M4&b)
{
	M4 ret = a;
	M4::gl_M4_MulLeft(&ret, &b);
	return ret;
}

M4 InvM4(M4&a)
{
//	a.printMat();
	M4 ret;
	M4 copya = a;
	M4::gl_M4_Inv(&ret, &copya);
//	a.printMat();
	return ret;
}
void DebugEuler2Quant(double angleX, double angleY, double angleZ, M4 baseMT)
{
	M4 retM = GetRotateMat(angleX, angleY, angleZ);
	M4::gl_M4_MulLeft(&baseMT, &retM);
	V4 quant = mRot2Quat(baseMT);
	printf("%6.2f, %6.2f, %6.2f, %6.2f\n", quant.v[0], quant.v[1], quant.v[2], quant.v[3]);
}


void DebugEuler2Quant(double angleX, double angleY, double angleZ,
	double base_angleX, double base_angleY, double base_angleZ)
	//M4 baseMT)
{
	M4  retM     = GetRotateMat(angleX, angleY, angleZ);
	M4 baseMT = GetRotateMat(base_angleX,   base_angleY,  base_angleZ);
	M4::gl_M4_MulLeft(&baseMT, &retM);
	V4 quant = mRot2Quat(baseMT);
	printf("%6.2f, %6.2f, %6.2f, %6.2f\n", quant.v[0], quant.v[1], quant.v[2], quant.v[3]);
}

//==========================================================================================
#include <iostream>
#include <cmath> 

using namespace std;

///////////////////////////////
// Quaternion struct
// Simple incomplete quaternion struct for demo purpose
///////////////////////////////
struct Quaternion {
	Quaternion() :x(0), y(0), z(0), w(1) {};
	Quaternion(double x, double y, double z, double w) :x(x), y(y), z(z), w(w) {};

	void normalize() {
		double norm = std::sqrt(x*x + y * y + z * z + w * w);
		x /= norm;
		y /= norm;
		z /= norm;
		w /= norm;
	}

	double norm() {
		return std::sqrt(x*x + y * y + z * z + w * w);
	}

	double x;
	double y;
	double z;
	double w;

};

///////////////////////////////
// Quaternion to Euler
///////////////////////////////
enum RotSeq { zyx, zyz, zxy, zxz, yxz, yxy, yzx, yzy, xyz, xyx, xzy, xzx };

void  inline twoaxisrot(double r11, double r12, double r21, double r31, double r32, double res[]) {
	res[0] = atan2(r11, r12);
	res[1] = acos(r21);
	res[2] = atan2(r31, r32);
}

void  inline threeaxisrot(double r11, double r12, double r21, double r31, double r32, double res[]) {
	res[0] = atan2(r31, r32);
	res[1] = asin(r21);
	res[2] = atan2(r11, r12);
}

void  inline quaternion2Euler(const Quaternion& q, double res[], RotSeq rotSeq)
{
	switch (rotSeq) {
	case zyx:
		threeaxisrot(2 * (q.x*q.y + q.w*q.z),
			q.w*q.w + q.x*q.x - q.y*q.y - q.z*q.z,
			-2 * (q.x*q.z - q.w*q.y),
			2 * (q.y*q.z + q.w*q.x),
			q.w*q.w - q.x*q.x - q.y*q.y + q.z*q.z,
			res);
		break;

	case zyz:
		twoaxisrot(2 * (q.y*q.z - q.w*q.x),
			2 * (q.x*q.z + q.w*q.y),
			q.w*q.w - q.x*q.x - q.y*q.y + q.z*q.z,
			2 * (q.y*q.z + q.w*q.x),
			-2 * (q.x*q.z - q.w*q.y),
			res);
		break;

	case zxy:
		threeaxisrot(-2 * (q.x*q.y - q.w*q.z),
			q.w*q.w - q.x*q.x + q.y*q.y - q.z*q.z,
			2 * (q.y*q.z + q.w*q.x),
			-2 * (q.x*q.z - q.w*q.y),
			q.w*q.w - q.x*q.x - q.y*q.y + q.z*q.z,
			res);
		break;

	case zxz:
		twoaxisrot(2 * (q.x*q.z + q.w*q.y),
			-2 * (q.y*q.z - q.w*q.x),
			q.w*q.w - q.x*q.x - q.y*q.y + q.z*q.z,
			2 * (q.x*q.z - q.w*q.y),
			2 * (q.y*q.z + q.w*q.x),
			res);
		break;

	case yxz:
		threeaxisrot(2 * (q.x*q.z + q.w*q.y),
			q.w*q.w - q.x*q.x - q.y*q.y + q.z*q.z,
			-2 * (q.y*q.z - q.w*q.x),
			2 * (q.x*q.y + q.w*q.z),
			q.w*q.w - q.x*q.x + q.y*q.y - q.z*q.z,
			res);
		break;

	case yxy:
		twoaxisrot(2 * (q.x*q.y - q.w*q.z),
			2 * (q.y*q.z + q.w*q.x),
			q.w*q.w - q.x*q.x + q.y*q.y - q.z*q.z,
			2 * (q.x*q.y + q.w*q.z),
			-2 * (q.y*q.z - q.w*q.x),
			res);
		break;

	case yzx:
		threeaxisrot(-2 * (q.x*q.z - q.w*q.y),
			q.w*q.w + q.x*q.x - q.y*q.y - q.z*q.z,
			2 * (q.x*q.y + q.w*q.z),
			-2 * (q.y*q.z - q.w*q.x),
			q.w*q.w - q.x*q.x + q.y*q.y - q.z*q.z,
			res);
		break;

	case yzy:
		twoaxisrot(2 * (q.y*q.z + q.w*q.x),
			-2 * (q.x*q.y - q.w*q.z),
			q.w*q.w - q.x*q.x + q.y*q.y - q.z*q.z,
			2 * (q.y*q.z - q.w*q.x),
			2 * (q.x*q.y + q.w*q.z),
			res);
		break;

	case xyz:
		threeaxisrot(-2 * (q.y*q.z - q.w*q.x),
			q.w*q.w - q.x*q.x - q.y*q.y + q.z*q.z,
			2 * (q.x*q.z + q.w*q.y),
			-2 * (q.x*q.y - q.w*q.z),
			q.w*q.w + q.x*q.x - q.y*q.y - q.z*q.z,
			res);
		break;

	case xyx:
		twoaxisrot(2 * (q.x*q.y + q.w*q.z),
			-2 * (q.x*q.z - q.w*q.y),
			q.w*q.w + q.x*q.x - q.y*q.y - q.z*q.z,
			2 * (q.x*q.y - q.w*q.z),
			2 * (q.x*q.z + q.w*q.y),
			res);
		break;

	case xzy:
		threeaxisrot(2 * (q.y*q.z + q.w*q.x),
			q.w*q.w - q.x*q.x + q.y*q.y - q.z*q.z,
			-2 * (q.x*q.y - q.w*q.z),
			2 * (q.x*q.z + q.w*q.y),
			q.w*q.w + q.x*q.x - q.y*q.y - q.z*q.z,
			res);
		break;

	case xzx:
		twoaxisrot(2 * (q.x*q.z - q.w*q.y),
			2 * (q.x*q.y + q.w*q.z),
			q.w*q.w + q.x*q.x - q.y*q.y - q.z*q.z,
			2 * (q.x*q.z + q.w*q.y),
			-2 * (q.x*q.y - q.w*q.z),
			res);
		break;
	default:
		std::cout << "Unknown rotation sequence" << std::endl;
		break;
	}
}

///////////////////////////////
// Helper functions
///////////////////////////////
Quaternion  inline operator*(Quaternion& q1, Quaternion& q2) {
	Quaternion q;
	q.w = q1.w*q2.w - q1.x*q2.x - q1.y*q2.y - q1.z*q2.z;
	q.x = q1.w*q2.x + q1.x*q2.w + q1.y*q2.z - q1.z*q2.y;
	q.y = q1.w*q2.y - q1.x*q2.z + q1.y*q2.w + q1.z*q2.x;
	q.z = q1.w*q2.z + q1.x*q2.y - q1.y*q2.x + q1.z*q2.w;
	return q;
}

/*
inline ostream&   operator <<(std::ostream& stream, const Quaternion& q) {
cout << q.w << " " << showpos << q.x << "i " << q.y << "j " << q.z << "k";
cout << noshowpos;
}
*/

double rad2deg(double rad) {
	return rad * 180.0 / M_PI;
}

//##########################################################################
//##########################################################################
class MimicMotionInfo
{
public:
	MimicMotionInfo();
	void init();//MimicMotionInfo(const MimicMotionInfo & c);

	M4 root;
	M4 chest;
	M4 neck;
	M4 right_hip;
	float right_knee;
	M4  right_ankle;
	M4  right_clavicle;
	M4  right_shoulder;
	float right_elbow;
	M4 left_hip;
	float left_knee;
	M4 left_ankle;
	M4 left_clavicle;
	M4 left_shoulder;
	float left_elbow;
	void PrintCharFrame();
	void CorrectZeroPose();
	map<string, int> jnameMapIdx;
	map<string, M4> jnameGlobalTrsMap;
	map<string, M4> jnameLocalTrsMap;
	int Flag;
	float root_height;
	M4 jointMat[17];
	//void JointTranformBack();
	void  JointTranformBack(vector<M4>&jmatvec);
};

void inline printout(M4&rotMat)
{
	V4 quant = mRot2Quat(rotMat);
	printf("%6.2f, %6.2f, %6.2f, %6.2f, \n", quant.v[0], quant.v[1], quant.v[2], quant.v[3]);
}


void inline CorrectML(M4& inputMat)
{
	M4  ZeroPose = GetRotateMat(00, 180, 0.0);
	M4::gl_M4_MulLeft(&ZeroPose, &inputMat);
	inputMat = ZeroPose;
}

void inline CorrectMR(M4& inputMat)
{
	M4  ZeroPose = GetRotateMat(0.0, 180, 0);
	M4::gl_M4_MulLeft(&ZeroPose, &inputMat);
	inputMat = ZeroPose;
}

void MimicMotionInfo::CorrectZeroPose()
{
	M4  ZeroPose = GetRotateMat(180.0, 180.0, 0);
}

void MimicMotionInfo::JointTranformBack(vector<M4>&jmatvec)
{
	M4 Minit;
	float mval[16] = { 0, 1, 0, 0,/**/1, 0, 0, 0,/**/0, 0, -1, 0,/**/0, 0, 0, 1 };
	memcpy(Minit.m, mval, 16 * 4);

	M4 identityMat;
	M4::gl_M4_Id(&identityMat);
	this->root           = jmatvec[0]; 
	this->chest          = jmatvec[1]; 
	this->neck           = jmatvec[2]; 
	this->right_hip      = jmatvec[3];  //<----
//	this->right_knee     = jmatvec[4]; 
	this->right_ankle    = jmatvec[5];  //<----X
	this->right_clavicle = jmatvec[6];  //<----
	this->right_shoulder = jmatvec[7];  //<----
//	this->right_elbow    = jmatvec[8]; 
//	this->right_wrist    = jmatvec[9]; 
	this->left_hip       = jmatvec[10];  //<----
//	this->left_knee      = jmatvec[11]; 
	this->left_ankle     = jmatvec[12]; //<----X 
	this->left_clavicle  = jmatvec[13]; //<----
	this->left_shoulder  = jmatvec[14]; //<----
//	this->left_elbow     = jmatvec[15]; 
//	this->left_wrist     = jmatvec[16]; 
	//M4 jointMat[17];
	for(int i=0;i<17;i++) jointMat[i]= identityMat;

	jointMat[0] = jmatvec[0] * Minit;
	jointMat[1] = InvM4(Minit)*jmatvec[1] * Minit;


	jointMat[3] = InvM4(jnameGlobalTrsMap["right_hip"]) * this->right_hip *
		jnameGlobalTrsMap["right_hip"] * jnameLocalTrsMap["right_hip"];
	jointMat[5] = InvM4(jnameGlobalTrsMap["right_ankle"]) * this->right_ankle *
		jnameGlobalTrsMap["right_ankle"] * jnameLocalTrsMap["right_ankle"];
	jointMat[6] = InvM4(jnameGlobalTrsMap["right_clavicle"]) * this->right_clavicle *
		jnameGlobalTrsMap["right_clavicle"] * jnameLocalTrsMap["right_clavicle"];
	jointMat[7] = InvM4(jnameGlobalTrsMap["right_shoulder"]) * this->right_shoulder *
		jnameGlobalTrsMap["right_shoulder"] * jnameLocalTrsMap["right_shoulder"];

	jointMat[10] = InvM4(jnameGlobalTrsMap["left_hip"]) * this->left_hip *
		jnameGlobalTrsMap["left_hip"] * jnameLocalTrsMap["left_hip"];
	jointMat[12] = InvM4(jnameGlobalTrsMap["left_ankle"]) * this->left_ankle *
		jnameGlobalTrsMap["left_ankle"] * jnameLocalTrsMap["left_ankle"];
	jointMat[13] = InvM4(jnameGlobalTrsMap["left_clavicle"]) * this->left_clavicle *
		jnameGlobalTrsMap["left_clavicle"] * jnameLocalTrsMap["left_clavicle"];
	jointMat[14] = InvM4(jnameGlobalTrsMap["left_shoulder"]) * this->left_shoulder *
		jnameGlobalTrsMap["left_shoulder"] * jnameLocalTrsMap["left_shoulder"];/**/

}
void MimicMotionInfo::PrintCharFrame()
{
	//printf("[0.03, \n 0.0, 1.0, 0.0,\n");
	printf("[0.03, \n 0.0, %.3f, 0.0,\n", root_height);
	printout(root);
	printout(chest);
	printout(neck);
	printout(right_hip);
	printf("%.2f,\n", right_knee *M_PI / 180.0);
	printout(right_ankle);
	printout(right_clavicle);
	printout(right_shoulder);
	printf("%.2f,\n", right_elbow *M_PI / 180.0);
	printout(left_hip);
	printf("%.2f,\n", left_knee*M_PI / 180.0);
	printout(left_ankle);
	printout(left_clavicle);
	printout(left_shoulder);
	printf("%.2f\n", left_elbow*M_PI / 180.0);
	printf("],\n");
}

MimicMotionInfo::MimicMotionInfo()
{
	Flag = 12345;
	M4::gl_M4_Id(&root);
	M4::gl_M4_Id(&chest);
	M4::gl_M4_Id(&neck);
	M4::gl_M4_Id(&right_hip);
	M4::gl_M4_Id(&right_ankle);
	M4::gl_M4_Id(&right_clavicle);
	M4::gl_M4_Id(&right_shoulder);
	M4::gl_M4_Id(&left_hip);
	M4::gl_M4_Id(&left_ankle);
	M4::gl_M4_Id(&left_clavicle);
	M4::gl_M4_Id(&left_shoulder);
	right_knee = right_elbow = left_knee = left_elbow = 0;

	jnameMapIdx["chest"] = 5;
	jnameMapIdx["neck"] = 6;
	jnameMapIdx["right_hip"] = 20;
	jnameMapIdx["right_ankle"] = 22;
	jnameMapIdx["right_clavicle"] = 12;
	jnameMapIdx["right_shoulder"] = 13;
	jnameMapIdx["left_hip"] = 16;
	jnameMapIdx["left_ankle"] = 18;
	jnameMapIdx["left_clavicle"] = 8;
	jnameMapIdx["left_shoulder"] = 9;
	jnameMapIdx["left_elbow"] = 10;
	jnameMapIdx["right_elbow"] = 14;

	////////////////REDNDERING ZERO POSE!!!//////////////////////////////
	map<string, string> sgSklMap;
	map<string, string> slSklMap;

	sgSklMap["chest"] = "b_Hips";
	sgSklMap["neck"] = "b_Spine2";
	sgSklMap["right_hip"] = "b_Hips";
	sgSklMap["left_hip"] = "b_Hips";
	sgSklMap["right_ankle"] = "b_RightLeg";
	sgSklMap["left_ankle"] = "b_LeftLeg";
	sgSklMap["right_clavicle"] = "b_Spine2";
	sgSklMap["left_clavicle"] = "b_Spine2";
	sgSklMap["right_shoulder"] = "b_RightClav";
	sgSklMap["left_shoulder"] = "b_LeftClav";

	slSklMap["chest"] = "b_Spine2";
	slSklMap["neck"] = "b_Neck";
	slSklMap["right_hip"] = "b_RightLegUpper";
	slSklMap["left_hip"] = "b_LeftLegUpper";
	slSklMap["right_ankle"] = "b_RightAnkle";
	slSklMap["left_ankle"] = "b_LeftAnkle";
	slSklMap["right_clavicle"] = "b_RightClav";
	slSklMap["left_clavicle"] = "b_LeftClav";
	slSklMap["right_shoulder"] = "b_RightArm";
	slSklMap["left_shoulder"] = "b_LeftArm";

	//####################################
	jnameGlobalTrsMap["RootNode"] = Quaternion2RotMat(1.000, 0.000, 0.000, 0.000);
	jnameLocalTrsMap["RootNode"] = Quaternion2RotMat(1.000, 0.000, 0.000, 0.000);
	//####################################
	//####################################
	jnameGlobalTrsMap["b_Root"] = Quaternion2RotMat(-0.500, 0.500, -0.500, -0.500);
	jnameLocalTrsMap["b_Root"] = Quaternion2RotMat(-0.500, 0.500, -0.500, -0.500);
	//####################################
	//####################################
	jnameGlobalTrsMap["Bip01"] = Quaternion2RotMat(0.707, -0.707, -0.000, 0.000);
	jnameLocalTrsMap["Bip01"] = Quaternion2RotMat(0.707, 0.000, 0.000, -0.707);
	//####################################
	//####################################
	jnameGlobalTrsMap["b_Hips"] = Quaternion2RotMat(0.000, 0.707, 0.707, 0.000);
	jnameLocalTrsMap["b_Hips"] = Quaternion2RotMat(-0.500, 0.500, 0.500, 0.500);
	//####################################
	//####################################
	jnameGlobalTrsMap["b_Spine"] = Quaternion2RotMat(0.000, 0.707, 0.707, 0.000);
	jnameLocalTrsMap["b_Spine"] = Quaternion2RotMat(1.000, 0.000, 0.000, 0.000);
	//####################################
	//####################################
	jnameGlobalTrsMap["b_Spine1"] = Quaternion2RotMat(0.000, 0.707, 0.707, 0.000);
	jnameLocalTrsMap["b_Spine1"] = Quaternion2RotMat(1.000, 0.000, 0.000, 0.000);
	//####################################
	//####################################
	jnameGlobalTrsMap["b_Spine2"] = Quaternion2RotMat(0.000, 0.707, 0.707, 0.000);
	jnameLocalTrsMap["b_Spine2"] = Quaternion2RotMat(1.000, 0.000, 0.000, 0.000);
	//####################################
	//####################################
	jnameGlobalTrsMap["b_Neck"] = Quaternion2RotMat(0.000, 0.707, 0.707, 0.000);
	jnameLocalTrsMap["b_Neck"] = Quaternion2RotMat(1.000, 0.000, 0.000, 0.000);
	//####################################
	//####################################
	jnameGlobalTrsMap["b_Head"] = Quaternion2RotMat(0.000, 0.707, 0.707, 0.000);
	jnameLocalTrsMap["b_Head"] = Quaternion2RotMat(1.000, 0.000, 0.000, 0.000);
	//####################################
	//####################################
	jnameGlobalTrsMap["b_LeftClav"] = Quaternion2RotMat(0.500, -0.500, 0.500, 0.500);
	jnameLocalTrsMap["b_LeftClav"] = Quaternion2RotMat(-0.000, 0.707, 0.000, 0.707);
	//####################################
	//####################################
	jnameGlobalTrsMap["b_LeftArm"] = Quaternion2RotMat(0.707, -0.000, 0.000, -0.707);
	jnameLocalTrsMap["b_LeftArm"] = Quaternion2RotMat(0.000, 0.707, 0.000, -0.707);
	//####################################
	//####################################
	jnameGlobalTrsMap["b_LeftForeArm"] = Quaternion2RotMat(0.707, -0.000, 0.000, -0.707);
	jnameLocalTrsMap["b_LeftForeArm"] = Quaternion2RotMat(1.000, 0.000, 0.000, 0.000);
	//####################################
	//####################################
	jnameGlobalTrsMap["b_LeftHand"] = Quaternion2RotMat(0.707, -0.000, 0.000, -0.707);
	jnameLocalTrsMap["b_LeftHand"] = Quaternion2RotMat(1.000, 0.000, 0.000, 0.000);
	//####################################
	//####################################
	jnameGlobalTrsMap["b_RightClav"] = Quaternion2RotMat(0.500, 0.500, -0.500, 0.500);
	jnameLocalTrsMap["b_RightClav"] = Quaternion2RotMat(0.000, 0.707, 0.000, -0.707);
	//####################################
	//####################################
	jnameGlobalTrsMap["b_RightArm"] = Quaternion2RotMat(0.707, -0.000, 0.000, -0.707);
	jnameLocalTrsMap["b_RightArm"] = Quaternion2RotMat(0.000, 0.707, -0.000, 0.707);
	//####################################
	//####################################
	jnameGlobalTrsMap["b_RightForeArm"] = Quaternion2RotMat(0.707, -0.000, 0.000, -0.707);
	jnameLocalTrsMap["b_RightForeArm"] = Quaternion2RotMat(1.000, 0.000, 0.000, 0.000);
	//####################################
	//####################################
	jnameGlobalTrsMap["b_RightHand"] = Quaternion2RotMat(0.707, -0.000, 0.000, -0.707);
	jnameLocalTrsMap["b_RightHand"] = Quaternion2RotMat(1.000, 0.000, 0.000, 0.000);
	//####################################
	//####################################
	jnameGlobalTrsMap["b_LeftLegUpper"] = Quaternion2RotMat(0.707, 0.000, -0.000, -0.707);
	jnameLocalTrsMap["b_LeftLegUpper"] = Quaternion2RotMat(-0.000, 0.000, 1.000, -0.000);
	//####################################
	//####################################
	jnameGlobalTrsMap["b_LeftLeg"] = Quaternion2RotMat(0.707, 0.000, -0.000, -0.707);
	jnameLocalTrsMap["b_LeftLeg"] = Quaternion2RotMat(1.000, 0.000, 0.000, 0.000);
	//####################################
	//####################################
	jnameGlobalTrsMap["b_LeftAnkle"] = Quaternion2RotMat(0.707, 0.000, -0.000, -0.707);
	jnameLocalTrsMap["b_LeftAnkle"] = Quaternion2RotMat(1.000, 0.000, 0.000, 0.000);
	//####################################
	//####################################
	jnameGlobalTrsMap["b_LeftToe"] = Quaternion2RotMat(0.707, 0.000, -0.000, -0.707);
	jnameLocalTrsMap["b_LeftToe"] = Quaternion2RotMat(1.000, 0.000, 0.000, 0.000);
	//####################################
	//####################################
	jnameGlobalTrsMap["b_RightLegUpper"] = Quaternion2RotMat(0.707, -0.000, 0.000, -0.707);
	jnameLocalTrsMap["b_RightLegUpper"] = Quaternion2RotMat(-0.000, -0.000, 1.000, 0.000);
	//####################################
	//####################################
	jnameGlobalTrsMap["b_RightLeg"] = Quaternion2RotMat(0.707, -0.000, 0.000, -0.707);
	jnameLocalTrsMap["b_RightLeg"] = Quaternion2RotMat(1.000, 0.000, 0.000, 0.000);
	//####################################
	//####################################
	jnameGlobalTrsMap["b_RightAnkle"] = Quaternion2RotMat(0.707, -0.000, 0.000, -0.707);
	jnameLocalTrsMap["b_RightAnkle"] = Quaternion2RotMat(1.000, 0.000, 0.000, 0.000);
	//####################################
	//####################################
	jnameGlobalTrsMap["b_RightToe"] = Quaternion2RotMat(0.707, -0.000, 0.000, -0.707);
	jnameLocalTrsMap["b_RightToe"] = Quaternion2RotMat(1.000, 0.000, 0.000, 0.000);
	//####################################
	//####################################
	jnameGlobalTrsMap["ZombieResident_Mesh"] = Quaternion2RotMat(0.707, -0.707, 0.000, 0.000);
	jnameLocalTrsMap["ZombieResident_Mesh"] = Quaternion2RotMat(0.707, -0.707, 0.000, 0.000);
	//####################################
	//####################################

	jnameGlobalTrsMap["chest"] = jnameGlobalTrsMap[sgSklMap["chest"]];
	jnameGlobalTrsMap["neck"] = jnameGlobalTrsMap[sgSklMap["neck"]];;
	jnameGlobalTrsMap["right_hip"] = jnameGlobalTrsMap[sgSklMap["right_hip"]];
	jnameGlobalTrsMap["left_hip"] = jnameGlobalTrsMap[sgSklMap["left_hip"]];
	jnameGlobalTrsMap["right_ankle"] = jnameGlobalTrsMap[sgSklMap["right_ankle"]];
	jnameGlobalTrsMap["left_ankle"] = jnameGlobalTrsMap[sgSklMap["left_ankle"]];
	jnameGlobalTrsMap["right_clavicle"] = jnameGlobalTrsMap[sgSklMap["right_clavicle"]];
	jnameGlobalTrsMap["left_clavicle"] = jnameGlobalTrsMap[sgSklMap["left_clavicle"]];
	jnameGlobalTrsMap["right_shoulder"] = jnameGlobalTrsMap[sgSklMap["right_shoulder"]];
	jnameGlobalTrsMap["left_shoulder"] = jnameGlobalTrsMap[sgSklMap["left_shoulder"]];

	jnameLocalTrsMap["chest"] = jnameLocalTrsMap[slSklMap["chest"]];
	jnameLocalTrsMap["neck"] = jnameLocalTrsMap[slSklMap["neck"]];
	jnameLocalTrsMap["right_hip"] = jnameLocalTrsMap[slSklMap["right_hip"]];
	jnameLocalTrsMap["left_hip"] = jnameLocalTrsMap[slSklMap["left_hip"]];
	jnameLocalTrsMap["right_ankle"] = jnameLocalTrsMap[slSklMap["right_ankle"]];
	jnameLocalTrsMap["left_ankle"] = jnameLocalTrsMap[slSklMap["left_ankle"]];
	jnameLocalTrsMap["right_clavicle"] = jnameLocalTrsMap[slSklMap["right_clavicle"]];
	jnameLocalTrsMap["left_clavicle"] = jnameLocalTrsMap[slSklMap["left_clavicle"]];
	jnameLocalTrsMap["right_shoulder"] = jnameLocalTrsMap[slSklMap["right_shoulder"]];
	jnameLocalTrsMap["left_shoulder"] = jnameLocalTrsMap[slSklMap["left_shoulder"]];
	//map<string, M4> jnameGlobalTrsMap;  map<string, M4> jnameLocalTrsMap;   
	//M4 Quaternion2RotMat(float qw, float qx, float qy, float qz)
  
}

void MimicMotionInfo::init()//const MimicMotionInfo & c)
{
	Flag = 123456;
	M4::gl_M4_Id(&root);
	M4::gl_M4_Id(&chest);
	M4::gl_M4_Id(&neck);
	M4::gl_M4_Id(&right_hip);
	M4::gl_M4_Id(&right_ankle);
	M4::gl_M4_Id(&right_clavicle);
	M4::gl_M4_Id(&right_shoulder);
	M4::gl_M4_Id(&left_hip);
	M4::gl_M4_Id(&left_ankle);
	M4::gl_M4_Id(&left_clavicle);
	M4::gl_M4_Id(&left_shoulder);
	right_knee = right_elbow = left_knee = left_elbow = 0;

	jnameMapIdx["chest"] = 5;
	jnameMapIdx["neck"] = 6;
	jnameMapIdx["right_hip"] = 20;
	jnameMapIdx["right_ankle"] = 22;
	jnameMapIdx["right_clavicle"] = 12;
	jnameMapIdx["right_shoulder"] = 13;
	jnameMapIdx["left_hip"] = 16;
	jnameMapIdx["left_ankle"] = 18;
	jnameMapIdx["left_clavicle"] = 8;
	jnameMapIdx["left_shoulder"] = 9;
	jnameMapIdx["left_elbow"] = 10;
	jnameMapIdx["right_elbow"] = 14;

	////////////////REDNDERING ZERO POSE!!!//////////////////////////////
	map<string, string> sgSklMap;
	map<string, string> slSklMap;

	sgSklMap["chest"] = "b_Hips";
	sgSklMap["neck"] = "b_Spine2";
	sgSklMap["right_hip"] = "b_Hips";
	sgSklMap["left_hip"] = "b_Hips";
	sgSklMap["right_ankle"] = "b_RightLeg";
	sgSklMap["left_ankle"] = "b_LeftLeg";
	sgSklMap["right_clavicle"] = "b_Spine2";
	sgSklMap["left_clavicle"] = "b_Spine2";
	sgSklMap["right_shoulder"] = "b_RightClav";
	sgSklMap["left_shoulder"] = "b_LeftClav";

	slSklMap["chest"] = "b_Spine2";
	slSklMap["neck"] = "b_Neck";
	slSklMap["right_hip"] = "b_RightLegUpper";
	slSklMap["left_hip"] = "b_LeftLegUpper";
	slSklMap["right_ankle"] = "b_RightAnkle";
	slSklMap["left_ankle"] = "b_LeftAnkle";
	slSklMap["right_clavicle"] = "b_RightClav";
	slSklMap["left_clavicle"] = "b_LeftClav";
	slSklMap["right_shoulder"] = "b_RightArm";
	slSklMap["left_shoulder"] = "b_LeftArm";

	//####################################
	jnameGlobalTrsMap["RootNode"] = Quaternion2RotMat(1.000, 0.000, 0.000, 0.000);
	jnameLocalTrsMap["RootNode"] = Quaternion2RotMat(1.000, 0.000, 0.000, 0.000);
	//####################################
	//####################################
	jnameGlobalTrsMap["b_Root"] = Quaternion2RotMat(-0.500, 0.500, -0.500, -0.500);
	jnameLocalTrsMap["b_Root"] = Quaternion2RotMat(-0.500, 0.500, -0.500, -0.500);
	//####################################
	//####################################
	jnameGlobalTrsMap["Bip01"] = Quaternion2RotMat(0.707, -0.707, -0.000, 0.000);
	jnameLocalTrsMap["Bip01"] = Quaternion2RotMat(0.707, 0.000, 0.000, -0.707);
	//####################################
	//####################################
	jnameGlobalTrsMap["b_Hips"] = Quaternion2RotMat(0.000, 0.707, 0.707, 0.000);
	jnameLocalTrsMap["b_Hips"] = Quaternion2RotMat(-0.500, 0.500, 0.500, 0.500);
	//####################################
	//####################################
	jnameGlobalTrsMap["b_Spine"] = Quaternion2RotMat(0.000, 0.707, 0.707, 0.000);
	jnameLocalTrsMap["b_Spine"] = Quaternion2RotMat(1.000, 0.000, 0.000, 0.000);
	//####################################
	//####################################
	jnameGlobalTrsMap["b_Spine1"] = Quaternion2RotMat(0.000, 0.707, 0.707, 0.000);
	jnameLocalTrsMap["b_Spine1"] = Quaternion2RotMat(1.000, 0.000, 0.000, 0.000);
	//####################################
	//####################################
	jnameGlobalTrsMap["b_Spine2"] = Quaternion2RotMat(0.000, 0.707, 0.707, 0.000);
	jnameLocalTrsMap["b_Spine2"] = Quaternion2RotMat(1.000, 0.000, 0.000, 0.000);
	//####################################
	//####################################
	jnameGlobalTrsMap["b_Neck"] = Quaternion2RotMat(0.000, 0.707, 0.707, 0.000);
	jnameLocalTrsMap["b_Neck"] = Quaternion2RotMat(1.000, 0.000, 0.000, 0.000);
	//####################################
	//####################################
	jnameGlobalTrsMap["b_Head"] = Quaternion2RotMat(0.000, 0.707, 0.707, 0.000);
	jnameLocalTrsMap["b_Head"] = Quaternion2RotMat(1.000, 0.000, 0.000, 0.000);
	//####################################
	//####################################
	jnameGlobalTrsMap["b_LeftClav"] = Quaternion2RotMat(0.500, -0.500, 0.500, 0.500);
	jnameLocalTrsMap["b_LeftClav"] = Quaternion2RotMat(-0.000, 0.707, 0.000, 0.707);
	//####################################
	//####################################
	jnameGlobalTrsMap["b_LeftArm"] = Quaternion2RotMat(0.707, -0.000, 0.000, -0.707);
	jnameLocalTrsMap["b_LeftArm"] = Quaternion2RotMat(0.000, 0.707, 0.000, -0.707);
	//####################################
	//####################################
	jnameGlobalTrsMap["b_LeftForeArm"] = Quaternion2RotMat(0.707, -0.000, 0.000, -0.707);
	jnameLocalTrsMap["b_LeftForeArm"] = Quaternion2RotMat(1.000, 0.000, 0.000, 0.000);
	//####################################
	//####################################
	jnameGlobalTrsMap["b_LeftHand"] = Quaternion2RotMat(0.707, -0.000, 0.000, -0.707);
	jnameLocalTrsMap["b_LeftHand"] = Quaternion2RotMat(1.000, 0.000, 0.000, 0.000);
	//####################################
	//####################################
	jnameGlobalTrsMap["b_RightClav"] = Quaternion2RotMat(0.500, 0.500, -0.500, 0.500);
	jnameLocalTrsMap["b_RightClav"] = Quaternion2RotMat(0.000, 0.707, 0.000, -0.707);
	//####################################
	//####################################
	jnameGlobalTrsMap["b_RightArm"] = Quaternion2RotMat(0.707, -0.000, 0.000, -0.707);
	jnameLocalTrsMap["b_RightArm"] = Quaternion2RotMat(0.000, 0.707, -0.000, 0.707);
	//####################################
	//####################################
	jnameGlobalTrsMap["b_RightForeArm"] = Quaternion2RotMat(0.707, -0.000, 0.000, -0.707);
	jnameLocalTrsMap["b_RightForeArm"] = Quaternion2RotMat(1.000, 0.000, 0.000, 0.000);
	//####################################
	//####################################
	jnameGlobalTrsMap["b_RightHand"] = Quaternion2RotMat(0.707, -0.000, 0.000, -0.707);
	jnameLocalTrsMap["b_RightHand"] = Quaternion2RotMat(1.000, 0.000, 0.000, 0.000);
	//####################################
	//####################################
	jnameGlobalTrsMap["b_LeftLegUpper"] = Quaternion2RotMat(0.707, 0.000, -0.000, -0.707);
	jnameLocalTrsMap["b_LeftLegUpper"] = Quaternion2RotMat(-0.000, 0.000, 1.000, -0.000);
	//####################################
	//####################################
	jnameGlobalTrsMap["b_LeftLeg"] = Quaternion2RotMat(0.707, 0.000, -0.000, -0.707);
	jnameLocalTrsMap["b_LeftLeg"] = Quaternion2RotMat(1.000, 0.000, 0.000, 0.000);
	//####################################
	//####################################
	jnameGlobalTrsMap["b_LeftAnkle"] = Quaternion2RotMat(0.707, 0.000, -0.000, -0.707);
	jnameLocalTrsMap["b_LeftAnkle"] = Quaternion2RotMat(1.000, 0.000, 0.000, 0.000);
	//####################################
	//####################################
	jnameGlobalTrsMap["b_LeftToe"] = Quaternion2RotMat(0.707, 0.000, -0.000, -0.707);
	jnameLocalTrsMap["b_LeftToe"] = Quaternion2RotMat(1.000, 0.000, 0.000, 0.000);
	//####################################
	//####################################
	jnameGlobalTrsMap["b_RightLegUpper"] = Quaternion2RotMat(0.707, -0.000, 0.000, -0.707);
	jnameLocalTrsMap["b_RightLegUpper"] = Quaternion2RotMat(-0.000, -0.000, 1.000, 0.000);
	//####################################
	//####################################
	jnameGlobalTrsMap["b_RightLeg"] = Quaternion2RotMat(0.707, -0.000, 0.000, -0.707);
	jnameLocalTrsMap["b_RightLeg"] = Quaternion2RotMat(1.000, 0.000, 0.000, 0.000);
	//####################################
	//####################################
	jnameGlobalTrsMap["b_RightAnkle"] = Quaternion2RotMat(0.707, -0.000, 0.000, -0.707);
	jnameLocalTrsMap["b_RightAnkle"] = Quaternion2RotMat(1.000, 0.000, 0.000, 0.000);
	//####################################
	//####################################
	jnameGlobalTrsMap["b_RightToe"] = Quaternion2RotMat(0.707, -0.000, 0.000, -0.707);
	jnameLocalTrsMap["b_RightToe"] = Quaternion2RotMat(1.000, 0.000, 0.000, 0.000);
	//####################################
	//####################################
	jnameGlobalTrsMap["ZombieResident_Mesh"] = Quaternion2RotMat(0.707, -0.707, 0.000, 0.000);
	jnameLocalTrsMap["ZombieResident_Mesh"] = Quaternion2RotMat(0.707, -0.707, 0.000, 0.000);
	//####################################
	//####################################

	jnameGlobalTrsMap["chest"] = jnameGlobalTrsMap[sgSklMap["chest"]];
	jnameGlobalTrsMap["neck"] = jnameGlobalTrsMap[sgSklMap["neck"]]; ;
	jnameGlobalTrsMap["right_hip"] = jnameGlobalTrsMap[sgSklMap["right_hip"]];
	jnameGlobalTrsMap["left_hip"] = jnameGlobalTrsMap[sgSklMap["left_hip"]];
	jnameGlobalTrsMap["right_ankle"] = jnameGlobalTrsMap[sgSklMap["right_ankle"]];
	jnameGlobalTrsMap["left_ankle"] = jnameGlobalTrsMap[sgSklMap["left_ankle"]];
	jnameGlobalTrsMap["right_clavicle"] = jnameGlobalTrsMap[sgSklMap["right_clavicle"]];
	jnameGlobalTrsMap["left_clavicle"] = jnameGlobalTrsMap[sgSklMap["left_clavicle"]];
	jnameGlobalTrsMap["right_shoulder"] = jnameGlobalTrsMap[sgSklMap["right_shoulder"]];
	jnameGlobalTrsMap["left_shoulder"] = jnameGlobalTrsMap[sgSklMap["left_shoulder"]];

	jnameLocalTrsMap["chest"] = jnameLocalTrsMap[slSklMap["chest"]];
	jnameLocalTrsMap["neck"] = jnameLocalTrsMap[slSklMap["neck"]];
	jnameLocalTrsMap["right_hip"] = jnameLocalTrsMap[slSklMap["right_hip"]];
	jnameLocalTrsMap["left_hip"] = jnameLocalTrsMap[slSklMap["left_hip"]];
	jnameLocalTrsMap["right_ankle"] = jnameLocalTrsMap[slSklMap["right_ankle"]];
	jnameLocalTrsMap["left_ankle"] = jnameLocalTrsMap[slSklMap["left_ankle"]];
	jnameLocalTrsMap["right_clavicle"] = jnameLocalTrsMap[slSklMap["right_clavicle"]];
	jnameLocalTrsMap["left_clavicle"] = jnameLocalTrsMap[slSklMap["left_clavicle"]];
	jnameLocalTrsMap["right_shoulder"] = jnameLocalTrsMap[slSklMap["right_shoulder"]];
	jnameLocalTrsMap["left_shoulder"] = jnameLocalTrsMap[slSklMap["left_shoulder"]];

	//###############################################################################

}

#endif  __ZMATH__
