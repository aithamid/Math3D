#pragma once
#define INTERSECTIONS_H
#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <cstdlib>
//#include "My3DPrimitives.h"

using namespace std;

typedef struct Line {
	Vector3 pt;
	Vector3 dir;
} Line;

typedef struct Plane {
	Vector3 normal;
	float d;
} Plane;

typedef struct Segment {
	Vector3 pt1;
	Vector3 pt2;
} Segment;

// Vector conversion
Vector3 LocalToGlobalVect(Vector3 localPos, ReferenceFrame localRef)
{
	Vector3 globalVect = { (localPos.x + localRef.origin.x), (localPos.y + localRef.origin.y), (localPos.z + localRef.origin.z) };
	globalVect = Vector3RotateByQuaternion(globalVect, localRef.q);
	return globalVect;
}

Vector3 GlobalToLocalVect(Vector3 globalPos, ReferenceFrame localRef)
{
	Vector3 localVect = { (globalPos.x - localRef.origin.x), (globalPos.y - localRef.origin.y), (globalPos.z - localRef.origin.z) };
	localVect = Vector3RotateByQuaternion(localVect, localRef.q);
	//QuaternionFromAxisAngle(localVect);
	return localVect;
}




// Position conversion
Vector3 LocalToGlobalPos(Vector3 localPos, ReferenceFrame localRef)
{
	Vector3 globalpos = { (localPos.x + localRef.origin.x), (localPos.y + localRef.origin.y ), (localPos.z + localRef.origin.z) };

	return globalpos;
}

Vector3 GlobalToLocalPos(Vector3 globalPos, ReferenceFrame localRef)
{
	Vector3 localVect = { (globalPos.x - localRef.origin.x), (globalPos.y - localRef.origin.y), (globalPos.z - localRef.origin.z) };

	return localVect;
}

bool IntersectLinePlane(Line line, Plane plane, float& t, Vector3& interPt, Vector3&
	interNormal)
{
	// no intersection if line is parallel to the plane
	float dotProd = Vector3DotProduct(plane.normal, line.dir);
	if (fabsf(dotProd) < EPSILON) return false;
	// intersection: t, interPt & interNormal
	t = (plane.d - Vector3DotProduct(plane.normal, line.pt)) / dotProd;
	interPt = Vector3Add(line.pt, Vector3Scale(line.dir, t)); // OM = OA+tAB
	interNormal = Vector3Scale(plane.normal,
		Vector3DotProduct(Vector3Subtract(line.pt, interPt), plane.normal) < 0 ? -1.f : 1.f);
	return true;
}


bool IntersectSegmentPlane(Segment segment, Plane plane, float& t, Vector3& interPt, Vector3&
	interNormal)
{
	Vector3 dir = Vector3Subtract(segment.pt2, segment.pt1);
	// no intersection if segment is parallel to the plane
	float dotProd = Vector3DotProduct(plane.normal, dir);
	if (fabsf(dotProd) < EPSILON) return false;
	// intersection: t, interPt & interNormal
	t = (plane.d - Vector3DotProduct(plane.normal, segment.pt1)) / dotProd;
	interPt = Vector3Add(segment.pt1, Vector3Scale(dir, t)); // OM = OA+tAB
	interNormal = Vector3Scale(plane.normal,
		Vector3DotProduct(Vector3Subtract(segment.pt1, interPt), plane.normal) < 0 ? -1.f : 1.f);
	return true;
}
