#define MY3DPRIMITIVES_H
#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <cstdlib>

using namespace std;
struct ReferenceFrame {
	Vector3 origin;
	Vector3 i, j, k;
	Quaternion q;
	ReferenceFrame()
	{
		origin = { 0,0,0 };
		i = { 1,0,0 };
		j = { 0,1,0 };
		k = { 0,0,1 };
		q = QuaternionIdentity();
	}
	ReferenceFrame(Vector3 origin, Quaternion q)
	{
		this->q = q;
		this->origin = origin;
		i = Vector3RotateByQuaternion({ 1,0,0 }, q);
		j = Vector3RotateByQuaternion({ 0,1,0 }, q);
		k = Vector3RotateByQuaternion({ 0,0,1 }, q);
	}
	void Translate(Vector3 vect)
	{
		this->origin = Vector3Add(this->origin, vect);
	}
	void RotateByQuaternion(Quaternion qRot)
	{
		q = QuaternionMultiply(qRot, q);
		i = Vector3RotateByQuaternion({ 1,0,0 }, q);
		j = Vector3RotateByQuaternion({ 0,1,0 }, q);
		k = Vector3RotateByQuaternion({ 0,0,1 }, q);
	}
};


typedef struct Quad {
	ReferenceFrame ref;
	Vector3 extents;

} Quad;

typedef struct Disk {
	ReferenceFrame ref;
	float radius;
} Disk;

typedef struct Box {
	ReferenceFrame ref;
	Vector3 extents;
} Box;


typedef struct Sphere {
	ReferenceFrame ref;
	float radius;
} Sphere;

typedef struct InfiniteCylinder {
	ReferenceFrame ref;
	float radius;
} InfiniteCylinder;

typedef struct Cylinder {
	ReferenceFrame ref;
	float halfHeight;
	float radius;
} Cylinder;

typedef struct Capsule {
	ReferenceFrame ref;
	float halfHeight;
	float radius;
} Capsule;

typedef struct RoundedBox {
	ReferenceFrame ref;
	Vector3 extents;
	float radius;
} RoundedBox;



void MyDrawPolygonQuad(Quad quad, Color color = LIGHTGRAY)
{
    int numVertex = 6;
    if (rlCheckBufferLimit(numVertex)) rlglDraw();
    // BEGINNING OF SPACE TRANSFORMATION INDUCED BY THE LOCAL REFERENCE FRAME
    // methods should be called in this order: rlTranslatef, rlRotatef & rlScalef
    // so that transformations occur in the opposite order: scale, then rotation, then translation
    rlPushMatrix();
    //TRANSLATION
    rlTranslatef(quad.ref.origin.x, quad.ref.origin.y, quad.ref.origin.z);
    //ROTATION
    Vector3 vect;
    float angle;
    QuaternionToAxisAngle(quad.ref.q, &vect, &angle);
    rlRotatef(angle * RAD2DEG, vect.x, vect.y, vect.z);
    //SCALING
    rlScalef(quad.extents.x, 1, quad.extents.z);
    // END OF SPACE TRANSFORMATION INDUCED BY THE LOCAL REFERENCE FRAME
    rlBegin(RL_TRIANGLES);
    rlColor4ub(color.r, color.g, color.b, color.a);
    rlVertex3f(1, 0, 1);
    rlVertex3f(1, 0, -1);
    rlVertex3f(-1, 0, -1);
    rlVertex3f(1, 0, 1);
    rlVertex3f(-1, 0, -1);
    rlVertex3f(-1, 0, 1);
    rlEnd();
    //EVERY rlPushMatrix method call should be followed by a rlPopMatrix method call
    rlPopMatrix();
}

void MyDrawWireframeQuad(Quad quad, Color color = DARKGRAY)
{
    int numVertex = 10;
    if (rlCheckBufferLimit(numVertex)) rlglDraw();
    rlPushMatrix();
    rlTranslatef(quad.ref.origin.x, quad.ref.origin.y, quad.ref.origin.z);
    Vector3 vect;
    float angle;
    QuaternionToAxisAngle(quad.ref.q, &vect, &angle);
    rlRotatef(angle * RAD2DEG, vect.x, vect.y, vect.z);
    rlScalef(quad.extents.x, 1, quad.extents.z);
    rlBegin(RL_LINES);
    rlColor4ub(color.r, color.g, color.b, color.a);
    rlVertex3f(1, 0, 1);
    rlVertex3f(1, 0, -1);

    rlVertex3f(1, 0, -1);
    rlVertex3f(-1, 0, -1);

    

    rlVertex3f(-1, 0, -1);
    rlVertex3f(-1, 0, 1);

    rlVertex3f(-1, 0, 1);
    rlVertex3f(1, 0, 1);
    rlEnd();
    rlPopMatrix();
}


