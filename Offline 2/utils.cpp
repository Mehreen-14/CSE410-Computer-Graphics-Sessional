#include <bits/stdc++.h>
#include <vector>
#include <cmath>

#define pi (2 * acos(0.0))
using namespace std;

double Rad_angle(double deg)
{
    double angle = (double)(deg * pi) / 180;
    return angle;
}

struct Point
{
public:
    double x, y, z, w;
    Point() : x(0), y(0), z(0), w(1) {}

    Point(double x, double y, double z) : x(x), y(y), z(z), w(1) {}
    Point(double x, double y, double z, double w) : x(x), y(y), z(z), w(w) {}

    Point operator+(const Point &point) const
    {
        Point temp = Point(x + point.x, y + point.y, z + point.z); 
        return temp;
    }

    Point operator-(const Point &point) const
    {
        Point temp = Point(x - point.x, y - point.y, z - point.z);
        return temp;
    }

    Point operator*(double d) const
    {
        Point temp = Point(x * d, y * d, z * d);;
        return temp;
    }

    Point operator/(double d) const
    {
        Point temp = Point(x / d, y / d, z / d);
        return temp;
    }

    double dotproduct(const Point &point) const
    {
        double temp = (x * point.x + y * point.y + z * point.z);
        return temp;
    }

    Point cross_product(const Point &point) const
    {
        Point temp = Point(y * point.z - z * point.y, z * point.x - x * point.z, x * point.y - y * point.x);
        return temp;
    }

    // normalization
    void normalize()
    {
        double n = sqrt(x * x + y * y + z * z);
        x = x / n;
        y = y / n;
        z = z / n;
    }

    void Scaling_point()
    {
        x = x / w;
        y = y / w;
        z = z / w;
        w = w / w;
    }
};

class All_Transformations
{
    vector<vector<double>> my_matrix;

public:
    All_Transformations()
    {
        my_matrix = vector<vector<double>>(4, std::vector<double>(4, 0));
        my_matrix[0][0] = 1;
        my_matrix[1][1] = 1;
        my_matrix[2][2] = 1;
        my_matrix[3][3] = 1;
    }

    All_Transformations(vector<vector<double>> matrix) : my_matrix(matrix) {}

    ~All_Transformations(){
        //cout<<"cleared"<<endl;
        my_matrix.clear();
    }

    // Translation
    static All_Transformations Translation(All_Transformations temp, double tx, double ty, double tz)
    {

        temp.my_matrix[0][3] = tx;
        temp.my_matrix[1][3] = ty;
        temp.my_matrix[2][3] = tz;
        return temp;
    }

    // Scaling
    static All_Transformations Scaling(All_Transformations temp, double sx, double sy, double sz)
    {
        // All_Transformations temp;
        temp.my_matrix[0][0] = sx;
        temp.my_matrix[1][1] = sy;
        temp.my_matrix[2][2] = sz;
        return temp;
    }

    // Rotation

    static All_Transformations Rotation(All_Transformations temp, double angle, double ax, double ay, double az)
    {
        double rad_ang = Rad_angle(angle);
        double cos_theta = cos(rad_ang);
        double sin_theta = sin(rad_ang);

        Point axis = Point(ax, ay, az);
        axis.normalize();

        temp.my_matrix[0][0] = cos_theta + (1 - cos_theta) * axis.x * axis.x;
        temp.my_matrix[0][1] = (1 - cos_theta) * axis.x * axis.y - sin_theta * axis.z;
        temp.my_matrix[0][2] = (1 - cos_theta) * axis.x * axis.z + sin_theta * axis.y;

        temp.my_matrix[1][0] = (1 - cos_theta) * axis.y * axis.x + sin_theta * axis.z;
        temp.my_matrix[1][1] = cos_theta + (1 - cos_theta) * axis.y * axis.y;
        temp.my_matrix[1][2] = (1 - cos_theta) * axis.y * axis.z - sin_theta * axis.x;

        temp.my_matrix[2][0] = (1 - cos_theta) * axis.z * axis.x - sin_theta * axis.y;
        temp.my_matrix[2][1] = (1 - cos_theta) * axis.z * axis.y + sin_theta * axis.x;
        temp.my_matrix[2][2] = cos_theta + (1 - cos_theta) * axis.z * axis.z;

        return temp;
    }

    All_Transformations operator*(const All_Transformations &other) const
    {
        All_Transformations result = All_Transformations(vector<vector<double>>(4, vector<double>(4, 0)));

        for (size_t i = 0; i < 4; ++i)
        {
            for (size_t j = 0; j < 4; ++j)
            {
                for (size_t k = 0; k < 4; ++k)
                {
                    result.my_matrix[i][j] += my_matrix[i][k] * other.my_matrix[k][j];
                }
            }
        }

        return result;
    }

