#ifndef __ARKHE_MATRIX_PREPROC_H__
#define __ARKHE_MATRIX_PREPROC_H__

//arkhe local includes (foundation)
#include "ArkheTMath.hpp"
using namespace arkhe::foundation;

//matrix specific preprocessor defines to avoid unnecessary function
//calls while avoiding code duplication...
//
//assumes column-major for all operations. for example:
//
//3x3 matrix is stored in an array [0 1 2 3 4 5 6 7 8]
//and represented as:
// | 0 3 6 |
// | 1 4 7 |
// | 2 5 8 |
//
//and for the 4x4 case it is stored in an array [0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15]
//and represented as
// |  0  4  8  12 |
// |  1  5  9  13 |
// |  2  6  10 14 |
// |  3  7  11 15 |
//

//compute 3x3 deterimant of M and store result in D.
#define ARKHE_MATRIX33_DETERMINANT(M,D)						\
	D = M[0]*M[4]*M[8] + M[3]*M[7]*M[2] + M[6]*M[1]*M[5] -	\
		M[6]*M[4]*M[2] - M[3]*M[1]*M[8] - M[0]*M[7]*M[5];	\

//compute 3x3 adjugate. M is the original matrix and N is it's adjugate.
//the adjugate is the transpose of the cofactor matrix.
#define ARKHE_MATRIX33_ADJUGATE(M,N)	\
	N[0] = M[4]*M[8] - M[7]*M[5];		\
	N[1] = -(M[1]*M[8] - M[7]*M[2]);	\
	N[2] = M[1]*M[5] - M[4]*M[2];		\
	N[3] = -(M[3]*M[8] - M[6]*M[5]);	\
	N[4] = M[0]*M[8] - M[6]*M[2];		\
	N[5] = -(M[0]*M[5] - M[3]*M[2]);	\
	N[6] = M[3]*M[7] - M[6]*M[4];		\
	N[7] = -(M[0]*M[7] - M[6]*M[1]);	\
	N[8] = M[0]*M[4] - M[3]*M[1];		\

//compute 3x3 inverse. M is the original matrix and N is the inverse of M.
//compute the determinant.
//if non-zero, compute reciprocal of the determinant otherwise set zero matrix.
//return the adjugate scaled by the reciprocal of the determinant.
#define ARKHE_MATRIX33_INVERSE(TYPE,M,N)				\
	TYPE det =	M[0]*M[4]*M[8] + M[3]*M[7]*M[2] +		\
				M[6]*M[1]*M[5] - M[6]*M[4]*M[2] -		\
				M[3]*M[1]*M[8] - M[0]*M[7]*M[5];		\
	if(TMath<TYPE>::isZero(det))						\
	{													\
		N[0] = N[1] = N[2] =							\
		N[3] = N[4] = N[5] =							\
		N[6] = N[7] = N[8] = TMath<TYPE>::ZERO;			\
	}													\
	else												\
	{													\
		TYPE recipDet = TMath<TYPE>::ONE / det;			\
		N[0] = (M[4]*M[8] - M[7]*M[5]) * recipDet;		\
		N[1] = -(M[1]*M[8] - M[7]*M[2]) * recipDet;		\
		N[2] = (M[1]*M[5] - M[4]*M[2]) * recipDet;		\
		N[3] = -(M[3]*M[8] - M[6]*M[5]) * recipDet;		\
		N[4] = (M[0]*M[8] - M[6]*M[2]) * recipDet;		\
		N[5] = -(M[0]*M[5] - M[3]*M[2]) * recipDet;		\
		N[6] = (M[3]*M[7] - M[6]*M[4]) * recipDet;		\
		N[7] = -(M[0]*M[7] - M[6]*M[1]) * recipDet;		\
		N[8] = (M[0]*M[4] - M[3]*M[1]) * recipDet;		\
	}													\