void MyDrawQuad(Quad quad, bool drawPolygon = true, bool drawWireframe = true,
    Color polygonColor = LIGHTGRAY, Color wireframeColor = DARKGRAY)
{
    if (drawPolygon) MyDrawPolygonQuad(quad, polygonColor);
    if (drawWireframe)MyDrawWireframeQuad(quad, wireframeColor);
}

void MyDrawPolygonDisk(Disk disk, int nSectors, Color color = LIGHTGRAY)
{
    float theta = 0;

    for (int i = 0; i < nSectors; i++)
    {
        int numVertex = 3;
        if (rlCheckBufferLimit(numVertex)) rlglDraw();
        rlPushMatrix();
        rlTranslatef(disk.ref.origin.x, disk.ref.origin.y, disk.ref.origin.z);
        Vector3 vect;
        float angle;
        QuaternionToAxisAngle(disk.ref.q, &vect, &angle);
        rlRotatef(angle * RAD2DEG, vect.x, vect.y, vect.z);
        rlScalef(disk.radius, 1, disk.radius);
        
        rlBegin(RL_TRIANGLES);
        rlColor4ub(color.r, color.g, color.b, color.a);
        rlVertex3f(0, 0, 0);
        rlVertex3f(sin(theta), 0, cos(theta));
        rlVertex3f(sin(theta + (2 * PI) / nSectors), 0, cos(theta + (2 * PI) / nSectors));

        rlEnd();
        rlPopMatrix();

        theta += (2 * PI) / nSectors;
    }
}
void MyDrawWireframeDisk(Disk disk, int nSectors, Color color = DARKGRAY)
{
    float theta = 0;

    for (int i = 0; i < nSectors; i++)
    {
        int numVertex = 7;
        if (rlCheckBufferLimit(numVertex)) rlglDraw();
        rlPushMatrix();
        rlTranslatef(disk.ref.origin.x, disk.ref.origin.y, disk.ref.origin.z);
        Vector3 vect;
        float angle;
        QuaternionToAxisAngle(disk.ref.q, &vect, &angle);
        rlRotatef(angle * RAD2DEG, vect.x, vect.y, vect.z);
        rlScalef(disk.radius, 1, disk.radius);


        rlBegin(RL_LINES);
        rlColor4ub(color.r, color.g, color.b, color.a);
        rlVertex3f(0, 0, 0);
        rlVertex3f(sin(theta), 0, cos(theta));
        rlVertex3f(sin(theta), 0, cos(theta));
        rlVertex3f(sin(theta + (2 * PI) / nSectors), 0, cos(theta + (2 * PI) / nSectors));
        rlEnd();
        rlPopMatrix();

        theta += (2 * PI) / nSectors;
    }
}

void MyDrawDisk(Disk disk, int nSectors, bool drawPolygon = true, bool
    drawWireframe = true, Color polygonColor = LIGHTGRAY, Color wireframeColor =
    DARKGRAY)
{
    if (drawPolygon) MyDrawPolygonDisk(disk, nSectors, polygonColor);
    if (drawWireframe)MyDrawWireframeDisk(disk, nSectors, wireframeColor);
}


//BOX
void MyDrawWireframeBox(Box box, Color color = DARKGRAY)
{
    // UNE BOX EST COMPOSÉ DE 6 COTÉS 

    //  On dessise 6 quads 


    Vector3 size = box.extents;
    ReferenceFrame right = {
        Vector3Add(box.ref.origin,Vector3Scale(box.ref.i,box.extents.x)),
        QuaternionMultiply(box.ref.q, QuaternionFromAxisAngle(
                    Vector3Normalize({ 0,0,-1 }), PI / 2))
    };
    Quad qright = { right, {size.y,size.x,size.z} };
    MyDrawWireframeQuad(qright, color);

    ReferenceFrame left = {
        Vector3Add(box.ref.origin,Vector3Scale(box.ref.i,-box.extents.x)),
        QuaternionMultiply(box.ref.q, QuaternionFromAxisAngle(
                    Vector3Normalize({ 0,0,1 }), PI / 2))
    };
    Quad qleft = { left, {size.y,size.x,size.z} };
    MyDrawWireframeQuad(qleft, color);

    ReferenceFrame top = {
        Vector3Add(box.ref.origin,Vector3Scale(box.ref.j,box.extents.y)),
        QuaternionMultiply(box.ref.q, QuaternionFromAxisAngle(
                    Vector3Normalize({ 0,0,0 }), PI / 2))
    };
    Quad qtop = { top, {size.x,size.y,size.z} };
    MyDrawWireframeQuad(qtop, color);

    ReferenceFrame bottom = {
        Vector3Add(box.ref.origin,Vector3Scale(box.ref.j,-box.extents.y)),
        QuaternionMultiply(box.ref.q, QuaternionFromAxisAngle(
                    Vector3Normalize({ 0,0,1 }), PI))
    };
    Quad qbottom = { bottom, {size.x,size.y,size.z} };
    MyDrawWireframeQuad(qbottom, color);

    ReferenceFrame front = {
        Vector3Add(box.ref.origin,Vector3Scale(box.ref.k,-box.extents.z)),
        QuaternionMultiply(box.ref.q, QuaternionFromAxisAngle(
                    Vector3Normalize({ -1,0,0 }), PI / 2))
    };
    Quad qfront = { front, {size.x,size.z,size.y} };
    MyDrawWireframeQuad(qfront, color);

    ReferenceFrame rear = {
        Vector3Add(box.ref.origin,Vector3Scale(box.ref.k,box.extents.z)),
        QuaternionMultiply(box.ref.q, QuaternionFromAxisAngle(
                    Vector3Normalize({ 1,0,0 }), PI / 2))
    };
    Quad qrear = { rear, {size.x,size.z,size.y} };
    MyDrawWireframeQuad(qrear, color);
}

