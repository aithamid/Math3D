#pragma once

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
	coordonnes->rho -= 2 * GetMouseWheelMove();
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