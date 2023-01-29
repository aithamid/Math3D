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

typedef struct Physics
{
	Sphere sphere;
	Vector3 G = { 0, -0.81,0 };
	float masse=0.01;
	float velocity_0=0.01;
	float hauteur = sphere.ref.origin.y;
	float energie_y_0 = ((masse * powf(velocity_0,2)) / 2) + masse * G.y * hauteur;
	float energie_potentielle = masse * G.y * hauteur;
};

//Calcule une acceleration , donc un ajout sur la vitesse de base
Vector3 UpdateGravityVelocity(Physics p, float vitesse, Vector3 velocity, float deltaTime)
{
	Vector3 dir;
	velocity.y += sqrt(2 * ((p.energie_y_0 - p.energie_potentielle) / p.masse));
	//dir = Vector3Normalize(velocity);
	//velocity = Vector3Scale(dir, vitesse);
	velocity = Vector3Add(velocity, Vector3Scale(p.G, deltaTime));
	return velocity;
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
    Physics gravity = { sphere };
    Vector3 endPos = Vector3Subtract(sphere.ref.origin, {0, sphere.radius, 0});
    Vector3 velocity_nor = Vector3Scale(Vector3Normalize(velocity), sphere.radius);
    Vector3 origin_radius = { sphere.ref.origin.x + sphere.radius,sphere.ref.origin.y + sphere.radius,sphere.ref.origin.z + sphere.radius };
    velocity_nor = Vector3Add(sphere.ref.origin,velocity_nor);
    Segment trajectory = { sphere.ref.origin, velocity_nor };

    DrawLine3D(trajectory.pt1, trajectory.pt2, BLACK);
    DrawSphere(velocity_nor, 0.5, RED);

    Vector3 interPt, interNormal;
    
    if (IntersectSegmentBox(trajectory, box, colT, interPt, interNormal))
    {
        colSpherePos = Vector3Lerp(sphere.ref.origin, endPos, colT);
        colNormal = interNormal;
        newPosition = Vector3Reflect(Vector3Subtract(colSpherePos, sphere.ref.origin), colNormal);
        newPosition = Vector3Add(sphere.ref.origin, newPosition);
        newVelocity = Vector3Reflect(velocity, colNormal);
        printf("New velocity y : %f\n", newVelocity.y);
        printf("New velocity x : %f\n", newVelocity.x);
        if (newVelocity.y <= 0.15)
        {
            newVelocity.y += 0.005;
        }
        else if (newVelocity.y >= 0.3)
        {
            newVelocity.y -= 0.005;
        }
        return true;
    }
    else
    {
        return false;
    }
}