void MyDrawPolygonBox(Box box, Color color = LIGHTGRAY)
{
    // UNE BOX EST COMPOSÉ DE 6 COTÉS 

    //  On dessise 6 quads 

    Vector3 size = box.extents;
    ReferenceFrame right = {
        Vector3Add(box.ref.origin,Vector3Scale(box.ref.i,box.extents.x)),
        QuaternionMultiply(box.ref.q, QuaternionFromAxisAngle(
                    Vector3Normalize({ 0,0,-1 }), PI / 2))
    };
    Quad qright = { right, {size.y,size.x,size.z} };
    MyDrawPolygonQuad(qright, color);

    ReferenceFrame left = {
        Vector3Add(box.ref.origin,Vector3Scale(box.ref.i,-box.extents.x)),
        QuaternionMultiply(box.ref.q, QuaternionFromAxisAngle(
                    Vector3Normalize({ 0,0,1 }), PI / 2))
    };
    Quad qleft = { left, {size.y,size.x,size.z} };
    MyDrawPolygonQuad(qleft, color);

    ReferenceFrame top = {
        Vector3Add(box.ref.origin,Vector3Scale(box.ref.j,box.extents.y)),
        QuaternionMultiply(box.ref.q, QuaternionFromAxisAngle(
                    Vector3Normalize({ 0,0,0 }), PI / 2))
    };
    Quad qtop = { top, {size.x,size.y,size.z} };
    MyDrawPolygonQuad(qtop, color);

    ReferenceFrame bottom = {
        Vector3Add(box.ref.origin,Vector3Scale(box.ref.j,-box.extents.y)),
        QuaternionMultiply(box.ref.q, QuaternionFromAxisAngle(
                    Vector3Normalize({ 0,0,1 }), PI))
    };
    Quad qbottom = { bottom, {size.x,size.y,size.z} };
    MyDrawPolygonQuad(qbottom, color);

    ReferenceFrame front = {
        Vector3Add(box.ref.origin,Vector3Scale(box.ref.k,-box.extents.z)),
        QuaternionMultiply(box.ref.q, QuaternionFromAxisAngle(
                    Vector3Normalize({ -1,0,0 }), PI / 2))
    };
    Quad qfront = { front, {size.x,size.z,size.y} };
    MyDrawPolygonQuad(qfront, color);

    ReferenceFrame rear = {
        Vector3Add(box.ref.origin,Vector3Scale(box.ref.k,box.extents.z)),
        QuaternionMultiply(box.ref.q, QuaternionFromAxisAngle(
                    Vector3Normalize({ 1,0,0 }), PI / 2))
    };
    Quad qrear = { rear, {size.x,size.z,size.y} };
    MyDrawPolygonQuad(qrear, color);
    
}

void MyDrawBox(Box box, bool drawPolygon = true, bool drawWireframe = true, Color polygonColor = LIGHTGRAY, Color wireframeColor = DARKGRAY)
{
    if (drawPolygon) MyDrawPolygonBox(box, polygonColor);
    if (drawWireframe)MyDrawWireframeBox(box, wireframeColor);
}

// Fonction permettant de trouver les coordonnées d'une sphere

Vector3 Coordonnes_sphere(int i,int nMeridians, int j, int nParallels, float r, float portion = PI, Vector3 position = {0,0,0})
{
    float lon = (i - 0) * (portion - -portion) / (nMeridians - 0) + -portion;
    float lat = (j - 0) * (HALF_PI - -HALF_PI) / (nParallels - 0) + -HALF_PI;
    float x = r * sin(lon) * cos(lat)+ position.x;
    float z = r * sin(lon) * sin(lat) + position.z;
    float y = r * cos(lon) + position.y ;
    return { x,y,z };
}

//SPHERE

