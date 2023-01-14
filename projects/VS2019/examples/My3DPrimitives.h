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
    rlVertex3f(1, 0, 1);
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

void MyDrawPolygonBox(Box box, Color color = LIGHTGRAY)
{
    float x = box.extents.x;
    float y = box.extents.y;
    float z = box.extents.z;

    Vector3 pt1{ x / 2,0,-z / 2 };
    Vector3 pt2{ -x / 2,0,z / 2 };
    Vector3 pt3{ x / 2,0,z / 2 };

    DrawTriangle3D(pt1, pt2, pt3, RED);

    pt3.x = -x / 2;
    pt3.z = -z / 2;
    DrawTriangle3D(pt2, pt1, pt3, YELLOW);

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

void MyDrawSphere(Sphere sphere, int nMeridians, int nParallels, bool
    drawPolygon = true, bool drawWireframe = true, Color polygonColor = LIGHTGRAY,
    Color wireframeColor = DARKGRAY)
{
    float r = sphere.radius;
 
    for (int i = 1; i < nMeridians + 1; i++)
    {
        for (int j = 1; j < nParallels + 1; j++)
        {
            Vector3 p = { 0,0,0 }; // position de la sphere 
            Vector3 p1 = Find_coo(i-1, nMeridians, j-1, nParallels, r);
            Vector3 p2 = Find_coo(i - 1, nMeridians, j, nParallels, r);
            Vector3 p3 = Find_coo(i, nMeridians, j - 1, nParallels, r);
            Vector3 p4 = Find_coo(i, nMeridians, j, nParallels, r);

            if (drawPolygon)
            {
                DrawTriangle3D(p1, p2, p3, polygonColor);
                DrawTriangle3D(p2, p1, p3, polygonColor);
                DrawTriangle3D(p2, p4, p3, polygonColor);
                DrawTriangle3D(p4, p2, p3, polygonColor);
            }

            if (drawWireframe)
            {
                DrawLine3D(p1, p2, wireframeColor);
                DrawLine3D(p2, p3, wireframeColor);
                DrawLine3D(p3, p4, wireframeColor);
                DrawLine3D(p1, p3, wireframeColor);
            }

        }
    }
}

void MyDrawPolygonSphere(Sphere sphere, int nMeridians, int nParallels, Color color = LIGHTGRAY)
{
    MyDrawSphere(sphere, nMeridians, nParallels, true, false, color, DARKGRAY);
}
void MyDrawWireframeSphere(Sphere sphere, int nMeridians, int nParallels, Color color = DARKGRAY)
{
    MyDrawSphere(sphere, nMeridians, nParallels, false, true, DARKGRAY, color);
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

        rlBegin(RL_TRIANGLES);
        rlColor4ub(color.r, color.g, color.b, color.a);
        rlVertex3f(0, 1, 0);
        rlVertex3f(sin(theta), 1, cos(theta));
        rlVertex3f(sin(theta + (2 * PI) / nSectors), 1, cos(theta + (2 * PI) / nSectors));
        
        rlVertex3f(sin(theta), -1, cos(theta));
        rlVertex3f(0, -1, 0);
        rlVertex3f(sin(theta + (2 * PI) / nSectors), -1, cos(theta + (2 * PI) / nSectors));

        rlEnd();

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
        rlVertex3f(0, 1, 0);
        rlVertex3f(sin(theta), 1, cos(theta));
        rlVertex3f(sin(theta), 1, cos(theta));
        rlVertex3f(sin(theta + (2 * PI) / nSectors), 1, cos(theta + (2 * PI) / nSectors));

        rlVertex3f(0, -1, 0);
        rlVertex3f(sin(theta), -1, cos(theta));
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
    if (drawPolygon) MyDrawPolygonCylinder(cylinder, nSectors, true, polygonColor);
    if (drawWireframe)MyDrawWireframeCylinder(cylinder, nSectors, true, wireframeColor);

}

// CAPSULE

void MyDrawCapsule(Capsule capsule, int nSectors, int nParallels, bool
    drawPolygon = true, bool drawWireframe = true, Color polygonColor = LIGHTGRAY,
    Color wireframeColor = DARKGRAY)
{
    float r = capsule.radius;


    float theta = 0;

    for (int i = 1; i < nSectors + 1; i++)
    {
        for (int j = 1; j < nParallels + 1; j++)
        {
            // Les deux demi spheres

            Vector3 p = { 0,capsule.halfHeight,0 }; // position de la sphere 
            Vector3 p1 = Find_coo(i - 1, nSectors, j - 1, nParallels, r, HALF_PI, p);
            Vector3 p2 = Find_coo(i - 1, nSectors, j, nParallels, r, HALF_PI, p);
            Vector3 p3 = Find_coo(i, nSectors, j - 1, nParallels, r, HALF_PI, p);
            Vector3 p4 = Find_coo(i, nSectors, j, nParallels, r, HALF_PI, p);

            p1 = Vector3Add(capsule.ref.origin, p1);
            p2 = Vector3Add(capsule.ref.origin, p2);
            p3 = Vector3Add(capsule.ref.origin, p3);
            p4 = Vector3Add(capsule.ref.origin, p4);

            p1 = Vector3RotateByQuaternion(p1, capsule.ref.q);
            p2 = Vector3RotateByQuaternion(p2, capsule.ref.q);
            p3 = Vector3RotateByQuaternion(p3, capsule.ref.q);
            p4 = Vector3RotateByQuaternion(p4, capsule.ref.q);

            if (drawPolygon)
            {
                DrawTriangle3D(p1, p2, p3, polygonColor);
                DrawTriangle3D(p2, p1, p3, polygonColor);
                DrawTriangle3D(p2, p4, p3, polygonColor);
                DrawTriangle3D(p4, p2, p3, polygonColor);
            }

            if (drawWireframe)
            {
                DrawLine3D(p1, p2, wireframeColor);
                DrawLine3D(p2, p3, wireframeColor);
                DrawLine3D(p3, p4, wireframeColor);
                DrawLine3D(p1, p3, wireframeColor);
            }
        }

        for (int j = 1; j < nParallels + 1; j++)
        {
            // Les deux demi spheres

            Vector3 p = { 0,capsule.halfHeight,0 }; // position de la sphere 
            Vector3 p1 = Find_coo(i - 1, nSectors, j - 1, nParallels, r, HALF_PI, p);
            Vector3 p2 = Find_coo(i - 1, nSectors, j, nParallels, r, HALF_PI, p);
            Vector3 p3 = Find_coo(i, nSectors, j - 1, nParallels, r, HALF_PI, p);
            Vector3 p4 = Find_coo(i, nSectors, j, nParallels, r, HALF_PI, p);

            p1.y = -p1.y;
            p2.y = -p2.y;
            p3.y = -p3.y;
            p4.y = -p4.y;

            p1 = Vector3Add(capsule.ref.origin, p1);
            p2 = Vector3Add(capsule.ref.origin, p2);
            p3 = Vector3Add(capsule.ref.origin, p3);
            p4 = Vector3Add(capsule.ref.origin, p4);

            p1 = Vector3RotateByQuaternion(p1, capsule.ref.q);
            p2 = Vector3RotateByQuaternion(p2, capsule.ref.q);
            p3 = Vector3RotateByQuaternion(p3, capsule.ref.q);
            p4 = Vector3RotateByQuaternion(p4, capsule.ref.q);

            if (drawPolygon)
            {
                DrawTriangle3D(p1, p2, p3, polygonColor);
                DrawTriangle3D(p2, p1, p3, polygonColor);
                DrawTriangle3D(p2, p4, p3, polygonColor);
                DrawTriangle3D(p4, p2, p3, polygonColor);
            }

            if (drawWireframe)
            {
                DrawLine3D(p1, p2, wireframeColor);
                DrawLine3D(p2, p3, wireframeColor);
                DrawLine3D(p3, p4, wireframeColor);
                DrawLine3D(p1, p3, wireframeColor);
            }
        }

        Vector3 pt1{ capsule.radius * sin(theta),capsule.halfHeight,capsule.radius * cos(theta) };
        Vector3 pt2{ capsule.radius * sin(theta + (2 * PI) / nSectors),capsule.halfHeight,capsule.radius * cos(theta + (2 * PI) / nSectors) };
        Vector3 pt3{ 0,capsule.halfHeight,0 };
        Vector3 pt4{ capsule.radius * sin(theta),-capsule.halfHeight,capsule.radius * cos(theta) };
        Vector3 pt5{ capsule.radius * sin(theta + (2 * PI) / nSectors),-capsule.halfHeight, capsule.radius * cos(theta + (2 * PI) / nSectors) };
        Vector3 pt6{ 0,-capsule.halfHeight,0 };


        // FACADES - LE CORPS DE LA CAPSULE

        /*pt1.x = capsule.radius * sin(theta);
        pt1.z = capsule.radius * cos(theta);

        pt2.x = capsule.radius * sin(theta + (2 * PI) / nSectors);
        pt2.z = capsule.radius * cos(theta + (2 * PI) / nSectors);


        pt4.x = capsule.radius * sin(theta);
        pt4.z = capsule.radius * cos(theta);

        pt5.x = capsule.radius * sin(theta + (2 * PI) / nSectors);
        pt5.z = capsule.radius * cos(theta + (2 * PI) / nSectors);*/

        pt1 = Vector3Add(capsule.ref.origin, pt1);
        pt2 = Vector3Add(capsule.ref.origin, pt2);
        pt3 = Vector3Add(capsule.ref.origin, pt3);
        pt4 = Vector3Add(capsule.ref.origin, pt4);
        pt5 = Vector3Add(capsule.ref.origin, pt5);
        pt6 = Vector3Add(capsule.ref.origin, pt6);

        pt1 = Vector3RotateByQuaternion(pt1, capsule.ref.q);
        pt2 = Vector3RotateByQuaternion(pt2, capsule.ref.q);
        pt3 = Vector3RotateByQuaternion(pt3, capsule.ref.q);
        pt4 = Vector3RotateByQuaternion(pt4, capsule.ref.q);
        pt5 = Vector3RotateByQuaternion(pt5, capsule.ref.q);
        pt6 = Vector3RotateByQuaternion(pt6, capsule.ref.q);

        if (drawWireframe)
        {
            DrawLine3D(pt1, pt2, wireframeColor);
            DrawLine3D(pt4, pt5, wireframeColor);
            DrawLine3D(pt1, pt4, wireframeColor);
            DrawLine3D(pt2, pt5, wireframeColor);
        }

        if (drawPolygon)
        {
            DrawTriangle3D(pt2, pt1, pt4, polygonColor);
            DrawTriangle3D(pt4, pt5, pt2, polygonColor);
        }
        theta += (2 * PI) / nSectors;
    }
}

void MyDrawPolygonCapsule(Capsule capsule, int nSectors, int nParallels, Color
    color = LIGHTGRAY)
{
    MyDrawCapsule(capsule, nSectors, nParallels, true, false, color,
        LIGHTGRAY);
}

void MyDrawWireframeCapsule(Capsule capsule, int nSectors, int nParallels, Color
    color = LIGHTGRAY)
{
    MyDrawCapsule(capsule, nSectors, nParallels, false, true, LIGHTGRAY,
        color);
}

void MyDrawPolygonRoundedBox(RoundedBox roundedBox, int nSectors, Color
    color = LIGHTGRAY);

void MyDrawWireframeRoundedBox(RoundedBox roundedBox, int nSectors, Color
    color = LIGHTGRAY);

void MyDrawRoundedBox(RoundedBox roundedBox, int nSectors, bool
    drawPolygon = true, bool drawWireframe = true, Color polygonColor = LIGHTGRAY,
    Color wireframeColor = DARKGRAY)
{
    float r = roundedBox.radius;

    for (int i = 1; i < nSectors/2 + 1; i++)
    {
        for (int j = 1; j < nSectors/2 + 1; j++)
        {
            Vector3 p = { 0,0,0 }; // position de la sphere 
            Vector3 p1 = Find_coo(i - 1, nSectors, j - 1, nSectors, r, HALF_PI, p);
            Vector3 p2 = Find_coo(i - 1, nSectors, j, nSectors, r, HALF_PI, p);
            Vector3 p3 = Find_coo(i, nSectors, j - 1, nSectors, r, HALF_PI, p);
            Vector3 p4 = Find_coo(i, nSectors, j, nSectors, r, HALF_PI, p);

            if (drawPolygon)
            {
                DrawTriangle3D(p1, p2, p3, polygonColor);
                DrawTriangle3D(p2, p1, p3, polygonColor);
                DrawTriangle3D(p2, p4, p3, polygonColor);
                DrawTriangle3D(p4, p2, p3, polygonColor);
            }

            if (drawWireframe)
            {
                DrawLine3D(p1, p2, wireframeColor);
                DrawLine3D(p2, p3, wireframeColor);
                DrawLine3D(p3, p4, wireframeColor);
                DrawLine3D(p1, p3, wireframeColor);
            }

        }
    }

    //roundedBox.ref
}

void MyTest(Quad quad, Color color = LIGHTGRAY)
{
    float x = quad.extents.x;
    float z = quad.extents.z;

    Vector3 pt1, pt2, pt3, pt4;
    pt1 = Vector3Add(quad.ref.origin, { x,0,z});
    pt2 = Vector3Add(quad.ref.origin, { x,0,-z});
    pt3 = Vector3Add(quad.ref.origin, { -x,0,z});
    pt4 = Vector3Add(quad.ref.origin, { -x,0,-z });

    pt1 = Vector3RotateByQuaternion(pt1, quad.ref.q);
    pt2 = Vector3RotateByQuaternion(pt2, quad.ref.q);
    pt3 = Vector3RotateByQuaternion(pt3, quad.ref.q);
    pt4 = Vector3RotateByQuaternion(pt4, quad.ref.q);

    DrawTriangle3D(pt1, pt2, pt3, RED);
    DrawTriangle3D(pt2, pt1, pt3, RED);
    DrawTriangle3D(pt3, pt2, pt4, YELLOW);
    DrawTriangle3D(pt2, pt3, pt4, YELLOW);
}