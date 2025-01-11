#ifndef C44A0E4C_40CA_4CE7_AAEF_37696C7DACCD
#define C44A0E4C_40CA_4CE7_AAEF_37696C7DACCD
#ifndef F8B47FFC_71DE_4DAC_B580_B747D557CCFC
#define F8B47FFC_71DE_4DAC_B580_B747D557CCFC

#include<stdio.h>
#include<math.h>
#include<vector>
#include<string>
#include<iostream>
#include<math.h>

#include <algorithm>
#include <fstream>

using namespace std;
#ifdef __linux__
    #include<GL/glut.h>
#elif WIN32
    #include <windows.h>
    #include <glut.h>
#endif

#define M_PI 3.14159265358979323846 


class Object;

vector<Object*> objects;

class Color{
public:
    double r, g, b;
    Color(){
        r = 0;
        g = 0;
        b = 0;
    }
    Color(double red, double green, double blue){
        r = red;
        g = green;
        b = blue;
    }

    void setColor(double red, double green, double blue){
        r = red;
        g = green;
        b = blue;
    }

    void setColor(Color c){
        r = c.r;
        g = c.g;
        b = c.b;
    }

    ~Color(){
        r = 0;
        g = 0;
        b = 0;

    }

    void ClipColor(){
        if(r > 1) r = 1;
        if(g > 1) g = 1;
        if(b > 1) b = 1;

        if(r < 0) r = 0;
        if(g < 0) g = 0;
        if(b < 0) b = 0;
    }

    Color operator+(Color c){
        Color temp;
        temp.r = r + c.r;
        temp.g = g + c.g;
        temp.b = b + c.b;
        return temp;
    }

    Color operator*(double scalar){
        Color temp;
        temp.r = r * scalar;
        temp.g = g * scalar;
        temp.b = b * scalar;
        return temp;
    }   
    Color operator*(Color c){
        Color temp;
        temp.r = r * c.r;
        temp.g = g * c.g;
        temp.b = b * c.b;
        return temp;
    }

    Color& operator+=(const Color& other) {
        // Add the RGB components of the two colors, ensuring they don't exceed 255
        this->r = min(255.0, this->r + other.r);
        this->g = min(255.0, this->g + other.g);
        this->b = min(255.0, this->b + other.b);
        return *this; // Return the current object by reference
    }


};


class Vector3D{
public:
    double x, y, z;
    Vector3D(){
        x = 0;
        y = 0;
        z = 0;
    }
    Vector3D(double x, double y, double z){
        this->x = x;
        this->y = y;
        this->z = z;
    }

    void setVector(double x, double y, double z){
        this->x = x;
        this->y = y;
        this->z = z;
    }

    void setVector(Vector3D v){
        x = v.x;
        y = v.y;
        z = v.z;
    }   

    void normalize(){
        double length = sqrt(x*x + y*y + z*z);
        x /= length;
        y /= length;
        z /= length;
    }   
    double euclidean_distance(Vector3D v){
        return sqrt((x - v.x)*(x - v.x) + (y - v.y)*(y - v.y) + (z - v.z)*(z - v.z));
    }

    Vector3D operator+(const Vector3D v){
        Vector3D temp;
        temp.x = x + v.x;
        temp.y = y + v.y;
        temp.z = z + v.z;
        return temp;
    }

    Vector3D operator-(const Vector3D v){
        Vector3D temp;
        temp.x = x - v.x;
        temp.y = y - v.y;
        temp.z = z - v.z;
        return temp;
    }

    Vector3D operator*(const double scalar){
        Vector3D temp;
        temp.x = x * scalar;
        temp.y = y * scalar;
        temp.z = z * scalar;
        return temp;
    }
    Vector3D operator*(const Vector3D& v) const {
        Vector3D temp;
        temp.x = x * v.x;
        temp.y = y * v.y;
        temp.z = z * v.z;
        return temp;
    }

    double operator*(Vector3D v){
        return x*v.x + y*v.y + z*v.z;
    } 
    