void MyDrawPolygonSphere(Sphere sphere, int nMeridians, int nParallels, Color color = LIGHTGRAY)
{
    nMeridians = nMeridians * 2;
    nParallels = nParallels / 2;
    float theta = 0;

    

    for (int i = 1; i < nMeridians + 1; i++)
    {
        for (int j = 1; j < nParallels + 1; j++)
        {
            // Ici j'appelle la fonction Coordonnes_sphere qui va me donner 4 points 
            // ces coordonnees serviront pour tracer les triangles

            Vector3 p1 = Coordonnes_sphere(i - 1, nMeridians, j - 1, nParallels, 1);
            Vector3 p2 = Coordonnes_sphere(i - 1, nMeridians, j, nParallels, 1);
            Vector3 p3 = Coordonnes_sphere(i, nMeridians, j - 1, nParallels, 1);
            Vector3 p4 = Coordonnes_sphere(i, nMeridians, j, nParallels, 1);

            int numVertex = 12;
            if (rlCheckBufferLimit(numVertex)) rlglDraw();
            rlPushMatrix();
            rlTranslatef(sphere.ref.origin.x, sphere.ref.origin.y, sphere.ref.origin.z);
            Vector3 vect;
            float angle;
            QuaternionToAxisAngle(sphere.ref.q, &vect, &angle);
            rlRotatef(angle * RAD2DEG, vect.x, vect.y, vect.z);
            rlScalef(sphere.radius, sphere.radius, sphere.radius);

            rlBegin(RL_TRIANGLES);

            rlColor4ub(color.r, color.g, color.b, color.a);
                
            // tracer 2 triangles avant 

            rlVertex3f(p1.x,p1.y,p1.z);
            rlVertex3f(p2.x,p2.y,p2.z);
            rlVertex3f(p3.x, p3.y, p3.z);

            rlVertex3f(p2.x, p2.y, p2.z);
            rlVertex3f(p1.x, p1.y, p1.z);
            rlVertex3f(p3.x, p3.y, p3.z);

            // tracer 2 triangles arrière

            rlVertex3f(p4.x, p4.y, p4.z);
            rlVertex3f(p2.x, p2.y, p2.z);
            rlVertex3f(p3.x, p3.y, p3.z);

            rlVertex3f(p2.x, p2.y, p2.z);
            rlVertex3f(p4.x, p4.y, p4.z);
            rlVertex3f(p3.x, p3.y, p3.z);

            rlEnd();
            rlPopMatrix();

        }
    }
}
void MyDrawWireframeSphere(Sphere sphere, int nMeridians, int nParallels, Color color = DARKGRAY)
{
    nMeridians = nMeridians * 2;
    nParallels = nParallels / 2;
    float theta = 0;

    for (int i = 1; i < nMeridians + 1; i++)
    {
        for (int j = 1; j < nParallels + 1; j++)
        {
            Vector3 p = { 0,0,0 };  //position de la sphere
            Vector3 p1 = Coordonnes_sphere(i - 1, nMeridians, j - 1, nParallels, 1);
            Vector3 p2 = Coordonnes_sphere(i - 1, nMeridians, j, nParallels, 1);
            Vector3 p3 = Coordonnes_sphere(i, nMeridians, j - 1, nParallels, 1);
            Vector3 p4 = Coordonnes_sphere(i, nMeridians, j, nParallels, 1);

            int numVertex = 8;
            if (rlCheckBufferLimit(numVertex)) rlglDraw();
            rlPushMatrix();
            rlTranslatef(sphere.ref.origin.x, sphere.ref.origin.y, sphere.ref.origin.z);
            Vector3 vect;
            float angle;
            QuaternionToAxisAngle(sphere.ref.q, &vect, &angle);
            rlRotatef(angle * RAD2DEG, vect.x, vect.y, vect.z);
            rlScalef(sphere.radius, sphere.radius, sphere.radius);

            rlBegin(RL_LINES);

            rlColor4ub(color.r, color.g, color.b, color.a);

            // DIAGONALE
            rlVertex3f(p2.x, p2.y, p2.z);
            rlVertex3f(p3.x, p3.y, p3.z);

            // --
            rlVertex3f(p1.x, p1.y, p1.z);
            rlVertex3f(p2.x, p2.y, p2.z);

            rlVertex3f(p3.x, p3.y, p3.z);
            rlVertex3f(p4.x, p4.y, p4.z);

            // |
            rlVertex3f(p1.x, p1.y, p1.z);
            rlVertex3f(p3.x, p3.y, p3.z);

            rlVertex3f(p2.x, p2.y, p2.z);
            rlVertex3f(p4.x, p4.y, p4.z);
            
            rlEnd();
            rlPopMatrix();

        }
    }
    
}

void MyDrawSphere(Sphere sphere, int nMeridians, int nParallels, bool
    drawPolygon = true, bool drawWireframe = true, Color polygonColor = LIGHTGRAY,
    Color wireframeColor = DARKGRAY)
{
    if (drawPolygon)MyDrawPolygonSphere(sphere, nMeridians, nParallels, polygonColor);
    if (drawWireframe)MyDrawWireframeSphere(sphere, nMeridians, nParallels, wireframeColor);
}

// DEMI SPHERE



