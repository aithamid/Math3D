#pragma once


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

typedef struct Object
{
	Vector3 position;
	Vector3 velocity;
	Vector3 force;
	float mass = 2;
};

typedef struct Physics
{
	Sphere sphere;
	Vector3 g = { 0, 9.81,0 };
	Vector3 velocity = { 0,0.1,0 };
	Vector3 newVelocity = { 0,0,0 };
	float vitesse;
	double m = 0.1;
	double hauteur0 = abs(sphere.ref.origin.y);
	double energie0 = ((m * powf(vitesse, 2)) / 2) + (m * (g.y) * hauteur0);
};


Vector3 Newvectorvitesse(Vector3 velocity, float deltaTime, Vector3 g)
{
	velocity = Vector3Add(velocity, Vector3Scale(g, deltaTime));
	return velocity;
}

//Donne la nouvelle vitesse avec la gravite
Vector3 Updatenewvelocity(Sphere sphere,
	Physics gravity, float vitesse)
{
	gravity.vitesse = vitesse;
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
	bool colision = false;
	Physics g = { sphere };
	float t;
	Vector3 interPt;
	Vector3 interNormal;
	Vector3 Future_position = Vector3Add(sphere.ref.origin, Vector3Scale(velocity, -10)); // la future position à la prochaine frame par rapport a sa vitesse
	Segment segment = { sphere.ref.origin , Future_position };// Segment du centre de la sphere a sa future position 

	DrawLine3D(segment.pt1, segment.pt2, BLACK);
	MyDrawPolygonSphere({ {segment.pt1,QuaternionIdentity()},.15f }, 16, 8, RED);
	MyDrawPolygonSphere({ {segment.pt2,QuaternionIdentity()},.15f }, 16, 8, GREEN);

	Line line = { segment.pt1,Vector3Subtract(segment.pt2,segment.pt1) };
	if (IntersectSegmentBox(segment, box, t, interPt, interNormal)) // interPt le point d intersection et interNormal le vecteur normal
	{
		printf("  Inter pt : x : %f y : %f z : %f\n", interPt.x, interPt.y, interPt.z);
		printf(" Inter normal : x : %f y : %f z : %f\n", interNormal.x, interNormal.y, interNormal.z);
		newVelocity = Vector3Scale(velocity, -1);
		printf("  New velocity : y : %f\n", newVelocity.y);
		colision = true;
	}
	if (colision)
	{
		newVelocity = Vector3Negate(velocity);
		return true;
	}

	/*velocity = Newvectorvitesse(velocity, deltaTime, g);*/
	return false;
}