//compute 3x3 inverse transpose (used for normal transformations). M is the original matrix and N is the inverse transpose of M.
#define ARKHE_MATRIX33_INVERSE_TRANSPOSE(TYPE,M,N)		\
	TYPE det =	M[0]*M[4]*M[8] + M[3]*M[7]*M[2] +		\
				M[6]*M[1]*M[5] - M[6]*M[4]*M[2] -		\
				M[3]*M[1]*M[8] - M[0]*M[7]*M[5];		\
	if(TMath<TYPE>::isZero(det))						\
	{													\
		N[0] = N[1] = N[2] =							\
		N[3] = N[4] = N[5] =							\
		N[6] = N[7] = N[8] = TMath<TYPE>::ZERO;			\
	}													\
	else												\
	{													\
		TYPE recipDet = TMath<TYPE>::ONE / det;			\
		N[0] = (M[4]*M[8] - M[7]*M[5]) * recipDet;		\
		N[3] = -(M[1]*M[8] - M[7]*M[2]) * recipDet;		\
		N[6] = (M[1]*M[5] - M[4]*M[2]) * recipDet;		\
		N[1] = -(M[3]*M[8] - M[6]*M[5]) * recipDet;		\
		N[4] = (M[0]*M[8] - M[6]*M[2]) * recipDet;		\
		N[7] = -(M[0]*M[5] - M[3]*M[2]) * recipDet;		\
		N[2] = (M[3]*M[7] - M[6]*M[4]) * recipDet;		\
		N[5] = -(M[0]*M[7] - M[6]*M[1]) * recipDet;		\
		N[8] = (M[0]*M[4] - M[3]*M[1]) * recipDet;		\
	}													\

//multiply 3D vector V by matrix M and store result in W.
#define ARKHE_MATRIX33_X_VECTOR3(M,V,W)		\
	W.x = M[0]*V.x + M[3]*V.y + M[6]*V.z;	\
	W.y = M[1]*V.x + M[4]*V.y + M[7]*V.z;	\
	W.z = M[2]*V.x + M[5]*V.y + M[8]*V.z;	\

//compute the 4x4 determinant of M and store the result in D.
#define ARKHE_MATRIX44_DETERMINANT(M,D)											\
	D = M[0]*M[5]*M[10]*M[15] + M[0]*M[6]*M[11]*M[13] + M[0]*M[7]*M[9]*M[14] +	\
		M[1]*M[4]*M[11]*M[14] + M[1]*M[6]*M[8]*M[15] + M[1]*M[7]*M[10]*M[12] +	\
		M[2]*M[4]*M[9]*M[15] + M[2]*M[5]*M[11]*M[12] + M[2]*M[7]*M[8]*M[13] +	\
		M[3]*M[4]*M[10]*M[13] + M[3]*M[5]*M[8]*M[14] + M[3]*M[6]*M[9]*M[12] -	\
		M[0]*M[5]*M[11]*M[14] - M[0]*M[6]*M[9]*M[15] - M[0]*M[7]*M[10]*M[13] -	\
		M[1]*M[4]*M[10]*M[15] - M[1]*M[6]*M[11]*M[12] - M[1]*M[7]*M[8]*M[14] -	\
		M[2]*M[4]*M[11]*M[13] - M[2]*M[5]*M[8]*M[15] - M[2]*M[7]*M[9]*M[12] -	\
		M[3]*M[4]*M[9]*M[14] - M[3]*M[5]*M[10]*M[12] - M[3]*M[6]*M[8]*M[13];	\

