#define MY3DPRIMITIVES_H

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

    Vector3 pt1{ x,0,0 };
    Vector3 pt2{ 0,0,z };
    Vector3 pt3{ 0,0,0 };

    DrawTriangle3D(pt2, pt1, pt3, RED);
    
    pt3.x = x;
    pt3.z = z;
    DrawTriangle3D(pt1, pt2, pt3, YELLOW);

    pt3.x = 0;
    pt3.z = 0;
    pt1.x = -x;
    DrawTriangle3D(pt1, pt2, pt3, GREEN);

    pt3.x = -x;
    pt3.z = z;
    DrawTriangle3D(pt2, pt1, pt3, BLACK);

    pt3.x = 0;
    pt3.z = 0;
    pt2.z = -z;
    DrawTriangle3D(pt2, pt1, pt3, VIOLET);

    pt3.x = -x;
    pt3.z = -z;
    DrawTriangle3D(pt1, pt2, pt3, WHITE);

    pt3.x = 0;
    pt3.z = 0;
    pt1.x = x;
    DrawTriangle3D(pt1, pt2, pt3, BLUE);
    
    
    pt3.x = x;
    pt3.z = -z;
    DrawTriangle3D(pt2, pt1, pt3, color);

}


void MyDrawDisk(Disk disk, int nb, Color color = LIGHTGRAY)
{


    float r = nb;
    const int total = 20;
    Vector3 globe[total + 1][total + 1];
    for (int i = 0; i < total + 1; i++)
    {
        float lon = (i - 0) * (PI - -PI) / (total - 0) + -PI;
        for (int j = 0; j < total + 1; j++)
        {
            float lat = (j - 0) * (HALF_PI - -HALF_PI) / (total - 0) + -HALF_PI;
            float x = r * sin(lon) * cos(lat);
            float z = r * sin(lon) * sin(lat);
            float y = r * cos(lon);
            globe[i][j] = { x,y,z };
        }
    }

    for (int i = 1; i < total + 1; i++)
    {

        for (int j = 1; j < total + 1; j++)
        {

            Vector3 p1 = globe[i - 1][j - 1];
            Vector3 p2 = globe[i - 1][j];
            Vector3 p3 = globe[i][j - 1];
            Vector3 p4 = globe[i][j];

            Vector3 t1[3] = { p1,p2,p3 };
            Vector3 t2[3] = { p2,p1,p3 };

            DrawTriangle3D(p1, p2, p3, color);
            DrawTriangle3D(p2, p1, p3, color);
            DrawTriangle3D(p2, p4, p3, color);
            DrawTriangle3D(p4, p2, p3, color);

            //DrawTriangleStrip3D(t1, 3, RED);
            //DrawTriangleStrip3D(t2, 2, RED);
        }
    }
}