    Vector3D operator/(Vector3D v){
        Vector3D temp;
        temp.x = x / v.x;
        temp.y = y / v.y;
        temp.z = z / v.z;
        return temp;
    }

    Vector3D operator^(Vector3D v){
        Vector3D temp;
        temp.x = y*v.z - z*v.y;
        temp.y = z*v.x - x*v.z;
        temp.z = x*v.y - y*v.x;
        return temp;
    }



};

class Ray{
public:
    Vector3D origin, direction;
    Ray(){
        origin = Vector3D(0, 0, 0);
        direction = Vector3D(0, 0, 0);
    }
    Ray(Vector3D origin, Vector3D direction){
        this->origin = origin;
        this->direction = direction;
        this->direction.normalize();
    }

    void setRay(Vector3D origin, Vector3D direction){
        this->origin = origin;
        this->direction = direction;
    }

    void setRay(Ray r){
        origin = r.origin;
        direction = r.direction;
    }

    ~Ray(){
        origin = Vector3D(0, 0, 0);
        direction = Vector3D(0, 0, 0);
    }
};

class PointLight{
    double radius;
    int stacks,slices;
public:
    Vector3D light_pos;
    Color color;
    PointLight(){
        light_pos = Vector3D(0, 0, 0);
        color = Color(1, 1, 1);
        radius = 1;
        stacks = 4;
        slices = 12;
    }
    PointLight(Vector3D light_pos, Color color){
        this->light_pos = light_pos;
        this->color = color;
        radius = 1;
        stacks = 4;
        slices = 12;
    }

    void setLight(Vector3D light_pos, Color color){
        this->light_pos = light_pos;
        this->color = color;
    }

    void setLight(PointLight l){
        light_pos = l.light_pos;
        color = l.color;
    }

    ~PointLight(){
        light_pos = Vector3D(0, 0, 0);
        color = Color(0, 0, 0);
    }
    void draw(){
        Vector3D points[stacks+1][slices+1];
        for(int i = 0; i <= stacks; i++){
            double h = radius * sin( (double)i / (double)stacks * M_PI/2 );
            double r = radius * cos( (double)i / (double)stacks * M_PI/2 );

            for(int j = 0; j <= slices; j++){
                points[i][j].x = r * cos( (double)j / (double)slices * 2 * M_PI );
                points[i][j].y = r * sin( (double)j / (double)slices * 2 * M_PI );
                points[i][j].z = h;
            }

            glColor3f(color.r, color.g, color.b);

            for(int i = 0; i < stacks; i++){
                for (int j = 0; j < slices; j++)
                {
                    /* code */
                    glBegin(GL_QUADS);
                    {
                        //upper
                        glVertex3f(points[i][j].x + light_pos.x, points[i][j].y + light_pos.y, points[i][j].z + light_pos.z);
                        glVertex3f(points[i][j+1].x + light_pos.x, points[i][j+1].y + light_pos.y, points[i][j+1].z + light_pos.z);
                        glVertex3f(points[i+1][j+1].x + light_pos.x, points[i+1][j+1].y + light_pos.y, points[i+1][j+1].z + light_pos.z);
                        glVertex3f(points[i+1][j].x + light_pos.x, points[i+1][j].y + light_pos.y, points[i+1][j].z + light_pos.z);

                        //lower
                        glVertex3f(points[i][j].x + light_pos.x, points[i][j].y + light_pos.y, -points[i][j].z + light_pos.z);
                        glVertex3f(points[i+1][j].x + light_pos.x, points[i+1][j].y + light_pos.y, -points[i+1][j].z + light_pos.z);
                        glVertex3f(points[i+1][j+1].x + light_pos.x, points[i+1][j+1].y + light_pos.y, -points[i+1][j+1].z + light_pos.z);
                        glVertex3f(points[i][j+1].x + light_pos.x, points[i][j+1].y + light_pos.y, -points[i][j+1].z + light_pos.z);


                    }

                    glEnd();
                }
                
            }
        }
    }

};