void MyDrawPortionSphere(Sphere sphere, int nMeridians, int nParallels, bool
    drawPolygon = true, bool drawWireframe = true, Color polygonColor = LIGHTGRAY,
    Color wireframeColor = DARKGRAY, int parts = 2)
{
    nMeridians = nMeridians / 2;
    float theta = 0;
    int portion;
    int M = nMeridians, P = nParallels;
    if (parts == 2)
    {
        portion = HALF_PI;
    }

    if (parts == 4)
    {
        M = M / 2;
        P = P / 2;
        portion = HALF_PI;
    }

    for (int i = 1; i < M + 1; i++)
    {
        for (int j = 1; j < P + 1; j++)
        {
            Vector3 p1 = Coordonnes_sphere(i - 1, nMeridians, j - 1, nParallels, 1, HALF_PI);
            Vector3 p2 = Coordonnes_sphere(i - 1, nMeridians, j, nParallels, 1, HALF_PI);
            Vector3 p3 = Coordonnes_sphere(i, nMeridians, j - 1, nParallels, 1, HALF_PI);
            Vector3 p4 = Coordonnes_sphere(i, nMeridians, j, nParallels, 1, HALF_PI);

            if (drawPolygon) {
                int numVertex = 12;
                if (rlCheckBufferLimit(numVertex)) rlglDraw();
                rlPushMatrix();
                rlTranslatef(sphere.ref.origin.x, sphere.ref.origin.y, sphere.ref.origin.z);
                Vector3 vect;
                float angle;
                QuaternionToAxisAngle(sphere.ref.q, &vect, &angle);
                rlRotatef(angle * RAD2DEG, vect.x, vect.y, vect.z);
                rlScalef(sphere.radius, sphere.radius, sphere.radius);


                rlBegin(RL_TRIANGLES);
                rlColor4ub(polygonColor.r, polygonColor.g, polygonColor.b, polygonColor.a);
                rlVertex3f(p1.x, p1.y, p1.z);
                rlVertex3f(p2.x, p2.y, p2.z);
                rlVertex3f(p3.x, p3.y, p3.z);

                rlVertex3f(p2.x, p2.y, p2.z);
                rlVertex3f(p1.x, p1.y, p1.z);
                rlVertex3f(p3.x, p3.y, p3.z);

                rlVertex3f(p4.x, p4.y, p4.z);
                rlVertex3f(p2.x, p2.y, p2.z);
                rlVertex3f(p3.x, p3.y, p3.z);

                rlVertex3f(p2.x, p2.y, p2.z);
                rlVertex3f(p4.x, p4.y, p4.z);
                rlVertex3f(p3.x, p3.y, p3.z);

                rlEnd();
                rlPopMatrix();
            }

            if (drawWireframe) {
                int numVertex = 8;
                if (rlCheckBufferLimit(numVertex)) rlglDraw();
                rlPushMatrix();
                rlTranslatef(sphere.ref.origin.x, sphere.ref.origin.y, sphere.ref.origin.z);
                Vector3 vect;
                float angle;
                QuaternionToAxisAngle(sphere.ref.q, &vect, &angle);
                rlRotatef(angle * RAD2DEG, vect.x, vect.y, vect.z);
                rlScalef(sphere.radius, sphere.radius, sphere.radius);

                rlBegin(RL_LINES);
                rlColor4ub(wireframeColor.r, wireframeColor.g, wireframeColor.b, wireframeColor.a);
                // DIAGONALE
                rlVertex3f(p2.x, p2.y, p2.z);
                rlVertex3f(p3.x, p3.y, p3.z);

                // --
                rlVertex3f(p1.x, p1.y, p1.z);
                rlVertex3f(p2.x, p2.y, p2.z);

                rlVertex3f(p3.x, p3.y, p3.z);
                rlVertex3f(p4.x, p4.y, p4.z);

                // |
                rlVertex3f(p1.x, p1.y, p1.z);
                rlVertex3f(p3.x, p3.y, p3.z);

                rlVertex3f(p2.x, p2.y, p2.z);
                rlVertex3f(p4.x, p4.y, p4.z);

                rlEnd();
                rlPopMatrix();
            }
        }
    }
}

void MyDrawPolygonPortionSphere(Sphere sphere, int nMeridians, int nParallels, Color color = LIGHTGRAY, int parts = 2)
{
    MyDrawPortionSphere(sphere, nMeridians, nParallels, true, false, color, LIGHTGRAY, parts);
}

void MyDrawWireframePortionSphere(Sphere sphere, int nMeridians, int nParallels, Color color = DARKGRAY, int parts = 2)
{
    MyDrawPortionSphere(sphere, nMeridians, nParallels,  false, true, LIGHTGRAY, color, parts);
}


// CYLINDER