    Point operator*(const Point point)
    {
        double t[4];
        for (int i = 0; i < 4; i++)
        {
            t[i] = 0.0;

            for (int j = 0; j < 4; j++)
            {
                t[i] += (j == 0) ? (my_matrix[i][j] * point.x) : (j == 1) ? (my_matrix[i][j] * point.y)
                                                             : (j == 2)   ? (my_matrix[i][j] * point.z)
                                                                          : (my_matrix[i][j] * point.w);
            }
        }
        return Point(t[0], t[1], t[2], t[3]);
    }

    // Stage 2
    static All_Transformations view_Transformation(Point eye, Point look, Point up)
    {
        Point l = (look - eye);
        l.normalize();
        Point r = l.cross_product(up);
        r.normalize();
        Point u = r.cross_product(l);

        All_Transformations T;
        T = Translation(T, -eye.x, -eye.y, -eye.z);

        // cout << "utils T" << endl;
        // T.print();
        All_Transformations R = All_Transformations();
        R.my_matrix[0][0] = r.x;
        R.my_matrix[0][1] = r.y;
        R.my_matrix[0][2] = r.z;

        R.my_matrix[1][0] = u.x;
        R.my_matrix[1][1] = u.y;
        R.my_matrix[1][2] = u.z;

        R.my_matrix[2][0] = -l.x;
        R.my_matrix[2][1] = -l.y;
        R.my_matrix[2][2] = -l.z;

        All_Transformations V = R * T;
        return V;
    }

    // Stage 3
    static All_Transformations Projection_Transformation(double fovY, double aspectRatio, double near, double far)
    {
        double fovX = fovY * aspectRatio;
        double fy = Rad_angle(fovY);
        double fx = Rad_angle(fovX);
        double t = near * tan(fy / 2);
        double r = near * tan(fx / 2);

        All_Transformations P = All_Transformations(vector<vector<double>>(4, vector<double>(4, 0)));
        P.my_matrix[0][0] = near / r;
        P.my_matrix[1][1] = near / t;
        P.my_matrix[2][2] = (-(far + near)) / (far - near);
        P.my_matrix[2][3] = (-(2 * far * near)) / (far - near);
        P.my_matrix[3][2] = -1;

        return P;
    }

    
};

struct Color
{
    int R;
    int G;
    int B;
};

struct Triangle
{
    Point vertices[3];
    Color color;
};

static unsigned long int g_seed = 1;
inline int Random()
{
    g_seed = (214013 * g_seed + 2531011);
    return (g_seed >> 16) & 0x7FFF;
}

pair<double, double> LeftIntersecting(const Triangle &tri, double ys)
{
    double min_x = INFINITY;
    double za; // min z

    for (int i = 0; i < 3; i++)
    {
        Point current = tri.vertices[i];
        Point next = tri.vertices[(i + 1) % 3];

        double x = (ys - current.y) * (next.x - current.x) / (next.y - current.y) + current.x;

        if (x >= min(current.x, next.x) && x <= max(current.x, next.x))
        {

            if ((current.y == next.y) && (current.y == ys))
            {
                // Horizontal case
                if (current.x < min_x)
                {
                    min_x = current.x;
                    za = current.z;
                }
                if (next.x < min_x)
                {
                    min_x = next.x;
                    za = next.z;
                }
            }
            else
            {
                // Not horizontal
                if (x < min_x)
                {
                    min_x = x;
                    za = (ys - current.y) * (next.z - current.z) / (next.y - current.y) + current.z;
                }
            }
        }
        if (min_x == INFINITY)
            continue;
    }

    return make_pair(min_x, za);
}

pair<double, double> RightIntersecting(const Triangle &tri, double ys)
{
    double max_x = -INFINITY;
    double zb; //max z

    for (int i = 0; i < 3; i++)
    {
        Point current = tri.vertices[i];
        Point next = tri.vertices[(i + 1) % 3];

        double x = (ys - current.y) * (next.x - current.x) / (next.y - current.y) + current.x;

        if (x >= min(current.x, next.x) && x <= max(current.x, next.x))
        {
            //double z = 
            if ((current.y == next.y) && (current.y == ys))
            {
                // Horizontal case
                if (current.x > max_x)
                {
                    max_x = current.x;
                    zb = current.z;
                }
                if (next.x > max_x)
                {
                    max_x = next.x;
                    zb = next.z;
                }
            }
            else
            {
                // Not horizontal
                if (x > max_x)
                {
                    max_x = x;
                    zb = (ys - current.y) * (next.z - current.z) / (next.y - current.y) + current.z;

                }
            }
        }
        if (max_x == -INFINITY)
            continue;
    }

    return make_pair(max_x, zb);
}