class spotLight{
    double radius;
    int stacks,slices;
public:
    Vector3D light_pos, direction;
    Color color;
    double cutoff_angle;
    spotLight(){
        light_pos = Vector3D(0, 0, 0);
        direction = Vector3D(0, 0, 0);
        color = Color(1, 1, 1);
        radius = 1;
        stacks = 4;
        slices = 12;
        cutoff_angle = 30;
    }
    spotLight(Vector3D light_pos, Vector3D direction, Color color, double angle){
        this->light_pos = light_pos;
        this->direction = direction;
        this->color = color;
        this->cutoff_angle = angle;
        radius = 1;
        stacks = 4;
        slices = 12;
    }

    void setLight(Vector3D light_pos, Vector3D direction, Color color, double angle){
        this->light_pos = light_pos;
        this->direction = direction;
        this->color = color;
        this->cutoff_angle = angle;
    }

    void setLight(spotLight l){
        light_pos = l.light_pos;
        direction = l.direction;
        color = l.color;
        cutoff_angle = l.cutoff_angle;
    }

    ~spotLight(){
        light_pos = Vector3D(0, 0, 0);
        direction = Vector3D(0, 0, 0);
        color = Color(0, 0, 0);
        cutoff_angle = 0;
    }
    void draw(){
        Vector3D points[stacks+1][slices+1];
        for(int i = 0; i <= stacks; i++){
            double h = radius * sin( (double)i / (double)stacks * M_PI/2 );
            double r = radius * cos( (double)i / (double)stacks * M_PI/2 );

            for(int j = 0; j <= slices; j++){
                points[i][j].x = r * cos( (double)j / (double)slices * 2 * M_PI );
                points[i][j].y = r * sin( (double)j / (double)slices * 2 * M_PI );
                points[i][j].z = h;
            }

            glColor3f(color.r, color.g, color.b);

            for(int i = 0; i < stacks; i++){
                for (int j = 0; j < slices; j++)
                {
                    /* code */
                    glBegin(GL_QUADS);
                    {
                        //upper
                        glVertex3f(points[i][j].x + light_pos.x, points[i][j].y + light_pos.y, points[i][j].z + light_pos.z);
                        glVertex3f(points[i][j+1].x + light_pos.x, points[i][j+1].y + light_pos.y, points[i][j+1].z + light_pos.z);
                        glVertex3f(points[i+1][j+1].x + light_pos.x, points[i+1][j+1].y + light_pos.y, points[i+1][j+1].z + light_pos.z);
                        glVertex3f(points[i+1][j].x + light_pos.x, points[i+1][j].y + light_pos.y, points[i+1][j].z + light_pos.z);

                        //lower
                        glVertex3f(points[i][j].x + light_pos.x, points[i][j].y + light_pos.y, -points[i][j].z + light_pos.z);
                        glVertex3f(points[i+1][j].x + light_pos.x, points[i+1][j].y + light_pos.y, -points[i+1][j].z + light_pos.z);
                        glVertex3f(points[i+1][j+1].x + light_pos.x, points[i+1][j+1].y + light_pos.y, -points[i+1][j+1].z + light_pos.z);
                        glVertex3f(points[i][j+1].x + light_pos.x, points[i][j+1].y + light_pos.y, -points[i][j+1].z + light_pos.z);
                    }
                    glEnd();
                }
            }
        }
    }


    
};



int recursion;
Vector3D eye;
vector<PointLight> pointLights;
vector<spotLight> spotLights;




class Object{
    
public:
    Vector3D reference_point;
    Color color;
    double height, width, length;
    double coefficents[4];
    int shine;

    Object(){
        height = 0;
        width = 0;
        length = 0;
        coefficents[0] = 0; //ambient
        coefficents[1] = 0; //diffuse
        coefficents[2] = 0; //specular
        coefficents[3] = 0; //reflection
        shine = 0;
    }

    virtual void draw() = 0;
    virtual double intersect(Ray, Color&, int) = 0;