//compute 4x4 adjugate. M is the original matrix and N is it's adjugate.
//the adjugate is the transpose of the cofactor matrix
//a cofactor of any entry A(i,j) is:
//C(i,j) = (-1)^(i+j) |A(i,j)| where |A(i,j)| is the minor of A(i,j).
#define ARKHE_MATRIX44_ADJUGATE(TYPE,M,N)														\
	TYPE fp = M[5]*M[15];																		\
	TYPE gp = M[6]*M[15];																		\
	TYPE ho = M[7]*M[14];																		\
	TYPE fo = M[5]*M[14];																		\
	TYPE gn = M[6]*M[13];																		\
	TYPE hn = M[7]*M[13];																		\
	TYPE fl = M[5]*M[11];																		\
	TYPE fm = M[5]*M[12];																		\
	TYPE gl = M[6]*M[11];																		\
	TYPE gm = M[6]*M[12];																		\
	TYPE hk = M[7]*M[10];																		\
	TYPE hm = M[7]*M[12];																		\
	TYPE jp = M[9]*M[15];																		\
	TYPE kp = M[10]*M[15];																		\
	TYPE lo = M[11]*M[14];																		\
	TYPE fk = M[5]*M[10];																		\
	TYPE gj = M[6]*M[9];																		\
	TYPE hj = M[7]*M[9];																		\
	TYPE jo = M[9]*M[14];																		\
	TYPE kn = M[10]*M[13];																		\
	TYPE ln = M[11]*M[13];																		\
	TYPE mc = M[12]*M[2];																		\
	TYPE mk = M[12]*M[10];																		\
	TYPE id = M[8]*M[3];																		\
	TYPE ib = M[8]*M[1];																		\
	TYPE ic = M[8]*M[2];																		\
	TYPE ed = M[4]*M[3];																		\
	TYPE ec = M[4]*M[2];																		\
	TYPE eb = M[4]*M[1];																		\
	N[0] = fp*M[10] + ho*M[9] + gn*M[11] - hn*M[10] - gp*M[9] - fo*M[11];						\
	N[1] = -(kp*M[1] + jo*M[3] + ln*M[2] - kn*M[3] - jp*M[2] - lo*M[1]);						\
	N[2] = gp*M[1] + fo*M[3] + hn*M[2] - gn*M[3] - fp*M[2] - ho*M[1];							\
	N[3] = -(gl*M[1] + fk*M[3] + hj*M[2] - gj*M[3] - fl*M[2] - hk*M[1]);						\
	N[4] = -(kp*M[4] + ho*M[8] + gl*M[12] - hk*M[12] - gp*M[8] - lo*M[4]);						\
	N[5] = kp*M[0] + id*M[14] + mc*M[11] - mk*M[3] - ic*M[15] - lo*M[0];						\
	N[6] = -(gp*M[0] + ed*M[14] + hm*M[2] - gm*M[3] - ec*M[15] - ho*M[0]);						\
	N[7] = gl*M[0] + ed*M[10] + ic*M[7] - id*M[6] - ec*M[11] - hk*M[0];							\
	N[8] = jp*M[4] + hn*M[8] + fl*M[12] - hm*M[9] - fp*M[8] - ln*M[4];							\
	N[9] = -(jp*M[0] + id*M[13] + M[12]*M[1]*M[11] - M[12]*M[9]*M[3] - ib*M[15] - ln*M[0]);		\
	N[10] = fp*M[0] + ed*M[13] + hm*M[1] - fm*M[3] - eb*M[15] - hn*M[0];						\
	N[11] = -(fl*M[0] + ed*M[9] + ib*M[7] - id*M[5] - eb*M[11] - hj*M[0]);						\
	N[12] = -(jo*M[4] + gn*M[8] + fm*M[10] - gm*M[9] - fo*M[8] - kn*M[4]);						\
	N[13] = jo*M[0] + ic*M[13] + mk*M[1] - mc*M[9] - ib*M[14] - kn*M[0];						\
	N[14] = -(fo*M[0] + ec*M[13] + gm*M[1] - fm*M[2] - eb*M[14] - gn*M[0]);						\
	N[15] = fk*M[0] + ec*M[9] + ib*M[6] - ic*M[5] - eb*M[10] - gj*M[0];							\