void MyDrawPolygonCylinder(Cylinder cylinder, int nSectors, bool drawCaps = false, Color color = LIGHTGRAY, int parts=1)
{
    float theta = 0;

    for (int i = 0; i < nSectors/parts; i++)
    {
        int numVertex = 9;
        if (rlCheckBufferLimit(numVertex)) rlglDraw();
        rlPushMatrix();
        rlTranslatef(cylinder.ref.origin.x, cylinder.ref.origin.y, cylinder.ref.origin.z);
        Vector3 vect;
        float angle;
        QuaternionToAxisAngle(cylinder.ref.q, &vect, &angle);
        rlRotatef(angle * RAD2DEG, vect.x, vect.y, vect.z);
        rlScalef(cylinder.radius, cylinder.halfHeight, cylinder.radius);

        if (drawCaps)
        {
            rlBegin(RL_TRIANGLES);
            rlColor4ub(color.r, color.g, color.b, color.a);
            rlVertex3f(0, 1, 0);
            rlVertex3f(sin(theta), 1, cos(theta));
            rlVertex3f(sin(theta + (2 * PI) / nSectors), 1, cos(theta + (2 * PI) / nSectors));

            rlVertex3f(sin(theta), -1, cos(theta));
            rlVertex3f(0, -1, 0);
            rlVertex3f(sin(theta + (2 * PI) / nSectors), -1, cos(theta + (2 * PI) / nSectors));

            rlEnd();
        }
        rlBegin(RL_TRIANGLES);
        rlColor4ub(color.r, color.g, color.b, color.a);
        rlVertex3f(sin(theta), 1, cos(theta));
        rlVertex3f(sin(theta), -1, cos(theta));
        rlVertex3f(sin(theta + (2 * PI) / nSectors), 1, cos(theta + (2 * PI) / nSectors));

        rlVertex3f(sin(theta + (2 * PI) / nSectors), -1, cos(theta + (2 * PI) / nSectors));
        rlVertex3f(sin(theta + (2 * PI) / nSectors), 1, cos(theta + (2 * PI) / nSectors));
        rlVertex3f(sin(theta), -1, cos(theta));

        rlEnd();
        rlPopMatrix();

        theta += (2 * PI) / nSectors;
    }
}


void MyDrawWireframeCylinder(Cylinder cylinder, int nSectors, bool drawCaps = false, Color color = DARKGRAY, int parts = 1)
{
    float theta = 0;

    for (int i = 0; i < nSectors/parts; i++)
    {
        int numVertex = 14;
        if (rlCheckBufferLimit(numVertex)) rlglDraw();
        rlPushMatrix();
        rlTranslatef(cylinder.ref.origin.x, cylinder.ref.origin.y, cylinder.ref.origin.z);
        Vector3 vect;
        float angle;
        QuaternionToAxisAngle(cylinder.ref.q, &vect, &angle);
        rlRotatef(angle * RAD2DEG, vect.x, vect.y, vect.z);
        rlScalef(cylinder.radius, cylinder.halfHeight, cylinder.radius);


        rlBegin(RL_LINES);
        rlColor4ub(color.r, color.g, color.b, color.a);

        if (drawCaps)
        {
            rlVertex3f(0, 1, 0);
            rlVertex3f(sin(theta), 1, cos(theta));

            rlVertex3f(0, -1, 0);
            rlVertex3f(sin(theta), -1, cos(theta));
        }

        rlVertex3f(sin(theta), 1, cos(theta));
        rlVertex3f(sin(theta + (2 * PI) / nSectors), 1, cos(theta + (2 * PI) / nSectors));

        rlVertex3f(sin(theta), -1, cos(theta));
        rlVertex3f(sin(theta + (2 * PI) / nSectors), -1, cos(theta + (2 * PI) / nSectors));

        rlVertex3f(sin(theta), -1, cos(theta));
        rlVertex3f(sin(theta), 1, cos(theta));
        rlEnd();
        rlPopMatrix();
        theta += (2 * PI) / nSectors;
    }
}

void MyDrawCylinder(Cylinder cylinder, int nSectors, bool drawCaps = false, bool
    drawPolygon = true, bool drawWireframe = true, Color polygonColor = LIGHTGRAY,
    Color wireframeColor = DARKGRAY, int parts = 1) {
    if (parts < 1) parts = 1;
    if (drawPolygon) MyDrawPolygonCylinder(cylinder, nSectors, drawCaps, polygonColor, parts);
    if (drawWireframe)MyDrawWireframeCylinder(cylinder, nSectors, drawCaps, wireframeColor, parts);

}

// CAPSULE ----------------