    Color PhongModel_RecursiveReflection(Ray r,Color& col,int level,Vector3D& normal,Vector3D& intersectionPoint,Color& intersectionPointColor){
        
        for(int i = 0;i<pointLights.size();i++){
            
            Ray ray1(pointLights[i].light_pos, intersectionPoint - pointLights[i].light_pos);

            double t;
            double min_t2 = INFINITY;
            for(int i=0;i<objects.size();i++){
                Color c;
                t = objects[i]->intersect(ray1,c,0);
                if(t>0 && t<min_t2){
                    min_t2 = t;
                }
            }

            Vector3D shadow_intersectionPoint = ray1.origin + ray1.direction*min_t2;
            double e = 0.0000001;
            if(ray1.origin.euclidean_distance(shadow_intersectionPoint)  < (ray1.origin.euclidean_distance(intersectionPoint) - e)){
                continue;
            }


            double lambert = max(0.0,normal * (ray1.direction*(-1)));

            Ray reflected_ray(intersectionPoint, ray1.direction - (normal*(2*(ray1.direction*normal))));
            double phongValue = pow(max(0.0, reflected_ray.direction * (reflected_ray.direction*(-1))), shine);
            col += (pointLights[i].color * (coefficents[1] * lambert)) * intersectionPointColor;
            col += (pointLights[i].color * (coefficents[2] * phongValue)) * intersectionPointColor;
        
        }

        for(int i = 0;i < spotLights.size();i++){
            Ray ray1(spotLights[i].light_pos, intersectionPoint - spotLights[i].light_pos);

            Vector3D spotLightDirection = spotLights[i].direction;
            spotLightDirection.normalize();
            double angle = acos(spotLightDirection * (ray1.direction*(-1)))*180/M_PI;
            if(angle > spotLights[i].cutoff_angle){
                continue;
            }
            double t,min_t2 = INFINITY;
            for(int i=0;i<objects.size();i++){
                Color c;
                t = objects[i]->intersect(ray1,c,0);
                if(t>0 && t<min_t2){
                    min_t2 = t;
                }
            }

            Vector3D shadow_intersectionPoint = ray1.origin + ray1.direction*min_t2;
            double e = 0.0000001;
            if(ray1.origin.euclidean_distance(shadow_intersectionPoint)  < (ray1.origin.euclidean_distance(intersectionPoint) - e)){
                continue;
            }

            double lambert = max(0.0,normal * (ray1.direction*(-1)));

            Ray reflected_ray(intersectionPoint, ray1.direction - normal*(2*(ray1.direction*normal)));
            double phongValue = pow(max(0.0, reflected_ray.direction * (reflected_ray.direction*(-1))), shine);
            col += (spotLights[i].color * (coefficents[1] * lambert)) * intersectionPointColor;
            col += (spotLights[i].color * (coefficents[2] * phongValue)) * intersectionPointColor;
        }


        //Recursive Reflection

        

        //cout << level << endl;    
        Vector3D reflected_ray = r.direction - normal*(2*(r.direction*normal));
        reflected_ray.normalize();
        Ray reflectedRay(intersectionPoint+reflected_ray, reflected_ray);

        int nearest = -1;
        double min_t2 = INFINITY;
        for(int i=0;i<objects.size();i++){
            Color c;
            //cout<<"print "<<objects.size()<<endl;
            double t = objects[i]->intersect(reflectedRay,c,0);
            if(t>0 && t<min_t2){
                min_t2 = t;
                nearest = i;
            }
        }

        if(nearest != -1){
            Color reflectionColor;
            min_t2 = objects[nearest]->intersect(reflectedRay, reflectionColor, level + 1);
            col += reflectionColor * coefficents[3];
       
        }

        col.ClipColor();
        return col;
    }

    void setColor(double red, double green, double blue){
        color.setColor(red, green, blue);
    }

    void setShine(int shine){
        this->shine = shine;
    }

    void setCoefficients(double ambient, double diffuse, double specular, double reflection){
        coefficents[0] = ambient;
        coefficents[1] = diffuse;
        coefficents[2] = specular;
        coefficents[3] = reflection;
    }

    

