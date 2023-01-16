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
        q = QuaternionFromAxisAngle(Vector3Normalize({ 1,1,1 }),
            PI / 4);
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


Vector3 Find_coo(int i,int nMeridians, int j, int nParallels, float r, float portion = PI, Vector3 position = {0,0,0})
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

    int numVertex = nMeridians* nParallels * 12;
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

    for (int i = 1; i < nMeridians + 1; i++)
    {
        for (int j = 1; j < nParallels + 1; j++)
        {
            Vector3 p1 = Find_coo(i - 1, nMeridians, j - 1, nParallels, 1);
            Vector3 p2 = Find_coo(i - 1, nMeridians, j, nParallels, 1);
            Vector3 p3 = Find_coo(i, nMeridians, j - 1, nParallels, 1);
            Vector3 p4 = Find_coo(i, nMeridians, j, nParallels, 1);

                

                rlVertex3f(p1.x,p1.y,p1.z);
                rlVertex3f(p2.x,p2.y,p2.z);
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

        }
    }
    rlEnd();
    rlPopMatrix();
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
            Vector3 p1 = Find_coo(i - 1, nMeridians, j - 1, nParallels, 1);
            Vector3 p2 = Find_coo(i - 1, nMeridians, j, nParallels, 1);
            Vector3 p3 = Find_coo(i, nMeridians, j - 1, nParallels, 1);
            Vector3 p4 = Find_coo(i, nMeridians, j, nParallels, 1);

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

