#include <iostream>
#include <conio.h>
#include <math.h>

using namespace std;

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


bool get_distance_lines(CVect3 a, CVect3 b, CVect3 c, CVect3 d, float* u, float* v)
{
    //calc distance between lines 
    //f0(u) = A + u(B - A)
    //f1(v) = C + v(D - C)
    //return u and v if AB and CD not parallel

    CVect3 dir0 = b - a;
    CVect3 dir1 = d - c;
    CVect3 diff = a - c;

    float dir0_dir0 = dir0 * dir0;
    float dir0_dir1 = dir0 * dir1;
    float dir1_dir1 = dir1 * dir1;
    float dir0_diff = dir0 * diff;
    float dir1_diff = dir1 * diff;
    float devv = dir0_dir0 * dir1_dir1 - dir0_dir1 * dir0_dir1;

    if (devv != 0.)
    {
        *u = (dir0_dir1 * dir1_diff - dir1_dir1 * dir0_diff) / devv;
        *v = (dir0_dir0 * dir1_diff - dir0_dir1 * dir0_diff) / devv;
        return true;
    }

    return false;
}

float get_distance_line_point(CVect3 a, CVect3 b, CVect3 m)
{
    CVect3 dir0 = b - a;
    CVect3 am = m - a;

    if (a == b)
        return (m - a).abs();

    return am.mul(dir0).abs() / dir0.abs();
}

float get_parallel_projection(CVect3 a, CVect3 b, CVect3 m)
{
    //return value u. 'u' is rezilt parallel projection point M to Line AB f(u) = A + u(B - A). 
    CVect3 dir0 = b - a;
    return (dir0 * (m - a)) / (dir0 * dir0);
}

float get_distance_segments(CVect3 a, CVect3 b, CVect3 c, CVect3 d)
{
    if ((a == b) || (c == d))                       //invalid points values
        return -1.;

    if ((a == c) || (a == d) || (b == c) || (b == d))
        return 0.;

    float u, v;

    //AB not parallel to CD
    if (get_distance_lines(a, b, c, d, &u, &v))     
    {
        if ((u >= 0.) && (u <= 1.) && (v >= 0.) && (v <= 1.))       // u and v in [0., 1.]  - IDEAL VARIANT!
            return ((a + u * (b - a)) - (c + v * (d - c))).abs();

        if ((u >= 0.) && (u <= 1.))                                 // v out of range
            return get_distance_line_point(a, b, v < 0. ? c : d);

        if ((v >= 0.) && (v <= 1.))                                 // u out of range
            return get_distance_line_point(c, d, u < 0. ? a : b);

        return ((u < 0. ? a : b) - (v < 0. ? c : d)).abs();         // u and v out of range
    }

    // AB parallel to CD
    float uc = get_parallel_projection(a, b, c);
    float ud = get_parallel_projection(a, b, d);


    // if the projection on the line AB is outside the segment AB
    if ((uc < 0.) && (ud < 0.))
        return uc < ud ? (a - d).abs() : (a - c).abs();

    if ((uc > 1.) && (ud > 1.))
        return uc < ud ? (b - c).abs() : (b - d).abs();

    // the lines is parallel
    return get_distance_line_point(a, b, c);
}




int main()
{
    CVect3 a(0., 0., 0.);
    CVect3 b(2., 2., 0.);
    CVect3 c(2., 0., 1.);
    CVect3 d(0., 2., 1.);

    std::cout << get_distance_segments(a, b, c, d) << endl;
}

