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

// Position conversion
Vector3 LocalToGlobalPos(Vector3 localPos, ReferenceFrame localRef)
{
	Vector3 globalpos = { (localPos.x + localRef.origin.x), (localPos.y + localRef.origin.y), (localPos.z + localRef.origin.z) };

	return globalpos;
}

Vector3 GlobalToLocalPos(Vector3 globalPos, ReferenceFrame localRef)
{
	Vector3 localVect = { (globalPos.x - localRef.origin.x), (globalPos.y - localRef.origin.y), (globalPos.z - localRef.origin.z) };

	return localVect;
}

// Vector conversion
Vector3 LocalToGlobalVect(Vector3 localPos, ReferenceFrame localRef)
{
	Vector3 globalVect = { (localPos.x + localRef.origin.x), (localPos.y + localRef.origin.y), (localPos.z + localRef.origin.z) };
	globalVect = Vector3RotateByQuaternion(globalVect, localRef.q);
	return globalVect;
}

Vector3 GlobalToLocalVect(Vector3 globalPos, ReferenceFrame localRef)
{
	Vector3 localInterPt = GlobalToLocalPos(globalPos, localRef);
	Vector3 localInterPtProjected = Vector3({ Vector3DotProduct(localInterPt, localRef.i), Vector3DotProduct(localInterPt, localRef.k), 0.0f });
	//QuaternionFromAxisAngle(localVect);
	return localInterPtProjected;
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
	// on creer un plan infini pour voir si le segment est le touche
	Plane plane = { quad.ref.j, Vector3DotProduct(quad.ref.origin, quad.ref.j) };

	if (IntersectSegmentPlane(seg, plane, t, interPt, interNormal))
	{
		//On se place dans le referentiel local pour voir si le segment est dans le quad
		Vector3 localInterPtProjected = GlobalToLocalVect(interPt, quad.ref);

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

		float distance = Vector3Length(GlobalToLocalPos(interPt, disk.ref));

		if (distance <= disk.radius)
		{
			return true;
		}
	}
	return false;
}