    ~Object(){
        reference_point = Vector3D(0, 0, 0);
        color = Color(0, 0, 0);
        height = 0;
        width = 0;
        length = 0;
        coefficents[0] = 0; //ambient
        coefficents[1] = 0; //diffuse
        coefficents[2] = 0; //specular
        coefficents[3] = 0; //reflection
        shine = 0;
    
    }

};




class Sphere: public Object{
    int slices;
    int stacks;

public:
    Sphere(Vector3D center,double radius){
        reference_point = center;
        height = radius;
        width = radius;
        length = radius;
        slices = 72;
        stacks = 24;
    }


    void draw(){
        Vector3D points[stacks+1][slices+1];
        for(int i = 0; i <= stacks; i++){
            double h = length * sin( (double)i / (double)stacks * M_PI/2 );
            double r = length * cos( (double)i / (double)stacks * M_PI/2 );

            for(int j = 0; j <= slices; j++){
                points[i][j].x = r * cos( (double)j / (double)slices * 2 * M_PI );
                points[i][j].y = r * sin( (double)j / (double)slices * 2 * M_PI );
                points[i][j].z = h;
            }

            glColor3f(color.r, color.g, color.b);

            for(int i = 0; i < stacks; i++){
                for (int j = 0; j < slices; j++)
                {
                    /* code */
                    glBegin(GL_QUADS);
                    {
                        //upper
                        glVertex3f(points[i][j].x + reference_point.x, points[i][j].y + reference_point.y, points[i][j].z + reference_point.z);
                        glVertex3f(points[i][j+1].x + reference_point.x, points[i][j+1].y + reference_point.y, points[i][j+1].z + reference_point.z);
                        glVertex3f(points[i+1][j+1].x + reference_point.x, points[i+1][j+1].y + reference_point.y, points[i+1][j+1].z + reference_point.z);
                        glVertex3f(points[i+1][j].x + reference_point.x, points[i+1][j].y + reference_point.y, points[i+1][j].z + reference_point.z);

                        //lower
                        glVertex3f(points[i][j].x + reference_point.x, points[i][j].y + reference_point.y,   reference_point.z-points[i][j].z);
                        glVertex3f(points[i][j+1].x + reference_point.x, points[i][j+1].y + reference_point.y, reference_point.z-points[i][j+1].z);
                        glVertex3f(points[i+1][j+1].x + reference_point.x, points[i+1][j+1].y + reference_point.y,  reference_point.z-points[i+1][j+1].z);
                        glVertex3f(points[i+1][j].x + reference_point.x, points[i+1][j].y + reference_point.y,  reference_point.z-points[i+1][j].z);

                    }
                    glEnd();
                }
            }
        }
        

    }

    


    double intersect(Ray r, Color& col, int level){
        //cout << "here "<< endl;
        double A,B,C,t1,t2,min_t;
        A = r.direction*r.direction;
        B = 2*(r.direction*(r.origin - reference_point));
        C = (r.origin - reference_point)*(r.origin - reference_point) - length*length;
        double det = B*B - 4*A*C;

        if(det < 0){
            min_t = INFINITY;
        }
        else{
            t1 = (-B + sqrt(det)) / (2*A);
            t2 = (-B - sqrt(det)) / (2*A);
            min_t = min(t1,t2);
        }


        //Illumination with Phong Model
        //cout << level<<endl;
        if(level == 0){
            return min_t;
        }

        Vector3D intersectionPoint = r.origin + r.direction*min_t;
        Color intersectionPointColor = color;
        col = intersectionPointColor*coefficents[0]; //ambient light
        //cout << col.r << col.b;
        Vector3D normal = intersectionPoint - reference_point;
        normal.normalize();
        if(eye.euclidean_distance(reference_point)<=length){
            normal = normal*(-1);
        }



        if(level>=recursion){
            //cout<<level<<"   "<< recursion<<endl;
            //cout << "value "<<endl;
            return min_t;
        }

        col = PhongModel_RecursiveReflection(r,col,level,normal,intersectionPoint,intersectionPointColor);
        col.ClipColor();

        
        return min_t;
    }

};


