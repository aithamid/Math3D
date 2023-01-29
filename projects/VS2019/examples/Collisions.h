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
	Vector3 G = { 0, -2.81,0 };
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
    //********************************************

    Physics gravity = { sphere };
    Vector3 endPos = Vector3Subtract(sphere.ref.origin, {0, sphere.radius, 0});
    
    Vector3 v = Vector3Scale(Vector3Normalize(velocity), sphere.radius);
    Vector3 pt1 = Vector3Add(sphere.ref.origin,v);
    
    Segment trajectory = { sphere.ref.origin, pt1 };

    Vector3 v2;
    Vector3 res = velocity;
    Vector3OrthoNormalize(&res, &v2);

    Vector3 velocity_nor2 = Vector3Scale(v2, sphere.radius);
    velocity_nor2 = Vector3Add(sphere.ref.origin, velocity_nor2);

    Segment trajectory2 = { sphere.ref.origin, velocity_nor2 };

    Vector3 v3;
    Vector3 res2 = velocity;
    Vector3OrthoNormalize(&res2, &v3);

    v3 = Vector3Negate(v3);
    Vector3 velocity_nor3 = Vector3Scale(v3, sphere.radius);
    velocity_nor3 = Vector3Add(sphere.ref.origin, velocity_nor3);

    Segment trajectory3 = { sphere.ref.origin, velocity_nor3 };


    Vector3 pt4 = Vector3CrossProduct(v, v3);
    pt4= Vector3Scale(v3, sphere.radius);
    pt4 = Vector3Add(sphere.ref.origin, pt4);
    Segment trajectory4 = { sphere.ref.origin, pt4 };

    // Dessin des points de collisions
    /*DrawLine3D(trajectory.pt1, trajectory.pt2, BLACK);
    DrawSphere(trajectory.pt2, 0.5, RED);
    DrawLine3D(trajectory2.pt1, trajectory2.pt2, BLACK);
    DrawSphere(velocity_nor2, 0.5, RED);
    DrawLine3D(trajectory3.pt1, trajectory3.pt2, BLACK);
    DrawSphere(velocity_nor3, 0.5, RED);*/
    /*DrawLine3D(trajectory4.pt1, trajectory4.pt2, BLACK);
    DrawSphere(pt4, 0.5, RED);*/

    //********************************************
    if (IntersectSegmentBox(trajectory, box, colT, colSpherePos, colNormal))
    {
        newVelocity = Vector3Reflect(velocity, colNormal);
        return true;
    }
    //********************************************
    

    else {
        if (IntersectSegmentBox(trajectory2, box, colT, colSpherePos, colNormal))
        {
            newVelocity = Vector3Reflect(velocity, colNormal);
            return true;
        }
        ////*****************************************************************************************
        else {

            if (IntersectSegmentBox(trajectory3, box, colT, colSpherePos, colNormal))
            {

                newVelocity = Vector3Reflect(velocity, colNormal);
                return true;
            }
            // 
            //*******************************************************************************************
            else
            {
                return false;
            }
        }
    }
}



bool GetSphereNewPositionAndVelocityIfCollidingWithRoundedBoxes(
    Sphere sphere,
    Box * boxes,
    Vector3 velocity,
    float deltaTime,
    float& colT,
    Vector3& colSpherePos,
    Vector3& colNormal,
    Vector3& newPosition,
    Vector3& newVelocity)
{
    for (int i = 0; i < sizeof(boxes); i++)
    {
        if (GetSphereNewPositionAndVelocityIfCollidingWithBox(sphere, boxes[i], velocity, deltaTime, colT, colSpherePos, colNormal, newPosition, newVelocity))
        {
            sphere.ref.origin = Vector3Add(sphere.ref.origin, velocity);
            return true;
        }
    
        /*if (sphere.ref.origin.y >= 0)
            sphere.ref.origin = Vector3Add(sphere.ref.origin, velocity);*/
    }
    return false;
}