bool IntersectSegmentSphere(Segment seg, Sphere s, float& t1, float& t2, Vector3& interNormal)
{
	Vector3 interPt[2]; // Tableau pour stocker les points d'intersection
	Vector3 OA = Vector3Subtract(seg.pt1, s.ref.origin); // Calcule le vecteur allant de l'origine de la sphère au point de départ du segment
	Vector3 AB = Vector3Subtract(seg.pt2, seg.pt1); // Calcule le vecteur allant du point de départ au point d'arrivée du segment
	float a = Vector3DotProduct(AB, AB); // Calcule a pour l'équation quadratique
	float b = 2 * Vector3DotProduct(AB, OA); // Calcule b pour l'équation quadratique
	float c = Vector3DotProduct(OA, OA) - s.radius * s.radius; // Calcule c pour l'équation quadratique
	float discriminant = b * b - 4 * a * c; // Calcule le discriminant pour déterminer le nombre de solutions possibles
	if (discriminant < 0) // Pas de solutions
		return false;
	else if (discriminant == 0) // Une seule solution
	{
		t1 = -b / (2 * a);
		if (t1 >= 0 && t1 <= 1) // Vérifie si la solution est valide (entre 0 et 1)
		{
			interPt[0] = Vector3Add(seg.pt1, Vector3Scale(AB, t1)); // Calcule le point d'intersection
			interNormal = Vector3Normalize(Vector3Subtract(interPt[0], s.ref.origin)); // Calcule le vecteur normal à la surface de la sphère au point d'intersection
			t2 = NAN; // Initialise t2 car il n'y a qu'une intersection
			MyDrawPolygonSphere({ {interPt[0],QuaternionIdentity()},.1f }, 16, 8, YELLOW);
			DrawLine3D(interPt[0], Vector3Add(Vector3Scale(interNormal, 1), interPt[0]), YELLOW);
			return true;
		}
		else
			return false;
	}
	else // Deux solutions
	{
		float sqrt_discriminant = sqrt(discriminant);
		t1 = (-b + sqrt_discriminant) / (2 * a);
		t2 = (-b - sqrt_discriminant) / (2 * a);
		if (t1 >= 0 && t1 <= 1) // Vérifie si la 1er solution est valide (entre 0 et 1)
		{
			interPt[0] = Vector3Add(seg.pt1, Vector3Scale(AB, t1)); // Calcule le 1er point d'intersection
			interNormal = Vector3Normalize(Vector3Subtract(interPt[0], s.ref.origin)); // Calcule le vecteur normal à la surface de la sphère au point d'intersection
			MyDrawPolygonSphere({ {interPt[0],QuaternionIdentity()},.1f }, 16, 8, YELLOW);
			DrawLine3D(interPt[0], Vector3Add(Vector3Scale(interNormal, 1), interPt[0]), YELLOW);
		}
		if (t2 >= 0 && t2 <= 1) // Vérifie si la 2eme solution est valide (entre 0 et 1)
		{
			interPt[1] = Vector3Add(seg.pt1, Vector3Scale(AB, t2)); // Calcule le 2eme point d'intersection
			interNormal = Vector3Normalize(Vector3Subtract(interPt[1], s.ref.origin)); // Calcule le vecteur normal à la surface de la sphère au point d'intersection
			MyDrawPolygonSphere({ {interPt[1],QuaternionIdentity()},.1f }, 16, 8, GREEN);
			DrawLine3D(interPt[1], Vector3Add(Vector3Scale(interNormal, 1), interPt[1]), GREEN);
		}
		return true;
	}
}

	
//bool IntersectSegmentInfiniteCylinder(Segment segment, InfiniteCylinder cyl, float& t,
//	Vector3& interPt, Vector3& interNormal){}
//
bool IntersectSegmentCylinder(Segment segment, Cylinder cyl, float& t1, float& t2, Vector3& interNormal)
{
	Vector3 interPt_cyl[2]; // Tableau pour stocker les points d'intersection
	Vector3 OA = Vector3Subtract(segment.pt1, cyl.ref.origin); // Calcule le vecteur allant de l'origine de la sphère au point de départ du segment
	Vector3 AB = Vector3Subtract(segment.pt2, segment.pt1); // Calcule le vecteur allant du point de départ au point d'arrivée du segment
	float a = Vector3DotProduct(AB, AB); // Calcule a pour l'équation quadratique
	float b = 2 * Vector3DotProduct(AB, OA); // Calcule b pour l'équation quadratique
	float c = Vector3DotProduct(OA, OA) - cyl.radius * cyl.radius; // Calcule c pour l'équation quadratique
	float discriminant = b * b - 4 * a * c; // Calcule le discriminant pour déterminer le nombre de solutions possibles
	if (discriminant < 0) // Pas de solutions
		return false;
	else if (discriminant == 0) // Une seule solution
	{
		t1 = -b / (2 * a);
		if (t1 >= 0 && t1 <= 1) // Vérifie si la solution est valide (entre 0 et 1)
		{
			interPt_cyl[0] = Vector3Add(segment.pt1, Vector3Scale(AB, t1)); // Calcule le point d'intersection
			interNormal = Vector3Normalize(Vector3Subtract(interPt_cyl[0], cyl.ref.origin)); // Calcule le vecteur normal à la surface de la sphère au point d'intersection
			t2 = NAN; // Initialise t2 car il n'y a qu'une intersection
			MyDrawPolygonSphere({ {interPt_cyl[0],QuaternionIdentity()},.1f }, 16, 8, YELLOW);
			DrawLine3D(interPt_cyl[0], Vector3Add(Vector3Scale(interNormal, 1), interPt_cyl[0]), YELLOW);
			return true;
		}
		else
			return false;
	}
	else // Deux solutions
	{
		float sqrt_discriminant = sqrt(discriminant);
		t1 = (-b + sqrt_discriminant) / (2 * a);
		t2 = (-b - sqrt_discriminant) / (2 * a);
		if (t1 >= 0 && t1 <= 1) // Vérifie si la 1er solution est valide (entre 0 et 1)
		{
			interPt_cyl[0] = Vector3Add(segment.pt1, Vector3Scale(AB, t1)); // Calcule le 1er point d'intersection
			interNormal = Vector3Normalize(Vector3Subtract(interPt_cyl[0], cyl.ref.origin)); // Calcule le vecteur normal à la surface de la sphère au point d'intersection
			MyDrawPolygonSphere({ {interPt_cyl[0],QuaternionIdentity()},.1f }, 16, 8, YELLOW);
			DrawLine3D(interPt_cyl[0], Vector3Add(Vector3Scale(interNormal, 1), interPt_cyl[0]), YELLOW);
		}
		if (t2 >= 0 && t2 <= 1) // Vérifie si la 2eme solution est valide (entre 0 et 1)
		{
			interPt_cyl[1] = Vector3Add(segment.pt1, Vector3Scale(AB, t2)); // Calcule le 2eme point d'intersection
			interNormal = Vector3Normalize(Vector3Subtract(interPt_cyl[1], cyl.ref.origin)); // Calcule le vecteur normal à la surface de la sphère au point d'intersection
			MyDrawPolygonSphere({ {interPt_cyl[1],QuaternionIdentity()},.1f }, 16, 8, GREEN);
			DrawLine3D(interPt_cyl[1], Vector3Add(Vector3Scale(interNormal, 1), interPt_cyl[1]), GREEN);
		}
		return true;
	}
}
//
//bool IntersectSegmentCapsule(Segment seg, Capsule capsule, float& t, Vector3&
//	interPt, Vector3& interNormal){}
//
bool IntersectSegmentBox(Segment seg, Box box, float& t, Vector3& interPt, Vector3& interNormal)
{
	Vector3 size = box.extents;
	ReferenceFrame right = {
		Vector3Add(box.ref.origin,Vector3Scale(box.ref.i,box.extents.x)),
		QuaternionMultiply(box.ref.q, QuaternionFromAxisAngle(
					Vector3Normalize({ 0,0,-1 }), PI / 2))
	};
	Quad qright = { right, {size.y,size.x,size.z} };
	//
	if (IntersectSegmentQuad(seg, qright, t, interPt, interNormal))
	{
		MyDrawPolygonSphere({ {interPt,QuaternionIdentity()},.1f }, 16, 8, RED);
		DrawLine3D(interPt, Vector3Add(Vector3Scale(interNormal, 1), interPt), RED);
		return true;
	}

	ReferenceFrame left = {
		Vector3Add(box.ref.origin,Vector3Scale(box.ref.i,-box.extents.x)),
		QuaternionMultiply(box.ref.q, QuaternionFromAxisAngle(
					Vector3Normalize({ 0,0,1 }), PI / 2))
	};
	Quad qleft = { left, {size.y,size.x,size.z} };
	//
	if (IntersectSegmentQuad(seg, qleft, t, interPt, interNormal))
	{
		MyDrawPolygonSphere({ {interPt,QuaternionIdentity()},.1f }, 16, 8, RED);
		DrawLine3D(interPt, Vector3Add(Vector3Scale(interNormal, 1), interPt), RED);
		return true;
	}

	ReferenceFrame top = {
		Vector3Add(box.ref.origin,Vector3Scale(box.ref.j,box.extents.y)),
		QuaternionMultiply(box.ref.q, QuaternionFromAxisAngle(
					Vector3Normalize({ 0,0,0 }), PI / 2))
	};
	Quad qtop = { top, {size.x,size.y,size.z} };
	//
	if (IntersectSegmentQuad(seg, qtop, t, interPt, interNormal))
	{
		MyDrawPolygonSphere({ {interPt,QuaternionIdentity()},.1f }, 16, 8, RED);
		DrawLine3D(interPt, Vector3Add(Vector3Scale(interNormal, 1), interPt), RED);
		return true;
	}

	ReferenceFrame bottom = {
		Vector3Add(box.ref.origin,Vector3Scale(box.ref.j,-box.extents.y)),
		QuaternionMultiply(box.ref.q, QuaternionFromAxisAngle(
					Vector3Normalize({ 0,0,1 }), PI))
	};
	Quad qbottom = { bottom, {size.x,size.y,size.z} };
	//
	if (IntersectSegmentQuad(seg, qbottom, t, interPt, interNormal))
	{
		MyDrawPolygonSphere({ {interPt,QuaternionIdentity()},.1f }, 16, 8, RED);
		DrawLine3D(interPt, Vector3Add(Vector3Scale(interNormal, 1), interPt), RED);
		return true;
	}

	ReferenceFrame front = {
		Vector3Add(box.ref.origin,Vector3Scale(box.ref.k,-box.extents.z)),
		QuaternionMultiply(box.ref.q, QuaternionFromAxisAngle(
					Vector3Normalize({ -1,0,0 }), PI / 2))
	};
	Quad qfront = { front, {size.x,size.z,size.y} };
	//
	if (IntersectSegmentQuad(seg, qfront, t, interPt, interNormal))
	{
		MyDrawPolygonSphere({ {interPt,QuaternionIdentity()},.1f }, 16, 8, RED);
		DrawLine3D(interPt, Vector3Add(Vector3Scale(interNormal, 1), interPt), RED);
		return true;
	}

	ReferenceFrame rear = {
		Vector3Add(box.ref.origin,Vector3Scale(box.ref.k,box.extents.z)),
		QuaternionMultiply(box.ref.q, QuaternionFromAxisAngle(
					Vector3Normalize({ 1,0,0 }), PI / 2))
	};
	Quad qrear = { rear, {size.x,size.z,size.y} };
	//
	if (IntersectSegmentQuad(seg, qrear, t, interPt, interNormal))
	{
		MyDrawPolygonSphere({ {interPt,QuaternionIdentity()},.1f }, 16, 8, RED);
		DrawLine3D(interPt, Vector3Add(Vector3Scale(interNormal, 1), interPt), RED);
		return true;
	}
	
}


