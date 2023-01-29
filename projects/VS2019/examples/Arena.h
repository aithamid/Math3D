#pragma once

Quaternion * RandomQuartenions(int n) {
	Quaternion* rot = (Quaternion*) malloc(sizeof(Quaternion) * n);
	for (int i = 0; i < n; i++)
	{
		float x = ((float)rand());
		float y = ((float)rand());
		float z = ((float)rand());

		float theta = ((float)rand());
		rot[i] = QuaternionFromAxisAngle(
			Vector3Normalize({ x,y,z }),
			 (theta *PI)
		);
	}

	return rot;
}

void Draw3DReferential() {
	//3D REFERENTIAL
	DrawGrid(20, 1.0f);        // Draw a grid
	//DrawLine3D({ 0 }, { 0,10,0 }, DARKGRAY);
	//DrawSphere({ 10,0,0 }, .2f, RED);
	//DrawSphere({ 0,10,0 }, .2f, GREEN);
	//DrawSphere({ 0,0,10 }, .2f, BLUE);
}

Box * DrawArena(Quaternion * rot) {
	float s = 40;
	int n = 23;
	n= n+5;
	float h = s/4;

	Box * boxes = (Box *) malloc(n *sizeof(Box));

	// BOX SOL 
	DrawGrid((s*2)-2,1);
	Vector3 size = { s-1,h,1 };
	ReferenceFrame ref = ReferenceFrame(
		{0,-1,0},
		QuaternionFromAxisAngle(
			Vector3Normalize({ 0,0,0 }),
			PI / 4));
	boxes[0] = {ref, {s-1,1,s-1}};
	MyDrawBox(boxes[0], true, true, DARKGREEN);

	// LES BOXS QUI ENTOURE L'ARENE

	float y = size.y;

	float rot1[4] = {1,1,0,0};
	Vector3 bords[4] = {
			{ s,y,0 },
			{ -s,y,0 },
			{ 0,y,s },
			{ 0,y,-s }
	};

	for (int i = 1; i < 5; i++)
	{
		ReferenceFrame ref = ReferenceFrame(
			bords[i-1],
			QuaternionFromAxisAngle(
				Vector3Normalize({ 0,rot1[i-1],0}),
				PI / 2));
		boxes[i] = { ref, size};
		MyDrawBox(boxes[i]);
	}

	// Le reste des boxs
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
	float h_o =  y*2;

	float incliniaison = 0;

	for (int i = 0; i < a; i++)
	{
		for (int j = 0; j < b; j++)
		{

			float x0 = (t_a * (i)) + t_a/2 -s;
			float z0 = (t_b * (j)) + t_b/2 -s;

			if (((i * b) + j) < aplacer) {
				Vector3 size_boxes = {
				t_a / 4,
				3,
				t_b / 2.8
				};
				ReferenceFrame ref = ReferenceFrame(
					{ x0,h_o,z0 }, rot[(i * b) + j]);
				boxes[(i * b) + j + 5 ] = { ref, size_boxes };
			}

		}
	}

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