void MyDrawPolygonPortionSphere(Sphere sphere, int nMeridians, int nParallels, Color color = LIGHTGRAY)
{
    float theta = 0;

    int numVertex = nMeridians * nParallels * 12;
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

    for (int i = 1; i < nMeridians + 1; i++)
    {
        for (int j = 1; j < nParallels + 1; j++)
        {
            Vector3 p1 = Find_coo(i - 1, nMeridians, j - 1, nParallels, 1);
            Vector3 p2 = Find_coo(i - 1, nMeridians, j, nParallels, 1);
            Vector3 p3 = Find_coo(i, nMeridians, j - 1, nParallels, 1);
            Vector3 p4 = Find_coo(i, nMeridians, j, nParallels, 1);



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



        }
    }
    rlEnd();
    rlPopMatrix();
}
void MyDrawWireframePortionSphere(Sphere sphere, int nMeridians, int nParallels, Color color = DARKGRAY)
{
    float theta = 0;

    for (int i = 1; i < nMeridians/2 + 1; i++)
    {
        for (int j = 1; j < nParallels + 1; j++)
        {
            Vector3 p = { 0,0,0 };  //position de la sphere
            Vector3 p1 = Find_coo(i - 1, nMeridians, j - 1, nParallels, 1);
            Vector3 p2 = Find_coo(i - 1, nMeridians, j, nParallels, 1);
            Vector3 p3 = Find_coo(i, nMeridians, j - 1, nParallels, 1);
            Vector3 p4 = Find_coo(i, nMeridians, j, nParallels, 1);

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

void MyDrawPortionSphere(Sphere sphere, int nMeridians, int nParallels, bool
    drawPolygon = true, bool drawWireframe = true, Color polygonColor = LIGHTGRAY,
    Color wireframeColor = DARKGRAY)
{
    if (drawPolygon)MyDrawPolygonPortionSphere(sphere, nMeridians, nParallels, polygonColor);
    if (drawWireframe)MyDrawWireframePortionSphere(sphere, nMeridians, nParallels, wireframeColor);
}

// CYLINDER



void MyDrawPolygonCylinder(Cylinder cylinder, int nSectors, bool drawCaps = false, Color color = LIGHTGRAY)
{
    float theta = 0;

    for (int i = 0; i < nSectors; i++)
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


void MyDrawWireframeCylinder(Cylinder cylinder, int nSectors, bool drawCaps = false, Color color = DARKGRAY)
{
    float theta = 0;

    for (int i = 0; i < nSectors; i++)
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
    Color wireframeColor = DARKGRAY) {
    if (drawPolygon) MyDrawPolygonCylinder(cylinder, nSectors, drawCaps, polygonColor);
    if (drawWireframe)MyDrawWireframeCylinder(cylinder, nSectors, drawCaps, wireframeColor);

}

// CAPSULE ----------------

void MyDrawPolygonCapsule(Capsule capsule, int nSectors, int nParallels, Color
    color = LIGHTGRAY)
{
    // Draw top sphere
    ReferenceFrame topRef = {
        Vector3Add(capsule.ref.origin,Vector3Scale(capsule.ref.j,capsule.halfHeight)),
        QuaternionMultiply(capsule.ref.q, QuaternionFromAxisAngle(
                    Vector3Normalize({ 0,0,-1 }), PI / 2))
    };
    Sphere topSphere = { topRef, capsule.radius };
    MyDrawPolygonPortionSphere(topSphere, nSectors, nParallels, color);

    // Draw cylinder
    Cylinder cylinder = { capsule.ref, capsule.halfHeight, capsule.radius };
    MyDrawPolygonCylinder(cylinder, nSectors, false, color);

    // Draw bottom sphere
    ReferenceFrame bottomRef = {
        Vector3Subtract(capsule.ref.origin,Vector3Scale(capsule.ref.j,capsule.halfHeight)),
        QuaternionMultiply(capsule.ref.q, QuaternionFromAxisAngle(
                    Vector3Normalize({ 0,0,1 }), PI / 2))
    };
    Sphere bottomSphere = { bottomRef, capsule.radius };
    MyDrawPolygonPortionSphere(bottomSphere, nSectors, nParallels, color);
}

void MyDrawWireframeCapsule(Capsule capsule, int nSectors, int nParallels, Color color = DARKGRAY)
{
    // Draw top sphere
    ReferenceFrame topRef = {
        Vector3Add(capsule.ref.origin,Vector3Scale(capsule.ref.j,capsule.halfHeight)),
        QuaternionMultiply(capsule.ref.q, QuaternionFromAxisAngle(
                    Vector3Normalize({ 0,0,-1 }), PI / 2))
    };
    Sphere topSphere = { topRef, capsule.radius };
    MyDrawWireframePortionSphere(topSphere, nSectors, nParallels, color);

    // Draw cylinder
    Cylinder cylinder = { capsule.ref, capsule.halfHeight, capsule.radius };
    MyDrawWireframeCylinder(cylinder, nSectors, false, color);

    // Draw bottom sphere
    ReferenceFrame bottomRef = {
        Vector3Subtract(capsule.ref.origin,Vector3Scale(capsule.ref.j,capsule.halfHeight)),
        QuaternionMultiply(capsule.ref.q, QuaternionFromAxisAngle(
                    Vector3Normalize({ 0,0,1 }), PI / 2))
    };
    Sphere bottomSphere = { bottomRef, capsule.radius };
    MyDrawWireframePortionSphere(bottomSphere, nSectors, nParallels, color);
}

void MyDrawCapsule(Capsule capsule, int nSectors, int nParallels, bool
    drawPolygon = true, bool drawWireframe = true, Color polygonColor = LIGHTGRAY,
    Color wireframeColor = DARKGRAY)
{
    if (drawPolygon) MyDrawPolygonCapsule(capsule, nSectors, nParallels, polygonColor);
    if (drawWireframe)MyDrawWireframeCapsule(capsule, nSectors, nParallels, wireframeColor);
}

void MyDrawPolygonRoundedBox(RoundedBox roundedBox, int nSectors, Color
    color = LIGHTGRAY)
{

}

void MyDrawWireframeRoundedBox(RoundedBox roundedBox, int nSectors, Color
    color = LIGHTGRAY)
{

    Vector3 size = roundedBox.extents;
    ReferenceFrame right = {
        Vector3Add(roundedBox.ref.origin,Vector3Scale(roundedBox.ref.i,roundedBox.extents.x+roundedBox.radius)),
        QuaternionMultiply(roundedBox.ref.q, QuaternionFromAxisAngle(
                    Vector3Normalize({ 0,0,-1 }), PI / 2))
    };
    Quad qright = { right, {size.y,size.x,size.z} };
    MyDrawQuad(qright);

    ReferenceFrame left = {
        Vector3Add(roundedBox.ref.origin,Vector3Scale(roundedBox.ref.i,-(roundedBox.extents.x + roundedBox.radius))),
        QuaternionMultiply(roundedBox.ref.q, QuaternionFromAxisAngle(
                    Vector3Normalize({ 0,0,1 }), PI / 2))
    };
    Quad qleft = { left, {size.y,size.x,size.z} };
    MyDrawQuad(qleft);

    ReferenceFrame top = {
        Vector3Add(roundedBox.ref.origin,Vector3Scale(roundedBox.ref.j,roundedBox.extents.y + roundedBox.radius)),
        QuaternionMultiply(roundedBox.ref.q, QuaternionFromAxisAngle(
                    Vector3Normalize({ 0,0,0 }), PI / 2))
    };
    Quad qtop = { top, {size.x,size.y,size.z} };
    MyDrawQuad(qtop);

    ReferenceFrame bottom = {
        Vector3Add(roundedBox.ref.origin,Vector3Scale(roundedBox.ref.j,-(roundedBox.extents.y + roundedBox.radius))),
        QuaternionMultiply(roundedBox.ref.q, QuaternionFromAxisAngle(
                    Vector3Normalize({ 0,0,1 }), PI))
    };
    Quad qbottom = { bottom, {size.x,size.y,size.z} };
    MyDrawQuad(qbottom);

    ReferenceFrame front = {
        Vector3Add(roundedBox.ref.origin,Vector3Scale(roundedBox.ref.k,-(roundedBox.extents.z + roundedBox.radius))),
        QuaternionMultiply(roundedBox.ref.q, QuaternionFromAxisAngle(
                    Vector3Normalize({ -1,0,0 }), PI / 2))
    };
    Quad qfront = { front, {size.x,size.z,size.y} };
    MyDrawQuad(qfront);

    ReferenceFrame rear = {
        Vector3Add(roundedBox.ref.origin,Vector3Scale(roundedBox.ref.k,roundedBox.extents.z + roundedBox.radius)),
        QuaternionMultiply(roundedBox.ref.q, QuaternionFromAxisAngle(
                    Vector3Normalize({ 1,0,0 }), PI / 2))
    };
    Quad qrear = { rear, {size.x,size.z,size.y} };
    MyDrawQuad(qrear);
}

void MyDrawRoundedBox(RoundedBox roundedBox, int nSectors, bool
    drawPolygon = true, bool drawWireframe = true, Color polygonColor = LIGHTGRAY,
    Color wireframeColor = DARKGRAY)
{
    if (drawPolygon) MyDrawPolygonRoundedBox(roundedBox, nSectors, polygonColor);
    if (drawWireframe)MyDrawWireframeRoundedBox(roundedBox, nSectors, wireframeColor);
}