//bool IntersectSegmentRoundedBox(Segment seg, RoundedBox rndBox, float& t, Vector3& interPt, Vector3& interNormal)
//{
//	
//}


bool GetSphereNewPositionAndVelocityIfCollidingWithRoundedBox(
	Sphere sphere,
	Box Box,
	Vector3 velocity,
	float deltaTime,
	float& colT,
	Vector3& colSpherePos,
	Vector3& colNormal,
	Vector3& newPosition,
	Vector3& newVelocity)
{
	return true;
}

typedef struct Gravity
{
	Sphere sphere;
	Vector3 g = { 0, 9.81,0 };
	Vector3 velocity = { 0,0.1,0 };
	Vector3 newVelocity = { 0,0,0 };
	float vitesse=0.3f;
	double m = 0.1;
	double hauteur0 = abs(sphere.ref.origin.y);
	double energie0 = ((m *powf(vitesse,2)) / 2) + (m * (g.y) * hauteur0);
};


Vector3 Newvectorvitesse(Vector3 velocity, float deltaTime,Vector3 g)
{
	velocity = Vector3Add(velocity, Vector3Scale( g, deltaTime));
	return velocity;
}

//Donne la nouvelle vitesse avec la gravite
Vector3 Updatenewvelocity(Sphere sphere,
	Gravity gravity, float vitesse) 
{
	Vector3 dir;
	double acceleration = sqrt(2 * ((gravity.energie0 - (gravity.m * gravity.g.y * gravity.hauteur0)) / gravity.m));
	gravity.vitesse += acceleration;
	dir = Vector3Normalize(gravity.velocity);
	gravity.velocity = Vector3Scale(dir, gravity.vitesse);
	return gravity.velocity;
}


