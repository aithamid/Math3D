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


bool IntersectSegmentPlane(Segment segment, Plane plane, float& t, Vector3& interPt, Vector3& interNormal)
{
	Line line = { segment.pt1, Vector3Subtract(segment.pt2, segment.pt1) };
	if (IntersectLinePlane(line, plane, t, interPt, interNormal))
	{
		if (t >= 0.0f && t <= 1.0f)
		{
			return true;
		}
	}
	return false;
}

bool IntersectSegmentQuad(Segment seg, Quad quad, float& t, Vector3& interPt, Vector3& interNormal)
{
    Plane plane = {quad.ref.j, Vector3DotProduct(quad.ref.origin, quad.ref.j)};

    if (IntersectSegmentPlane(seg, plane, t, interPt, interNormal))
    {

        Vector3 localInterPt = Vector3Subtract(interPt, quad.ref.origin);
        Vector3 localInterPtProjected = Vector3({Vector3DotProduct(localInterPt, quad.ref.i), Vector3DotProduct(localInterPt, quad.ref.k), 0.0f});

        if (abs(localInterPtProjected.x) <= quad.extents.x && abs(localInterPtProjected.y) <= quad.extents.z)
        {
            return true;
        }
    }
    return false;
}

bool IntersectSegmentDisk(Segment segment, Disk disk, float& t, Vector3& interPt, Vector3& interNormal)
{

	Plane plane = { disk.ref.j, Vector3DotProduct(disk.ref.origin, disk.ref.j) };

	if (IntersectSegmentPlane(segment, plane, t, interPt, interNormal))
	{

		float distance = Vector3Length(Vector3Subtract(interPt, disk.ref.origin));

		if (distance <= disk.radius)
		{
			return true;
		}
	}
	return false;
}

bool IntersectSegmentSphere(Segment seg, Sphere sphere, float& t, Vector3& interPt, Vector3& interNormal)
{
    V
}

	
//bool IntersectSegmentInfiniteCylinder(Segment segment, InfiniteCylinder cyl, float& t,
//	Vector3& interPt, Vector3& interNormal){}
//
//bool IntersectSegmentCylinder(Segment segment, Cylinder cyl, float& t, Vector3&
//	interPt, Vector3& interNormal){}
//
//bool IntersectSegmentCapsule(Segment seg, Capsule capsule, float& t, Vector3&
//	interPt, Vector3& interNormal){}
//
//bool IntersectSegmentBox(Segment seg, Box box, float& t, Vector3& interPt,
//	Vector3& interNormal){}
//
//bool IntersectSegmentRoundedBox(Segment seg, RoundedBox rndBox, float& t,
//	Vector3& interPt, Vector3& interNormal){}