class Triangle: public Object{  
public:
    Vector3D a, b, c;

    Triangle(Vector3D a, Vector3D b, Vector3D c){
        this->a = a;
        this->b = b;
        this->c = c;
    }

    void draw(){
        glColor3f(color.r, color.g, color.b);
        glBegin(GL_TRIANGLES);
        {
            glVertex3f(a.x, a.y, a.z);
            glVertex3f(b.x, b.y, b.z);
            glVertex3f(c.x, c.y, c.z);
        }
        glEnd();
    }

    double intersect(Ray r,Color& col,int level){
        double dA, dB, dG, dT, beta, gamma, t,min_t;


        dA = ((a.x - b.x) * ( (a.y - c.y) * r.direction.z - (a.z - c.z) * r.direction.y ))-((a.x - c.x) * ( (a.y - b.y) * r.direction.z - (a.z - b.z) * r.direction.y )) + (r.direction.x * ( (a.y - b.y) * (a.z - c.z) - (a.z - b.z) * (a.y - c.y) ));
        dB = ((a.x - r.origin.x) * ( (a.y - c.y) * r.direction.z - (a.z - c.z) * r.direction.y )) - ((a.x - c.x) * ( (a.y - r.origin.y) * r.direction.z - (a.z - r.origin.z) * r.direction.y )) + (r.direction.x * ( (a.y - r.origin.y) * (a.z - c.z) - (a.z - r.origin.z) * (a.y - c.y) ));
        dG = ((a.x - b.x) * ( (a.y - r.origin.y) * r.direction.z - (a.z - r.origin.z) * r.direction.y )) - ((a.x - r.origin.x) * ( (a.y - b.y) * r.direction.z - (a.z - b.z) * r.direction.y )) + (r.direction.x * ( (a.y - b.y) * (a.z - r.origin.z) - (a.z - b.z) * (a.y - r.origin.y) ));
        dT = ((a.x - b.x) * ( (a.y - c.y) * (a.z - r.origin.z) - (a.z - c.z) * (a.y - r.origin.y) )) - ((a.x - c.x) * ( (a.y - b.y) * (a.z - r.origin.z) - (a.z - b.z) * (a.y - r.origin.y) )) + ((a.x - r.origin.x) * ( (a.y - b.y) * (a.z - c.z) - (a.z - b.z) * (a.y - c.y) )) ;

        if(dA ==0){
            min_t = INFINITY;
        }
        else {
            beta = dB/dA;
            gamma = dG/dA;
            min_t = dT/dA;

            if(beta <= 0 || gamma <= 0 || beta + gamma >= 1){
                min_t = INFINITY;
            }
        }

        //Illumination with Phong Model

        if(level == 0){
            return min_t;
        }

        Vector3D intersectionPoint = r.origin + r.direction*min_t;
        Color intersectionPointColor = color;
        col = intersectionPointColor*coefficents[0]; //ambient light

        Vector3D normal = (b - a)^(c - a);
        normal.normalize();
        if(normal*(r.origin*(-1))<0){
            normal = normal*(-1);
        }

        

        //Recursive Reflection
        if(level >= recursion){
            //cout<<"Triangle "<<level <<" "<<recursion<<endl;
            return min_t;
        }

        col = PhongModel_RecursiveReflection(r,col,level,normal,intersectionPoint,intersectionPointColor);

        // clipping color vlaues
        col.ClipColor();

        return min_t;
    }
    
};



