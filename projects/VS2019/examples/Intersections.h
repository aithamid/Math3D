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