//compute 4x4 inverse. M is the original matrix and N is the inverse of M.
#define ARKHE_MATRIX44_INVERSE(TYPE,M,N)																		\
	TYPE det =	M[0]*M[5]*M[10]*M[15] + M[0]*M[6]*M[11]*M[13] + M[0]*M[7]*M[9]*M[14] +							\
				M[1]*M[4]*M[11]*M[14] + M[1]*M[6]*M[8]*M[15] + M[1]*M[7]*M[10]*M[12] +							\
				M[2]*M[4]*M[9]*M[15] + M[2]*M[5]*M[11]*M[12] + M[2]*M[7]*M[8]*M[13] +							\
				M[3]*M[4]*M[10]*M[13] + M[3]*M[5]*M[8]*M[14] + M[3]*M[6]*M[9]*M[12] -							\
				M[0]*M[5]*M[11]*M[14] - M[0]*M[6]*M[9]*M[15] - M[0]*M[7]*M[10]*M[13] -							\
				M[1]*M[4]*M[10]*M[15] - M[1]*M[6]*M[11]*M[12] - M[1]*M[7]*M[8]*M[14] -							\
				M[2]*M[4]*M[11]*M[13] - M[2]*M[5]*M[8]*M[15] - M[2]*M[7]*M[9]*M[12] -							\
				M[3]*M[4]*M[9]*M[14] - M[3]*M[5]*M[10]*M[12] - M[3]*M[6]*M[8]*M[13];							\
	if(TMath<TYPE>::isZero(det))																				\
	{																											\
		N[0] = N[1] = N[2] = N[3] =																				\
		N[4] = N[5] = N[6] = N[7] =																				\
		N[8] = N[9] = N[10] = N[11] =																			\
		N[12] = N[13] = N[14] = N[15] = TMath<TYPE>::ZERO;														\
	}																											\
	else																										\
	{																											\
		TYPE recipDet = TMath<TYPE>::ONE / det;																	\
		TYPE fp = M[5]*M[15];																					\
		TYPE gp = M[6]*M[15];																					\
		TYPE ho = M[7]*M[14];																					\
		TYPE fo = M[5]*M[14];																					\
		TYPE gn = M[6]*M[13];																					\
		TYPE hn = M[7]*M[13];																					\
		TYPE fl = M[5]*M[11];																					\
		TYPE fm = M[5]*M[12];																					\
		TYPE gl = M[6]*M[11];																					\
		TYPE gm = M[6]*M[12];																					\
		TYPE hk = M[7]*M[10];																					\
		TYPE hm = M[7]*M[12];																					\
		TYPE jp = M[9]*M[15];																					\
		TYPE kp = M[10]*M[15];																					\
		TYPE lo = M[11]*M[14];																					\
		TYPE fk = M[5]*M[10];																					\
		TYPE gj = M[6]*M[9];																					\
		TYPE hj = M[7]*M[9];																					\
		TYPE jo = M[9]*M[14];																					\
		TYPE kn = M[10]*M[13];																					\
		TYPE ln = M[11]*M[13];																					\
		TYPE mc = M[12]*M[2];																					\
		TYPE mk = M[12]*M[10];																					\
		TYPE id = M[8]*M[3];																					\
		TYPE ib = M[8]*M[1];																					\
		TYPE ic = M[8]*M[2];																					\
		TYPE ed = M[4]*M[3];																					\
		TYPE ec = M[4]*M[2];																					\
		TYPE eb = M[4]*M[1];																					\
		N[0] = (fp*M[10] + ho*M[9] + gn*M[11] - hn*M[10] - gp*M[9] - fo*M[11]) * recipDet;						\
		N[1] = (-(kp*M[1] + jo*M[3] + ln*M[2] - kn*M[3] - jp*M[2] - lo*M[1])) * recipDet;						\
		N[2] = (gp*M[1] + fo*M[3] + hn*M[2] - gn*M[3] - fp*M[2] - ho*M[1]) * recipDet;							\
		N[3] = (-(gl*M[1] + fk*M[3] + hj*M[2] - gj*M[3] - fl*M[2] - hk*M[1])) * recipDet;						\
		N[4] = (-(kp*M[4] + ho*M[8] + gl*M[12] - hk*M[12] - gp*M[8] - lo*M[4])) * recipDet;						\
		N[5] = (kp*M[0] + id*M[14] + mc*M[11] - mk*M[3] - ic*M[15] - lo*M[0]) * recipDet;						\
		N[6] = (-(gp*M[0] + ed*M[14] + hm*M[2] - gm*M[3] - ec*M[15] - ho*M[0])) * recipDet;						\
		N[7] = (gl*M[0] + ed*M[10] + ic*M[7] - id*M[6] - ec*M[11] - hk*M[0]) * recipDet;						\
		N[8] = (jp*M[4] + hn*M[8] + fl*M[12] - hm*M[9] - fp*M[8] - ln*M[4]) * recipDet;							\
		N[9] = (-(jp*M[0] + id*M[13] + M[12]*M[1]*M[11] - M[12]*M[9]*M[3] - ib*M[15] - ln*M[0])) * recipDet;	\
		N[10] = (fp*M[0] + ed*M[13] + hm*M[1] - fm*M[3] - eb*M[15] - hn*M[0]) * recipDet;						\
		N[11] = (-(fl*M[0] + ed*M[9] + ib*M[7] - id*M[5] - eb*M[11] - hj*M[0])) * recipDet;						\
		N[12] = (-(jo*M[4] + gn*M[8] + fm*M[10] - gm*M[9] - fo*M[8] - kn*M[4])) * recipDet;						\
		N[13] = (jo*M[0] + ic*M[13] + mk*M[1] - mc*M[9] - ib*M[14] - kn*M[0]) * recipDet;						\
		N[14] = (-(fo*M[0] + ec*M[13] + gm*M[1] - fm*M[2] - eb*M[14] - gn*M[0])) * recipDet;					\
		N[15] = (fk*M[0] + ec*M[9] + ib*M[6] - ic*M[5] - eb*M[10] - gj*M[0]) * recipDet;						\
	}																											\

