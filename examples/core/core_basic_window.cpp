/*******************************************************************************************
*
*   raylib [core] example - Basic window
*
*   Welcome to raylib!
*
*   To test examples, just press F6 and execute raylib_compile_execute script
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   This example has been created using raylib 1.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2014 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
//#include "My3DPrimitives.h"
#include <raymath.h>
#include "rlgl.h"
#include <math.h>
#include <float.h>
#include <vector>
#include <ctime>

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION            330
#else   // PLATFORM_RPI, PLATFORM_ANDROID, PLATFORM_WEB
#define GLSL_VERSION            100
#endif

#define EPSILON 1.e-6f
#include "My3DPrimitives.h"
#include "Intersections.h"

template <typename T> int sgn(T val) {
	return (T(0) < val) - (val < T(0));
}

struct Cylindrical {
	float rho;
	float theta;
	float y;
};

struct Spherical {
	float rho;
	float theta;
	float phi;
};

Vector3 CylindricalToCartesian(Cylindrical cyl) {
	return { cyl.rho * sinf(cyl.theta), cyl.y, cyl.rho * cosf(cyl.theta) };
}

Cylindrical CartesianToCylindrical(Vector3 cart) {
	Cylindrical cyl = {
		sqrt((double)powf(cart.x, 2) + (double)powf(cart.z, 2)),
		atan2(cart.x, cart.z),
		cart.y
	};

	if (cyl.theta <= 0)
		cyl.theta += 2 * PI;
	return cyl;
}

Vector3 SphericalToCartesian(Spherical sph) {
	return {
		sph.rho * sinf(sph.phi) * sinf(sph.theta),
		sph.rho * cosf(sph.phi),
		sph.rho * sinf(sph.phi) * cosf(sph.theta)
	};
}

Vector3 SphericalToCartesian(Spherical* sph) {
	return {
		sph->rho * sinf(sph->phi) * sinf(sph->theta),
		sph->rho * cosf(sph->phi),
		sph->rho * sinf(sph->phi) * cosf(sph->theta)
	};
}

Spherical CartesianToSpherical(Vector3 cart) {
	Spherical sph = {
		sqrt((double)pow(cart.x, 2) + (double)pow(cart.y, 2) + (double)pow(cart.z, 2)),
		atan2(cart.x, cart.z),
		acosf(cart.y / sph.rho)
	};

	if (sph.theta <= 0)
		sph.theta += 2 * PI;
	return sph;
}


Vector2 MyUpdateOrbitalCamera(Camera3D* Camera, Spherical* coordonnes, Vector2 prevMousePos)
{	
	Vector2 newMousePos = GetMousePosition();
	float rotSpeed = 0.25f; // to scale the mouse input
	float moveSpeed = 3.0f; // to scale the linear input

	float rhoMin = 0;
	float rhoMax = 40;

	float phiMin = -89;
	float phiMax = 89;
	Vector2 difference = Vector2Subtract(newMousePos, prevMousePos);
	if (IsMouseButtonDown(1)) // only rotate on right click
	{	
		
		// update the angles from the delta
		coordonnes->theta += (difference.x) * rotSpeed;
		coordonnes->phi += (difference.y) * rotSpeed;

		// clamp the tilt so we don't get gymbal lock
		if (coordonnes->phi > phiMax)
			coordonnes->phi = phiMax;
		if (coordonnes->phi < phiMin)
			coordonnes->phi = phiMin;
		DrawText(TextFormat("bouton droit"), 400, 50, 20, RED);
	}
	DrawText(TextFormat("Mouse: %.1f, %.1f", newMousePos.x, newMousePos.y), 400, 10, 20, RED);
	DrawText(TextFormat("Diffe: %.1f, %.1f", difference.x, difference.y), 400, 30, 20, RED);
	// always update the position so we don't get jumps


	// vector in rotation space to move

	// update zoom
	coordonnes->rho -= 2*GetMouseWheelMove();
	if (coordonnes->rho < rhoMin)
		coordonnes->rho = rhoMin;

	if (coordonnes->rho < rhoMax)
		coordonnes->rho = rhoMax;

	// vector we are going to transform to get the camera offset from the target point
	Vector3 camPos = { 0, 0, coordonnes->rho };

	Matrix thetaMatrix = MatrixRotateX(coordonnes->phi * GetFrameTime()); // a matrix for the tilt rotation
	Matrix phiMatrix = MatrixRotateY(coordonnes->theta * GetFrameTime()); // a matrix for the plane rotation
	Matrix mat = MatrixMultiply(thetaMatrix, phiMatrix); // the combined transformation matrix for the camera position

	//camPos = SphericalToCartesian(coordonnes);

	camPos = Vector3Transform(camPos, mat); // transform the camera position into a vector in world space

	Camera->position = Vector3Add(Camera->target, camPos); // offset the camera position by the vector from the target position
	return newMousePos;
	
}

int main(int argc, char* argv[])
{
	// Initialization
	//--------------------------------------------------------------------------------------
	float screenSizeCoef = .9f;
	const int screenWidth = 1920 * screenSizeCoef;
	const int screenHeight = 1080 * screenSizeCoef;

	Vector2 mouse = { 0,0 };
	static Vector2 ancienne_mouse = { 0,0 };
	Vector2 difference;// = { 0,0 };

	InitWindow(screenWidth, screenHeight, "ESIEE - E3FI - 2022 - 2023 -Maths 3D");

	SetTargetFPS(60);
	Camera3D Camera = {0};
	Vector3 cameraPos = { 8.0f, 15.0f, 14.0f };
	Camera.fovy = 45.0f;
	Camera.target = Vector3{ 1, 0 ,0 };
	Camera.position = cameraPos;
	Camera.up = { 0.0f, 1.0f, 0.0f };
	Camera.type = CAMERA_PERSPECTIVE;
	Spherical sph;
	ReferenceFrame ref = ReferenceFrame(
		{ 1,3,-2 },
		QuaternionFromAxisAngle(
			Vector3Normalize({ 0,0,0 }),
			PI / 4));
	//Sphere sphere = { ref, 3 };
	//sphere.ref.origin = { -10,5,-5 };
	sph.rho = 25;  // how far away from the target the camera is (radius)
	sph.theta = 45; // the rotation angle around the target  (around Y)
	sph.phi = 45; // the tilt tangle of the camera (up/down)
	float vitessesphere = 0.0;
	Quaternion qROt = { 0,1,0,0 };
	float time = (float)GetTime();

	ReferenceFrame refsphere;
	refsphere.origin = { 0,10,0 };
	Sphere sphere = { refsphere,1 };

	ReferenceFrame refbox;
	Box box = { refbox,{5,1,5} };

	Vector3 g = { 0,9.81,0 };
	Vector3 velocity = { 0,0.1,0 };
	Vector3 newVelocity = { 0,0,0 };
	double m = 0.1;
	double hauteur0;
	double energie0;
	Gravity gravity = { sphere,box };



	Vector2 cursorPos = GetMousePosition(); // save off current position so we have a start point

	//--------------------------------------------------------------------------------------
	
	hauteur0 = abs(sphere.ref.origin.y-sphere.radius - (box.ref.origin.y+box.extents.y));
	energie0 = ((m * (velocity.y * velocity.y)) / 2) + m * (g.y) * hauteur0;

	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		// Update
		//----------------------------------------------------------------------------------
		// TODO: Update your variables here
		//----------------------------------------------------------------------------------

		/*float deltaTime = GetFrameTime();
		float time = (float)GetTime();
		*/
		cursorPos = MyUpdateOrbitalCamera(&Camera, &sph, cursorPos);
		
		
		BeginDrawing();
		ClearBackground(RAYWHITE);


		// Fonction coordonnées



		
		BeginMode3D(Camera);
		{				
			//3D REFERENTIAL
			DrawGrid(20, 1.0f);        // Draw a grid
			DrawLine3D({ 0 }, { 0,10,0 }, DARKGRAY);
			DrawSphere({ 10,0,0 }, .2f, RED);
			DrawSphere({ 0,10,0 }, .2f, GREEN);
			DrawSphere({ 0,0,10 }, .2f, BLUE);


			//TESTS INTERSECTIONS
			Vector3 interPt;
			Vector3 interNormal;
			float t;
			float t2;
			time = (double)GetTime();

			
			//float vitesse = 0.01 * gravity.y;


			MyDrawBox(box,false,true);


			MyDrawSphere(sphere, 10,10);

			if (sphere.ref.origin.y-sphere.radius <= box.ref.origin.y+box.extents.y)
			{
				velocity.y = -velocity.y;
			}
			sphere.ref.origin.y -= 0.5* (double)velocity.y;
			
			
			

			printf("velocity : %f\n", velocity.y);
			velocity.y += Updatenewvelocity(sphere, box, gravity);
			//printf("energie: %f\n", energie0);
			printf("new velocity : %f\n", velocity.y);

			//THE SEGMENT
			Segment segment = { {box.ref.origin},{box.ref.origin.x,box.ref.origin.y+box.extents.y,box.ref.origin.z} };
			//DrawLine3D(segment.pt1, segment.pt2, BLACK);
			//MyDrawPolygonSphere({ {segment.pt1,QuaternionIdentity()},.15f }, 16, 8, RED);
			//MyDrawPolygonSphere({ {segment.pt2,QuaternionIdentity()},.15f }, 16, 8, GREEN);


			Segment segment2 = { {box.ref.origin.x-10,box.ref.origin.y + box.extents.y,box.ref.origin.z},{box.ref.origin.x,box.ref.origin.y + box.extents.y,box.ref.origin.z} };
			DrawLine3D(segment2.pt1, segment2.pt2, BLACK);
			MyDrawPolygonSphere({ {segment2.pt1,QuaternionIdentity()},.15f }, 16, 8, RED);
			MyDrawPolygonSphere({ {segment2.pt2,QuaternionIdentity()},.15f }, 16, 8, GREEN);
			// TEST LINE PLANE INTERSECTION
			Plane plane = { Vector3RotateByQuaternion({0,1,0}, QuaternionFromAxisAngle({1,0,0},time* .5f)), 2 };
			ReferenceFrame refQuad = { Vector3Scale(plane.normal, plane.d),
			QuaternionFromVector3ToVector3({0,1,0},plane.normal) };

			Line line = { segment.pt1,Vector3Subtract(segment.pt2,segment.pt1) };
			if (IntersectSegmentSphere(segment2, sphere, t, t2, interNormal))
			{
				//vitesse = -vitesse;
			//	MyDrawPolygonSphere({ {interPt,QuaternionIdentity()},.1f }, 16, 8, RED);
			//	DrawLine3D(interPt, Vector3Add(Vector3Scale(interNormal, 1), interPt), RED);
			}
			

		}
		EndMode3D();

		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------   
	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}