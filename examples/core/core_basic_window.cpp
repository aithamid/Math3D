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
#include "Collisions.h"
#include "Arena.h"
#include "OrbitalCamera.h"


template <typename T> int sgn(T val) {
	return (T(0) < val) - (val < T(0));
}



int main(int argc, char* argv[])
{
	// CAMERA

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
	//--------------------------------------------------------------------------------------

	// END CAMERA
	
	float vitessesphere = 0.0;
	Quaternion qROt = { 0,1,0,0 };
	float deltaTime = GetFrameTime();
	float time = (float)GetTime();

	ReferenceFrame refsphere;
	refsphere.origin = { 3,20,2 };
	Sphere sphere = { refsphere,1 };

	ReferenceFrame refbox;
	Box box = { refbox,{5,1,5} };

	Vector3 g = { 0,9.81,0 };
	
	Vector3 newVelocity = { 0,0,0 };
	double m = 0.01;


	Physics gravity = { sphere };
	float colT;
	Vector3 colSpherePos;
	Vector3 colNormal;
	Vector3 newPosition;
	Vector3 velocity = { 0.1,0.1,0 };


	Vector2 cursorPos = GetMousePosition(); // save off current position so we have a start point

	//--------------------------------------------------------------------------------------

	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		// Update
		//----------------------------------------------------------------------------------
		// TODO: Update your variables here
		//----------------------------------------------------------------------------------

		cursorPos = MyUpdateOrbitalCamera(&Camera, &sph, cursorPos);
		
		
		BeginDrawing();
		ClearBackground(RAYWHITE);


		// Fonction coordonnées



		
		BeginMode3D(Camera);
		{
			int n = 5;

			int ntotalboxes = n + 5;
			//Box* boxes = DrawArena(n, 20);
			

			Draw3DReferential();
			int nbBox =12;
			//Box * boxes = DrawArena(nbBox,40);
			//TESTS INTERSECTIONS
			Vector3 interPt;
			Vector3 interNormal;
			float t;
			float t2;
			time = (double)GetTime();
			deltaTime = GetFrameTime();
			
			//if (sphere.ref.origin.y >= 0)
			//{
				velocity = UpdateGravityVelocity(gravity, 0.6f, velocity, deltaTime);
			//}

			MyDrawBox(box,false,true);
			
			MyDrawSphere(sphere, 10, 10, false);

			printf("velocity : %f\n", velocity.y);
			
			Segment segment = { {sphere.ref.origin},{sphere.ref.origin.x,sphere.ref.origin.y-sphere.radius,sphere.ref.origin.z} };

			
			Line line = { segment.pt1,Vector3Subtract(segment.pt2,segment.pt1) };
			
			//
			GetSphereNewPositionAndVelocityIfCollidingWithBox(sphere, box, velocity, deltaTime, colT, colSpherePos, colNormal, newPosition, velocity);
			//
			//if (sphere.ref.origin.y >= sphere.radius)
			//{
				sphere.ref.origin = Vector3Add(sphere.ref.origin, velocity);
			//}
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