//compute 4x4 inverse transpose (used for normal transformations). M is the original matrix and N is the inverse transpose of M.
#define ARKHE_MATRIX44_INVERSE_TRANSPOSE(TYPE,M,N)																\
	TYPE det =	M[0]*M[5]*M[10]*M[15] + M[0]*M[6]*M[11]*M[13] + M[0]*M[7]*M[9]*M[14] +							\
				M[1]*M[4]*M[11]*M[14] + M[1]*M[6]*M[8]*M[15] + M[1]*M[7]*M[10]*M[12] +							\
				M[2]*M[4]*M[9]*M[15] + M[2]*M[5]*M[11]*M[12] + M[2]*M[7]*M[8]*M[13] +							\
				M[3]*M[4]*M[10]*M[13] + M[3]*M[5]*M[8]*M[14] + M[3]*M[6]*M[9]*M[12] -							\
				M[0]*M[5]*M[11]*M[14] - M[0]*M[6]*M[9]*M[15] - M[0]*M[7]*M[10]*M[13] -							\
				M[1]*M[4]*M[10]*M[15] - M[1]*M[6]*M[11]*M[12] - M[1]*M[7]*M[8]*M[14] -							\
				M[2]*M[4]*M[11]*M[13] - M[2]*M[5]*M[8]*M[15] - M[2]*M[7]*M[9]*M[12] -							\
				M[3]*M[4]*M[9]*M[14] - M[3]*M[5]*M[10]*M[12] - M[3]*M[6]*M[8]*M[13];							\
	if(TMath<TYPE>::isZero(det))																				\
	{																											\
		N[0] = N[1] = N[2] = N[3] =																				\
		N[4] = N[5] = N[6] = N[7] =																				\
		N[8] = N[9] = N[10] = N[11] =																			\
		N[12] = N[13] = N[14] = N[15] = TMath<TYPE>::ZERO;														\
	}																											\
	else																										\
	{																											\
		TYPE recipDet = TMath<TYPE>::ONE / det;																	\
		TYPE fp = M[5]*M[15];																					\
		TYPE gp = M[6]*M[15];																					\
		TYPE ho = M[7]*M[14];																					\
		TYPE fo = M[5]*M[14];																					\
		TYPE gn = M[6]*M[13];																					\
		TYPE hn = M[7]*M[13];																					\
		TYPE fl = M[5]*M[11];																					\
		TYPE fm = M[5]*M[12];																					\
		TYPE gl = M[6]*M[11];																					\
		TYPE gm = M[6]*M[12];																					\
		TYPE hk = M[7]*M[10];																					\
		TYPE hm = M[7]*M[12];																					\
		TYPE jp = M[9]*M[15];																					\
		TYPE kp = M[10]*M[15];																					\
		TYPE lo = M[11]*M[14];																					\
		TYPE fk = M[5]*M[10];																					\
		TYPE gj = M[6]*M[9];																					\
		TYPE hj = M[7]*M[9];																					\
		TYPE jo = M[9]*M[14];																					\
		TYPE kn = M[10]*M[13];																					\
		TYPE ln = M[11]*M[13];																					\
		TYPE mc = M[12]*M[2];																					\
		TYPE mk = M[12]*M[10];																					\
		TYPE id = M[8]*M[3];																					\
		TYPE ib = M[8]*M[1];																					\
		TYPE ic = M[8]*M[2];																					\
		TYPE ed = M[4]*M[3];																					\
		TYPE ec = M[4]*M[2];																					\
		TYPE eb = M[4]*M[1];																					\
		N[0] = (fp*M[10] + ho*M[9] + gn*M[11] - hn*M[10] - gp*M[9] - fo*M[11]) * recipDet;						\
		N[4] = (-(kp*M[1] + jo*M[3] + ln*M[2] - kn*M[3] - jp*M[2] - lo*M[1])) * recipDet;						\
		N[8] = (gp*M[1] + fo*M[3] + hn*M[2] - gn*M[3] - fp*M[2] - ho*M[1]) * recipDet;							\
		N[12] = (-(gl*M[1] + fk*M[3] + hj*M[2] - gj*M[3] - fl*M[2] - hk*M[1])) * recipDet;						\
		N[1] = (-(kp*M[4] + ho*M[8] + gl*M[12] - hk*M[12] - gp*M[8] - lo*M[4])) * recipDet;						\
		N[5] = (kp*M[0] + id*M[14] + mc*M[11] - mk*M[3] - ic*M[15] - lo*M[0]) * recipDet;						\
		N[9] = (-(gp*M[0] + ed*M[14] + hm*M[2] - gm*M[3] - ec*M[15] - ho*M[0])) * recipDet;						\
		N[13] = (gl*M[0] + ed*M[10] + ic*M[7] - id*M[6] - ec*M[11] - hk*M[0]) * recipDet;						\
		N[2] = (jp*M[4] + hn*M[8] + fl*M[12] - hm*M[9] - fp*M[8] - ln*M[4]) * recipDet;							\
		N[6] = (-(jp*M[0] + id*M[13] + M[12]*M[1]*M[11] - M[12]*M[9]*M[3] - ib*M[15] - ln*M[0])) * recipDet;	\
		N[10] = (fp*M[0] + ed*M[13] + hm*M[1] - fm*M[3] - eb*M[15] - hn*M[0]) * recipDet;						\
		N[14] = (-(fl*M[0] + ed*M[9] + ib*M[7] - id*M[5] - eb*M[11] - hj*M[0])) * recipDet;						\
		N[3] = (-(jo*M[4] + gn*M[8] + fm*M[10] - gm*M[9] - fo*M[8] - kn*M[4])) * recipDet;						\
		N[7] = (jo*M[0] + ic*M[13] + mk*M[1] - mc*M[9] - ib*M[14] - kn*M[0]) * recipDet;						\
		N[11] = (-(fo*M[0] + ec*M[13] + gm*M[1] - fm*M[2] - eb*M[14] - gn*M[0])) * recipDet;					\
		N[15] = (fk*M[0] + ec*M[9] + ib*M[6] - ic*M[5] - eb*M[10] - gj*M[0]) * recipDet;						\
	}																											\

//multiply 3D vector by matrix M and store result in W.
#define ARKHE_MATRIX44_X_VECTOR3(M,V,W)		\
	W.x = M[0]*V.x + M[4]*V.y + M[8]*V.z;	\
	W.y = M[1]*V.x + M[5]*V.y + M[9]*V.z;	\
	W.z = M[2]*V.x + M[6]*V.y + M[10]*V.z;	\

//multiply 4D point by matrix M and store result in W. We treat
//the w coordinate of the "point" as being 1.
#define ARKHE_MATRIX44_X_HOMOGENOUS_UNIT_W_POINT(M,V,W)		\
	W.x = M[0]*V.x + M[4]*V.y + M[8]*V.z + M[12];			\
	W.y = M[1]*V.x + M[5]*V.y + M[9]*V.z + M[13];			\
	W.z = M[2]*V.x + M[6]*V.y + M[10]*V.z + M[14];			\

#endif //__ARKHE_MATRIX_PREPROC_H__
