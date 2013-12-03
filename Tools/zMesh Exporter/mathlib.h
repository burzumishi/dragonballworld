/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.

This file is part of Quake III Arena source code.

Quake III Arena source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Quake III Arena source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Foobar; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/
#ifndef __MATHLIB_H__
#define __MATHLIB_H__

// mathlib.h

#include <math.h>

#ifdef DOUBLEVEC_T
typedef double vec_t;
#else
typedef float vec_t;
#endif
typedef vec_t vec2_t[3];
typedef vec_t vec3_t[3];
typedef vec_t vec4_t[4];

#define	Q_PI	3.14159265358979323846
#define DEG2RAD( a ) ( ( (a) * Q_PI ) / 180.0F )
#define RAD2DEG( a ) ( ( (a) * 180.0f ) / Q_PI )

extern vec3_t vec3_origin;

#define	EQUAL_EPSILON	0.001

#define DotProduct(x,y) (x[0]*y[0]+x[1]*y[1]+x[2]*y[2])
#define VectorSubtract(a,b,c) {c[0]=a[0]-b[0];c[1]=a[1]-b[1];c[2]=a[2]-b[2];}
#define VectorAdd(a,b,c) {c[0]=a[0]+b[0];c[1]=a[1]+b[1];c[2]=a[2]+b[2];}
#define VectorCopy(a,b) {b[0]=a[0];b[1]=a[1];b[2]=a[2];}
#define VectorScale(a,b,c) {c[0]=b*a[0];c[1]=b*a[1];c[2]=b*a[2];}
#define VectorClear(x) {x[0] = x[1] = x[2] = 0;}
#define	VectorNegate(x) {x[0]=-x[0];x[1]=-x[1];x[2]=-x[2];}

vec_t Q_rint (vec_t in);

double VectorLength( const vec3_t v );
vec_t VectorNormalize( const vec3_t in, vec3_t out );
void VectorMA( const vec3_t va, double scale, const vec3_t vb, vec3_t vc );
qboolean VectorCompare( const vec3_t v1, const vec3_t v2 );

void CrossProduct( const vec3_t v1, const vec3_t v2, vec3_t cross );
vec_t ColorNormalize( const vec3_t in, vec3_t out );

void ClearBounds (vec3_t mins, vec3_t maxs);
void AddPointToBounds( const vec3_t v, vec3_t mins, vec3_t maxs );

void RotatePointAroundVector( vec3_t dst, const vec3_t dir, const vec3_t point, float degrees );

void AnglesToMatrix( vec3_t in, vec3_t out[3] );
void MatrixMultiply(float in1[3][3], float in2[3][3], float out[3][3]);
void VectorRotate( vec3_t v, float r[3][3], vec3_t d );
void VectorInverseRotate( vec3_t v, float r[3][3], vec3_t d );
float RadiusFromBounds( const vec3_t mins, const vec3_t maxs );

#endif /*!__MATHLIB_H__*/