void MyDrawCapsule(Capsule capsule, int nSectors, int nParallels, bool
    drawPolygon = true, bool drawWireframe = true, Color polygonColor = LIGHTGRAY,
    Color wireframeColor = DARKGRAY)
{
    // Draw top sphere
    ReferenceFrame topRef = {
        Vector3Add(capsule.ref.origin,Vector3Scale(capsule.ref.j,capsule.halfHeight)),
        QuaternionMultiply(capsule.ref.q, QuaternionFromAxisAngle(
                    Vector3Normalize({ 0,0,-1 }), PI / 2))
    };
    Sphere topSphere = { topRef, capsule.radius };
    MyDrawPortionSphere(topSphere, nSectors, nParallels, drawPolygon, drawWireframe, polygonColor, wireframeColor);

    // Draw cylinder
    Cylinder cylinder = { capsule.ref, capsule.halfHeight, capsule.radius };
    MyDrawCylinder(cylinder, nSectors, false, drawPolygon, drawWireframe, polygonColor, wireframeColor);

    // Draw bottom sphere
    ReferenceFrame bottomRef = {
        Vector3Subtract(capsule.ref.origin,Vector3Scale(capsule.ref.j,capsule.halfHeight)),
        QuaternionMultiply(capsule.ref.q, QuaternionFromAxisAngle(
                    Vector3Normalize({ 0,0,1 }), PI / 2))
    };
    Sphere bottomSphere = { bottomRef, capsule.radius };
    MyDrawPortionSphere(bottomSphere, nSectors, nParallels, drawPolygon, drawWireframe, polygonColor, wireframeColor, 4);
}

void MyDrawPolygonCapsule(Capsule capsule, int nSectors, int nParallels, Color
    color = LIGHTGRAY)
{
    MyDrawCapsule(capsule, nSectors, nParallels, true, false, color, DARKGRAY);
}

void MyDrawWireframeCapsule(Capsule capsule, int nSectors, int nParallels, Color color = DARKGRAY)
{
    MyDrawCapsule(capsule, nSectors, nParallels, false, true, LIGHTGRAY, color);
}

// CAPSULE

