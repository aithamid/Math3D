#pragma once

Quaternion* RandomQuartenions(int n) {
	Quaternion* rot = (Quaternion*)malloc(sizeof(Quaternion) * n);
	for (int i = 0; i < n; i++)
	{
		float x = ((float)rand());
		float y = ((float)rand());
		float z = ((float)rand());

		float theta = ((float)rand());
		rot[i] = QuaternionFromAxisAngle(
			Vector3Normalize({ x,y,z }),
			(theta * PI)
		);
	}

	return rot;
}


// FONCTION POUR TRACER L'ARENE 


Box* DrawArena(Quaternion* rot) {
	float s = 80;
	int n = 3;
	n = n + 5;
	float h = s / 4;

	Box* boxes = (Box*)malloc(n * sizeof(Box));
	// **********************************************
	// ON DESSINE LA BOX BOX SOL 

	Vector3 size = { s - 1,h,1 };
	ReferenceFrame ref = ReferenceFrame(
		{ 0,-1,0 },
		QuaternionFromAxisAngle(
			Vector3Normalize({ 0,0,0 }),
			PI / 4));
	boxes[0] = { ref, {s - 1,1,s - 1} };
	MyDrawBox(boxes[0], true, true, DARKGREEN);

	// **********************************************


	// LES BOXS QUI ENTOURE L'ARENE

	float y = size.y;

	float rot1[4] = { 1,1,0,0 };
	Vector3 bords[4] = {
			{ s,y,0 },
			{ -s,y,0 },
			{ 0,y,s },
			{ 0,y,-s }
	};

	for (int i = 1; i < 5; i++)
	{
		ReferenceFrame ref = ReferenceFrame(
			bords[i - 1],
			QuaternionFromAxisAngle(
				Vector3Normalize({ 0,rot1[i - 1],0 }),
				PI / 2));
		boxes[i] = { ref, size };
	}

	// **********************************************

	// SOME MATHS
	//
	int aplacer = n - 5;
	int a = sqrt(aplacer);
	int b = aplacer / a;
	if (aplacer % a != 0)
		b++;

	float s_original = s;
	s = 0.8 * s_original;
	float t_a = ((s * 2) / a + 1);
	float t_b = ((s * 2) / b + 1);
	//


	// OBSTACLES

	// hauteur des obstacles
	float h_o = y * 2;

	float incliniaison = 0;

	float placement = -70;
	for (int i = 5; i < 7; i++)
	{
		ReferenceFrame ref;
		/*ref.origin = {placement,20.0f,placement/2 };*/
		boxes[i] = { ref, {80,10,5 } };
		//MyDrawBox(boxes[i], true, true , LIGHTGRAY);
		//placement += (10.0f * i);
	}
	boxes[5].ref = ReferenceFrame({ 0,-1,0 },
		QuaternionFromAxisAngle(
			Vector3Normalize({ 1,0,0 }), 3 * PI / 4));
	boxes[5].ref.origin = { 0.0f, 10.0f, -30.0f };


	boxes[6].ref = ReferenceFrame({ 0,-1,0 },
		QuaternionFromAxisAngle(
			Vector3Normalize({ 1,0,0 }), PI / 4));
	boxes[6].ref.origin = { 0.0f, 10.0f, 30.0f };


	// DRAW BOXES

	for (int i = 1; i < n; i++)
	{
		Color color = LIGHTGRAY;
		if (i >= 5)
		{
			color = GREEN;
		}
		MyDrawBox(boxes[i], true, true, color);
	}

	return boxes;

}