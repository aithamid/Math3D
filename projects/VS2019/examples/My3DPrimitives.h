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
    float x = quad.extents.x;
    float z = quad.extents.z;

    Vector3 pt1{ x/2,0,-z/2 };
    Vector3 pt2{ -x/2,0,z/2 };
    Vector3 pt3{ x/2,0,z/2 };

    DrawTriangle3D(pt1, pt2, pt3, RED);
    
    pt3.x = -x/2;
    pt3.z = -z/2;
    DrawTriangle3D(pt2, pt1, pt3, YELLOW);


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


void MyDrawDisk(Disk disk, int nb, Color color = LIGHTGRAY)
{
        Vector3 pt1{ 0,0,0 };
        Vector3 pt2{ 0,0,0 };
        Vector3 pt3{ 0,0,0 };

        float theta = 0;

        for (int i = 0; i < nb; i++)
        {
            pt1.x = disk.radius * sin(theta);
            pt1.z = disk.radius * cos(theta);

            pt2.x = disk.radius * sin(theta + (2 * PI) / nb);
            pt2.z = disk.radius * cos(theta + (2 * PI) / nb);
            DrawTriangle3D(pt1, pt2, pt3, RED);
            theta += (2 * PI) / nb;
        }
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
            Vector3 p1 = Find_coo(i-1, nMeridians, j-1, nParallels, r, HALF_PI, p);
            Vector3 p2 = Find_coo(i - 1, nMeridians, j, nParallels, r, HALF_PI, p);
            Vector3 p3 = Find_coo(i, nMeridians, j - 1, nParallels, r, HALF_PI, p);
            Vector3 p4 = Find_coo(i, nMeridians, j, nParallels, r, HALF_PI, p);

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

void MyDrawCylinder(Cylinder cylinder, int nSectors, bool drawCaps = false, bool
    drawPolygon = true, bool drawWireframe = true, Color polygonColor = LIGHTGRAY,
    Color wireframeColor = DARKGRAY) {

    Vector3 pt1{ 0,cylinder.halfHeight,0 };
    Vector3 pt2{ 0,cylinder.halfHeight,0 };
    Vector3 pt3{ 0,cylinder.halfHeight,0 };
    Vector3 pt4{ 0,-cylinder.halfHeight,0 };
    Vector3 pt5{ 0,-cylinder.halfHeight,0 };
    Vector3 pt6{ 0,-cylinder.halfHeight,0 };

    float theta = 0;

    for (int i = 0; i < nSectors; i++)
    {
        pt1.x = cylinder.radius * sin(theta);
        pt1.z = cylinder.radius * cos(theta);

        pt2.x = cylinder.radius * sin(theta + (2 * PI) / nSectors);
        pt2.z = cylinder.radius * cos(theta + (2 * PI) / nSectors);


        pt4.x = cylinder.radius * sin(theta);
        pt4.z = cylinder.radius * cos(theta);

        pt5.x = cylinder.radius * sin(theta + (2 * PI) / nSectors);
        pt5.z = cylinder.radius * cos(theta + (2 * PI) / nSectors);

        if (drawWireframe)
        {
            DrawLine3D(pt1, pt2, wireframeColor);
            DrawLine3D(pt4, pt5, wireframeColor);
            DrawLine3D(pt1, pt4, wireframeColor);
            DrawLine3D(pt2, pt5, wireframeColor);
            if (drawCaps)
            {
                DrawLine3D(pt1, pt3, wireframeColor);
                DrawLine3D(pt2, pt3, wireframeColor);
                DrawLine3D(pt4, pt6, wireframeColor);
                DrawLine3D(pt5, pt6, wireframeColor);
            }
        }

        // FACADES
        if (drawPolygon)
        {
            DrawTriangle3D(pt2, pt1, pt4, polygonColor);
            DrawTriangle3D(pt4, pt5, pt2, polygonColor);
            if (drawCaps)
            {
                DrawTriangle3D(pt1, pt2, pt3, polygonColor);
                DrawTriangle3D(pt5, pt4, pt6, polygonColor);
            }
        }
        theta += (2 * PI) / nSectors;
    }
}

void MyDrawPolygonCylinder(Cylinder cylinder, int nSectors, bool drawCaps = false, Color color = LIGHTGRAY)
{
    MyDrawCylinder(cylinder, nSectors, drawCaps, true, false, color, LIGHTGRAY);
}

void MyDrawWireframeCylinder(Cylinder cylinder, int nSectors, bool drawCaps = false, Color color = LIGHTGRAY)
{
    MyDrawCylinder(cylinder, nSectors, drawCaps, false, true, LIGHTGRAY, color);
}


// CAPSULE

void MyDrawCapsule(Capsule capsule, int nSectors, int nParallels, bool
    drawPolygon = true, bool drawWireframe = true, Color polygonColor = LIGHTGRAY,
    Color wireframeColor = DARKGRAY)
{
    float r = capsule.radius;

    Vector3 pt1{ 0,capsule.halfHeight,0 };
    Vector3 pt2{ 0,capsule.halfHeight,0 };
    Vector3 pt3{ 0,capsule.halfHeight,0 };
    Vector3 pt4{ 0,-capsule.halfHeight,0 };
    Vector3 pt5{ 0,-capsule.halfHeight,0 };
    Vector3 pt6{ 0,-capsule.halfHeight,0 };

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

            p1.y = -p1.y;
            p2.y = -p2.y;
            p3.y = -p3.y;
            p4.y = -p4.y;

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

        // FACADES - LE CORPS DE LA CAPSULE

        pt1.x = capsule.radius * sin(theta);
        pt1.z = capsule.radius * cos(theta);

        pt2.x = capsule.radius * sin(theta + (2 * PI) / nSectors);
        pt2.z = capsule.radius * cos(theta + (2 * PI) / nSectors);


        pt4.x = capsule.radius * sin(theta);
        pt4.z = capsule.radius * cos(theta);

        pt5.x = capsule.radius * sin(theta + (2 * PI) / nSectors);
        pt5.z = capsule.radius * cos(theta + (2 * PI) / nSectors);

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

}