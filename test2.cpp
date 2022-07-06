// Point of intersection of a line segment and a triangle

#include <iostream>
#include <conio.h>
#include <math.h>

using namespace std;

//get_point_in_thriangle
#define ILLEGAL_ANGLES_OR_SEGMENT   -1
#define LINE_AND_THRIANGLE_PARALLEL -2
#define CROSS_OUT_OT_SEGMENT        -3
#define DO_NOT_CROSS                -4

class CVect3
{
private:
public:
    float x;
    float y;
    float z;
    CVect3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
    CVect3() : x(0), y(0), z(0) {}

    friend bool operator==(CVect3 l, CVect3 r)  { return (l.x == r.x) && (l.y == r.y) && (l.z == r.z); }
    friend CVect3 operator+(CVect3 l, CVect3 r) { return CVect3(l.x + r.x, l.y + r.y, l.z + r.z); }
    friend CVect3 operator-(CVect3 l, CVect3 r) { return CVect3(l.x - r.x, l.y - r.y, l.z - r.z); }
    friend float operator*(CVect3 l, CVect3 r)  { return l.x * r.x + l.y * r.y + l.z * r.z; }
    friend CVect3 operator*(float l, CVect3 r)  { return CVect3(l * r.x, l * r.y, l * r.z); }
    friend CVect3 operator*(CVect3 l, float r)  { return CVect3(r * l.x, r * l.y, r * l.z); }

    float abs()                                 { return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2)); }
    CVect3 mul(CVect3 a)                        { return CVect3(y * a.z - z * a.y, z * a.x - x * a.z, x * a.y - y * a.x); }
};


float get_distance_line_point(CVect3 a, CVect3 b, CVect3 m)
{
    CVect3 dir0 = b - a;
    CVect3 am = m - a;

    if (a == b)
        return (m - a).abs();

    return am.mul(dir0).abs() / dir0.abs();
}


float area_thriangle(CVect3 a, CVect3 b, CVect3 c)
{
    return (b - a).abs() * get_distance_line_point(a, b, c) / 2;
}


int get_point_in_thriangle(CVect3 t[], CVect3 a, CVect3 b, CVect3* m)
{
    if ((t[0] == t[1]) || (t[1] == t[2]) || (t[2] == t[0]) || (a == b))
        return ILLEGAL_ANGLES_OR_SEGMENT;
    
    CVect3 n = (t[1] - t[0]).mul(t[2] - t[1]);  //get normal to triangle plane 

    //dot multiplication of normal with line direction is zero when line and plane are parallel
    float devv = n * (a - b);
    if (devv == 0.)
        return LINE_AND_THRIANGLE_PARALLEL;

    float u = (n * (a - t[0])) / devv;         //get u where line AB cross plane
    if ((u < 0.) || (u > 1.))
        return CROSS_OUT_OT_SEGMENT;

    *m = a + u * (b - a);                             //get point of cross


    //The sum of the areas of the triangles formed by the point M and the angles of the triangle
    //is equal to the area of ​​the triangle itself if the point is inside
    return (area_thriangle(t[0], t[1], t[2]) ==
           (area_thriangle(  *m, t[1], t[2]) +
            area_thriangle(t[0],   *m, t[2]) +
            area_thriangle(t[0], t[1],   *m)))
        ? NULL : DO_NOT_CROSS;

}

int main()
{
    CVect3 tangle[3] = 
        { CVect3(0., 0., 0.),
          CVect3(1., 2., 0.), 
          CVect3(2., 0., 0.) };

    CVect3  a(1., 1., 1.);
    CVect3  b(1., 1.,-1.);

    CVect3 m;
    int rez = get_point_in_thriangle(tangle, a, b, &m);
    if (!rez)
    {
        std::cout << "x = " << m.x << endl;
        std::cout << "y = " << m.y << endl;
        std::cout << "z = " << m.z << endl;
    }
    else
    {
        switch (rez)
        {
        case ILLEGAL_ANGLES_OR_SEGMENT:     std::cout << "ILLEGAL_ANGLES_OR_SEGMENT" << endl; break;
        case LINE_AND_THRIANGLE_PARALLEL:   std::cout << "LINE_AND_THRIANGLE_PARALLEL " << endl; break;
        case CROSS_OUT_OT_SEGMENT:          std::cout << "CROSS_OUT_OT_SEGMENT" << endl; break;
        case DO_NOT_CROSS:                  std::cout << "DO_NOT_CROSS" << endl; break;
        }
        
    }

  //  std::cout << get_distance_segments(a, b) << endl;
}