class GeneralQuadratic: public Object{
    double A, B, C, D, E, F, G, H, I, J;
    Vector3D reference_point;
    double length,width,height;

public:
    GeneralQuadratic(double A, double B, double C, double D, double E, double F, double G, double H, double I, double J,Vector3D reference_point, double length, double width, double height){
        this->A = A;
        this->B = B;
        this->C = C;
        this->D = D;
        this->E = E;
        this->F = F;
        this->G = G;
        this->H = H;
        this->I = I;
        this->J = J;

        this->reference_point = reference_point;
        this->length = length;
        this->width = width;
        this->height = height;

        
    }
    void checkclip(Ray r,double &Max_t,double &Min_t){
        if(Min_t < INFINITY){
            if(Max_t < INFINITY){
               if(Min_t > 0){
                    Vector3D intersectionPoint = r.origin + r.direction*Min_t;
                    if((length !=0 && (intersectionPoint.x<reference_point.x || intersectionPoint.x>reference_point.x+length)) || 
                    (width !=0 && (intersectionPoint.y<reference_point.y || intersectionPoint.y>reference_point.y+width)) || 
                    (height !=0 && (intersectionPoint.z<reference_point.z || intersectionPoint.z>reference_point.z+height)))
                        Min_t = INFINITY;
               }

             if(Max_t > 0){
                    Vector3D intersectionPoint = r.origin + r.direction*Max_t;
                    if((length !=0 && (intersectionPoint.x < reference_point.x || intersectionPoint.x > reference_point.x + length)) || 
                    (width !=0 && (intersectionPoint.y < reference_point.y || intersectionPoint.y > reference_point.y + width)) || 
                    (height !=0 && (intersectionPoint.z < reference_point.z || intersectionPoint.z > reference_point.z + height)))
                        Max_t = INFINITY;
            }
            if(Min_t < 0 || Min_t > Max_t){
                Min_t = Max_t;
            }
                   
               
            }

            else{
                if(Min_t > 0){
                    Vector3D intersectionPoint = r.origin + r.direction*Min_t;
                    if(length != 0 && (intersectionPoint.x < reference_point.x || intersectionPoint.x > reference_point.x + length)){
                        Min_t = INFINITY;
                    }
                    if(width != 0 && (intersectionPoint.y < reference_point.y || intersectionPoint.y > reference_point.y + width)){
                        Min_t = INFINITY;
                    }
                    if(height != 0 && (intersectionPoint.z < reference_point.z || intersectionPoint.z > reference_point.z + height)){
                        Min_t = INFINITY;
                    }
                }
            }
        }
    }
    void draw(){
        //not implemented
    }

    double intersect(Ray r, Color& col, int level){

        double Min_t,Max_t;
        double a = A*r.direction.x*r.direction.x + B*r.direction.y*r.direction.y + C*r.direction.z*r.direction.z + D*r.direction.x*r.direction.y + E*r.direction.x*r.direction.z + F*r.direction.y*r.direction.z;
        double b = 2*(A*r.direction.x*r.origin.x + B*r.direction.y*r.origin.y + C*r.direction.z*r.origin.z + D*(r.direction.x*r.origin.y + r.direction.y*r.origin.x) + E*(r.direction.z*r.origin.x + r.direction.x*r.origin.z) + F*(r.direction.y*r.origin.z + r.direction.z*r.origin.y) + G*r.direction.x + H*r.direction.y + I*r.direction.z);
        double c = A*r.origin.x*r.origin.x + B*r.origin.y*r.origin.y + C*r.origin.z*r.origin.z + D*r.origin.x*r.origin.y + E*r.origin.x*r.origin.z + F*r.origin.y*r.origin.z + G*r.origin.x + H*r.origin.y + I*r.origin.z + J;

        double det = b*b - 4*a*c;
        if(det < 0){
            Min_t = INFINITY;
            Max_t = INFINITY;
        }

        else if(det == 0){
            Min_t = -b/(2*a);
            Max_t = INFINITY;
        }

        else{
        Min_t = min((-b - sqrt(det)) / (2 * a), (-b + sqrt(det)) / (2 * a));
        Max_t = max((-b - sqrt(det)) / (2 * a), (-b + sqrt(det)) / (2 * a));
        
        }

       checkclip(r,Max_t,Min_t);


        //Illumination with Phong Model
        if(level == 0){
            return Min_t;
        }

        Vector3D intersectionPoint = r.origin + r.direction*Min_t;
        Color intersectionPointColor = color;
        col = intersectionPointColor*coefficents[0]; //ambient light

        Vector3D normal = Vector3D(2*A*intersectionPoint.x + D*intersectionPoint.y + E*intersectionPoint.z + G, 2*B*intersectionPoint.y + D*intersectionPoint.x + F*intersectionPoint.z + H, 2*C*intersectionPoint.z + E*intersectionPoint.x + F*intersectionPoint.y + I);
        normal.normalize();
        if(normal*(r.direction*(-1))<0){
            normal = normal*(-1);
        }

        

        //Recursive Reflection
        if(level>=recursion)
            return Min_t;

        col = PhongModel_RecursiveReflection(r,col,level,normal,intersectionPoint,intersectionPointColor);
        col.ClipColor();
        return Min_t;

    }

};