void MyDrawRoundedBox(RoundedBox roundedBox, int nSectors, bool
    drawPolygon = true, bool drawWireframe = true, Color polygonColor = LIGHTGRAY,
    Color wireframeColor = DARKGRAY)
{

    Vector3 size = roundedBox.extents;
    ReferenceFrame right = {
        Vector3Add(roundedBox.ref.origin,Vector3Scale(roundedBox.ref.i,roundedBox.extents.x + roundedBox.radius)),
        QuaternionMultiply(roundedBox.ref.q, QuaternionFromAxisAngle(
                    Vector3Normalize({ 0,0,-1 }), PI / 2))
    };
    Quad qright = { right, {size.y,size.x,size.z} };
    MyDrawQuad(qright, drawPolygon, drawWireframe, polygonColor, wireframeColor);

    ReferenceFrame left = {
        Vector3Add(roundedBox.ref.origin,Vector3Scale(roundedBox.ref.i,-(roundedBox.extents.x + roundedBox.radius))),
        QuaternionMultiply(roundedBox.ref.q, QuaternionFromAxisAngle(
                    Vector3Normalize({ 0,0,1 }), PI / 2))
    };
    Quad qleft = { left, {size.y,size.x,size.z} };
    MyDrawQuad(qleft, drawPolygon, drawWireframe, polygonColor, wireframeColor);

    ReferenceFrame top = {
        Vector3Add(roundedBox.ref.origin,Vector3Scale(roundedBox.ref.j,roundedBox.extents.y + roundedBox.radius)),
        QuaternionMultiply(roundedBox.ref.q, QuaternionFromAxisAngle(
                    Vector3Normalize({ 0,0,0 }), PI / 2))
    };
    Quad qtop = { top, {size.x,size.y,size.z} };
    MyDrawQuad(qtop, drawPolygon, drawWireframe, polygonColor, wireframeColor);

    ReferenceFrame bottom = {
        Vector3Add(roundedBox.ref.origin,Vector3Scale(roundedBox.ref.j,-(roundedBox.extents.y + roundedBox.radius))),
        QuaternionMultiply(roundedBox.ref.q, QuaternionFromAxisAngle(
                    Vector3Normalize({ 0,0,1 }), PI))
    };
    Quad qbottom = { bottom, {size.x,size.y,size.z} };
    MyDrawQuad(qbottom, drawPolygon, drawWireframe, polygonColor, wireframeColor);

    ReferenceFrame front = {
        Vector3Add(roundedBox.ref.origin,Vector3Scale(roundedBox.ref.k,-(roundedBox.extents.z + roundedBox.radius))),
        QuaternionMultiply(roundedBox.ref.q, QuaternionFromAxisAngle(
                    Vector3Normalize({ -1,0,0 }), PI / 2))
    };
    Quad qfront = { front, {size.x,size.z,size.y} };
    MyDrawQuad(qfront, drawPolygon, drawWireframe, polygonColor, wireframeColor);

    ReferenceFrame rear = {
        Vector3Add(roundedBox.ref.origin,Vector3Scale(roundedBox.ref.k,roundedBox.extents.z + roundedBox.radius)),
        QuaternionMultiply(roundedBox.ref.q, QuaternionFromAxisAngle(
                    Vector3Normalize({ 1,0,0 }), PI / 2))
    };
    Quad qrear = { rear, {size.x,size.z,size.y} };
    MyDrawQuad(qrear, drawPolygon, drawWireframe, polygonColor, wireframeColor);

    // END QUADS

    // START 1/4 CYLINDERS
    // FRONT

    Vector3 origin[12] = {
        {0,1,1}, 
        {0,-1,1}, 
        {0,-1,-1}, 
        {0,1,-1}, // z

        {-1,0,-1},
        {1,0,-1},
        {-1,0,1},
        {1,0,1}, // y

        {-1,-1,0},
        {1,-1,0},
        {-1,1,0},
        {1,1,0} // x
    };

    Vector3 quater[12] = {
        {0,0,1}, // ok
        {0,0,-1}, // ok
        {0,1,-1}, // ok
        {0,1,1}, //ok

        {0,-2,0}, // ok
        {0,1,0}, // ok
        {0,-1,0}, // ok
        {0,0,0}, // ok

        {-1,0,2}, // ok
        {1,0,0}, // ok
        {1,0,-2}, // ok
        {-1,0,0} // ok
    };

    float halfh[12] = {
        size.x,
        size.x,
        size.x,
        size.x,
        size.y,
        size.y,
        size.y,
        size.y,
        size.z,
        size.z,
        size.z,
        size.z
    };

    // QUART CYLINDRE

    for (int i = 0; i < 12; i++)
    {
        ReferenceFrame ref = {
        Vector3Add(roundedBox.ref.origin,Vector3Add(
            Vector3Scale(roundedBox.ref.i,origin[i].x*roundedBox.extents.x),Vector3Add(
            Vector3Scale(roundedBox.ref.j,origin[i].y * roundedBox.extents.y),
            Vector3Scale(roundedBox.ref.k,origin[i].z * roundedBox.extents.z))
        )),
        QuaternionMultiply(roundedBox.ref.q, QuaternionMultiply(QuaternionFromAxisAngle(
                    Vector3Normalize({ 0,0,1}), quater[i].z*PI / 2), QuaternionMultiply(QuaternionFromAxisAngle(
                    Vector3Normalize({ 0,1,0 }), quater[i].y*PI / 2), QuaternionFromAxisAngle(
                    Vector3Normalize({ 1,0,0 }), quater[i].x*PI / 2))))
        };
        Cylinder cylindre = { ref, halfh[i], roundedBox.radius};
        MyDrawCylinder(cylindre, nSectors, false, drawPolygon, drawWireframe, polygonColor, wireframeColor, 4);
    }

    // SPHERE

    Vector3 s[8] = {
        {-1,1,1},
        {1,1,1},
        {-1,-1,1},
        {1,-1,1},
        {-1,1,-1},
        {1,1,-1},
        {-1,-1,-1},
        {1,-1,-1}
    };
    Vector3 a[8] = {
        {0,0,0}, //ok
        {0,1,0}, //ok
        {0,1,2}, // 
        //{1,0,0}, //ok
        {0,0,2}, // ok
        {1,-1,-1}, //ok
        {0,2,0}, // ok
        {0,2,2}, // ok
        {1,-1,1}, //ok
    };
    ReferenceFrame sphereref;
    for (int i = 0; i <= 7; i++)
    {
        sphereref = {
        Vector3Add(roundedBox.ref.origin,
        Vector3Add(Vector3Scale(roundedBox.ref.i,s[i].x * roundedBox.extents.x),
        Vector3Add(Vector3Scale(roundedBox.ref.j,s[i].y * roundedBox.extents.y),
                   Vector3Scale(roundedBox.ref.k,s[i].z * roundedBox.extents.z))
        )),
        QuaternionMultiply(roundedBox.ref.q, QuaternionMultiply(QuaternionFromAxisAngle(
                    Vector3Normalize({ 0,0,1}), a[i].z * PI / 2), QuaternionMultiply(QuaternionFromAxisAngle(
                    Vector3Normalize({ 0,1,0 }), a[i].y * PI / 2), QuaternionFromAxisAngle(
                    Vector3Normalize({ 1,0,0 }), a[i].x * PI / 2))))
        };
        Sphere sphere = { sphereref,roundedBox.radius };
        MyDrawPortionSphere(sphere, nSectors, nSectors/2, drawPolygon, drawWireframe, polygonColor, wireframeColor, 4);
    }
}

void MyDrawPolygonRoundedBox(RoundedBox roundedBox, int nSectors, Color
    color = LIGHTGRAY)
{
    MyDrawRoundedBox(roundedBox, nSectors, true, false, LIGHTGRAY, color);
}

void MyDrawWireframeRoundedBox(RoundedBox roundedBox, int nSectors, Color
    color = LIGHTGRAY)
{
    MyDrawRoundedBox(roundedBox, nSectors, false, true, color,
        LIGHTGRAY);
}