bool GetSphereNewPositionAndVelocityIfCollidingWithBox(
	Sphere sphere,
	Box box,
	Vector3 velocity,
	float deltaTime,
	float& colT,
	Vector3& colSpherePos,
	Vector3& colNormal,
	Vector3& newPosition,
	Vector3& newVelocity)
{
	float t;
	Vector3 interPt;
	Vector3 interNormal;
	Vector3 Future_position = Vector3Add(sphere.ref.origin, Vector3Scale(velocity,-3)); // la future position à la prochaine frame par rapport a sa vitesse
	Segment segment = { sphere.ref.origin , Future_position };// Segment du centre de la sphere a sa future position 

	DrawLine3D(segment.pt1, segment.pt2, BLACK);
	MyDrawPolygonSphere({ {segment.pt1,QuaternionIdentity()},.15f }, 16, 8, RED);
	MyDrawPolygonSphere({ {segment.pt2,QuaternionIdentity()},.15f }, 16, 8, GREEN);

	Line line = { segment.pt1,Vector3Subtract(segment.pt2,segment.pt1) };
	if (IntersectSegmentBox(segment, box, t, interNormal, interPt))
	{
		printf("Collision\n");
	}
	/*velocity = Newvectorvitesse(velocity, deltaTime, g);*/
	return true;
}