class Floor:public Object{
    double tileWidth;
    double floorWidth;
    int noOfTiles = int(floorWidth/tileWidth);

public:
    Floor(double floorWidth, double tileWidth){
        this->floorWidth = floorWidth;
        this->tileWidth = tileWidth;
        noOfTiles = floorWidth/tileWidth;

        reference_point = Vector3D(-floorWidth/2, -floorWidth/2, 0);

        length = tileWidth;
    }

    void draw(){
        for(int i = 0; i < noOfTiles; i++){
            for(int j = 0; j < noOfTiles; j++){
                if((i+j)%2 == 0){
                    glColor3f(1, 1, 1);
                }
                else{
                    glColor3f(0, 0, 0);
                }
                glBegin(GL_QUADS);
                {
                    glVertex3f(reference_point.x + i*tileWidth, reference_point.y + j*tileWidth, reference_point.z);
                    glVertex3f(reference_point.x + (i+1)*tileWidth, reference_point.y + j*tileWidth, reference_point.z);
                    glVertex3f(reference_point.x + (i+1)*tileWidth, reference_point.y + (j+1)*tileWidth, reference_point.z);
                    glVertex3f(reference_point.x + i*tileWidth, reference_point.y + (j+1)*tileWidth, reference_point.z);
                }
                glEnd();
            }
        }
    }

    double intersect(Ray r, Color& col, int level){
        Vector3D normal = Vector3D(0, 0, 1);
        if(eye*normal < 0 )
            normal = normal*-1;


        double min_t = INFINITY;

        if(normal*r.direction != 0){
            min_t = (normal*r.origin)/(normal*r.direction*(-1));
        }

        Vector3D intersectionPoint = r.origin + r.direction*min_t;

        if(min_t > 0 && min_t < INFINITY){
            if(intersectionPoint.x > -reference_point.x || intersectionPoint.x < reference_point.x || intersectionPoint.y > -reference_point.y || intersectionPoint.y < reference_point.y){
                min_t = INFINITY;
            }
        }

        //Illumination with Phong Model

        if(level == 0){
            return min_t;
        }

        Color intersectionPointColor;
        Vector3D ref_intersectionPoint = intersectionPoint - reference_point;
        if((int)(ref_intersectionPoint.x/tileWidth) % 2 == 0){
            if((int)(ref_intersectionPoint.y/tileWidth) % 2 == 0){
                intersectionPointColor = Color(1, 1, 1);
            }
            else{
                intersectionPointColor = Color(0, 0, 0);
            }
        }
        else{
            if((int)(ref_intersectionPoint.y/tileWidth) % 2 == 0){
                intersectionPointColor = Color(0, 0, 0);
            }
            else{
                intersectionPointColor = Color(1, 1, 1);
            }
        }

        col = intersectionPointColor*coefficents[0]; //ambient light

        

        //Recursive Reflection

        if(level>=recursion){
            //cout<<level<<"   "<< recursion<<endl;
            //cout << "value "<<endl;
            return min_t;
        }

        //cout << level << endl;    
        col = PhongModel_RecursiveReflection(r,col,level,normal,intersectionPoint,intersectionPointColor);
        col.ClipColor();
        return min_t;
    }


};
#endif /* F8B47FFC_71DE_4DAC_B580_B747D557CCFC */


#endif /* C44A0E4C_40CA_4CE7_AAEF_37696C7DACCD */
