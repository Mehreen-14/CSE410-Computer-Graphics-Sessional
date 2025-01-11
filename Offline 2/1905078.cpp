/*
Mehreen Tabassum Maliha
1905078
*/

#include <bits/stdc++.h>
#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>

#define FOLDER 5

#include "bitmap_image.hpp"

#include "utils.cpp"

using namespace std;

int main()
{

    
        string sceneFilePath = to_string(FOLDER) + "/scene.txt";
        string configFilePath = to_string(FOLDER) + "/config.txt";
        string stage1FilePath = to_string(FOLDER) + "/stage1.txt";
        string stage2FilePath = to_string(FOLDER) + "/stage2.txt";
        string stage3FilePath = to_string(FOLDER) + "/stage3.txt";
        string zBufferFilePath = to_string(FOLDER) + "/z_buffer.txt";
        string bmpFilePath = to_string(FOLDER) + "/out.bmp";

        // string extraPath = to_string(FOLDER) + "/extra.txt";

        ifstream input1;
        input1.open(sceneFilePath);
        ofstream output, out;
        output.open(stage1FilePath);

        // Stage 1 Complete

        Point eye, look, up;
        double fovY, aspectRatio, near, far;

        output << fixed << setprecision(7);

        // output<<"My file"<<endl;
        // eye,look,up
        input1 >> eye.x >> eye.y >> eye.z;
        input1 >> look.x >> look.y >> look.z;
        input1 >> up.x >> up.y >> up.z;

        input1 >> fovY >> aspectRatio >> near >> far;

        stack<All_Transformations> Stack;
        All_Transformations A;
        Stack.push(A);
        // A.print();
        string ins = "";
        int triangle_cnt = 0;
        while (true)
        {
            /* code */
            input1 >> ins;

            if (ins == "triangle")
            {
                triangle_cnt++;

                Point p[3];
                for (int i = 0; i < 3; i++)
                {
                    input1 >> p[i].x >> p[i].y >> p[i].z;
                    p[i] = Stack.top() * p[i];
                    p[i].Scaling_point();
                    output << p[i].x << " " << p[i].y << " " << p[i].z << " " << endl;
                }
                output << endl;
            }
            else if (ins == "scale")
            {
                double sx, sy, sz;
                input1 >> sx >> sy >> sz;
                All_Transformations S;
                S = S.Scaling(S, sx, sy, sz);
                // cout << "Scale" << endl;
                // S.print();

                All_Transformations temp = Stack.top() * S;
                // cout << "top" << endl;
                // Stack.top().print();
                // cout << "ins==scale" << endl;
                // temp.print();
                Stack.pop();
                Stack.push(temp);
            }
            else if (ins == "rotate")
            {
                double angle, ax, ay, az;
                input1 >> angle >> ax >> ay >> az;
                All_Transformations R;
                R = R.Rotation(R, angle, ax, ay, az);

                All_Transformations temp = Stack.top() * R;
                Stack.pop();
                Stack.push(temp);
            }
            else if (ins == "translate")
            {
                double tx, ty, tz;
                input1 >> tx >> ty >> tz;
                All_Transformations T;
                T = T.Translation(T, tx, ty, tz);

                All_Transformations temp = Stack.top() * T;
                Stack.pop();
                Stack.push(temp);
            }
            else if (ins == "push")
            {
                A = Stack.top(); // Save the current transformation matrix
                Stack.push(A);
                // cout << "pushed" << endl;
            }

            else if (ins == "pop")
            {
                Stack.pop();
            }

            else if (ins == "end")
            {
                break;
            }
            else
            {
                cout << "Invalid Instruction" << endl;
                break;
            }
        }

        input1.close();
        output.close();

        // cout<<triangle_cnt<<endl;
        // Stage 2: View Transformation

        input1.open(stage1FilePath);
        output.open(stage2FilePath);

        All_Transformations View;
        View = View.view_Transformation(eye, look, up);

        for (int i = 0; i < triangle_cnt; i++)
        {
            Point p[3];
            for (int j = 0; j < 3; j++)
            {
                input1 >> p[j].x >> p[j].y >> p[j].z;
                p[j] = View * (p[j]);
                p[j].Scaling_point();
                output << p[j].x << " " << p[j].y << " " << p[j].z << " " << endl;
            }
            output << endl;
        }

        input1.close();
        output.close();

        // Stage 3: Projection Transformation

        input1.open(stage2FilePath);
        output.open(stage3FilePath);

        All_Transformations projection;
        projection = projection.Projection_Transformation(fovY, aspectRatio, near, far);

        for (int i = 0; i < triangle_cnt; i++)
        {
            Point p[3];
            for (int j = 0; j < 3; j++)
            {
                input1 >> p[j].x >> p[j].y >> p[j].z;
                p[j] = projection * (p[j]);
                p[j].Scaling_point();
                output << p[j].x << " " << p[j].y << " " << p[j].z << " " << endl;
            }
            output << endl;
        }

        input1.close();
        output.close();

        double screen_width, screen_height;
        // Stage 4: Clipping & scan conversion using Z-buffer algorithm
        input1.open(configFilePath);
        input1 >> screen_width >> screen_height;
        input1.close();

        double x_left = -1.0, x_right = 1.0;
        double y_top = 1.0, y_bottom = -1.0;
        double z_front_limit = -1.0, z_max = 1.0;

        /*
        a. Create a pixel mapping between the x-y range values and the Screen_Width X Screen_height range.
                        dx = (right limit - left limit along X-axis) / Screen_Width
                        dy = (top limit - bottom limit along Y-axis) / Screen_Height
            Besides, specify Top_Y and Left_X values.
                    Top_Y = top limit along Y-axis - dy/2
                    Left_X = left limit along X-axis + dx/2
        */

        double dx, dy, Top_Y, Bottom_Y, Left_X, Right_X;

        dx = (x_right - x_left) / screen_width;
        dy = (y_top - y_bottom) / screen_height;
        Top_Y = y_top - dy / 2.0;
        Bottom_Y = y_bottom + dy / 2.0;
        Left_X = x_left + dx / 2.0;
        Right_X = x_right - dx / 2.0;

        /*
        b. During scanning from top to bottom and left to right, check for the middle values of each cell.
           e.g. Top_Y- row_no*dy, Left_X+col_no*dx
        */

        // for (int row_no = 0; row_no < screen_height; ++row_no)
        // {
        //     for (int col_no = 0; col_no < screen_width; ++col_no)
        //     {
        //         // Calculate middle values of each cell
        //         double middle_y = Top_Y - row_no * dy;
        //         double middle_x = Left_X + col_no * dx;

        //         // Print or perform further processing with middle_x and middle_y
        //         // cout << "Middle values: " << middle_x << " " << middle_y << endl;
        //     }
        // }

        /*
        c. Create a z-buffer, a two dimensional array of Screen_Width X Screen_Height dimension.
            Initialize all the values in z-buffer with z_max. In the aforementioned examples, z_max = 2.0.
            The memory for z-buffer should be dynamically allocated (using STL is allowed).

        */

        vector<vector<double>> zBuffer(screen_height, vector<double>(screen_width, z_max));
        Color frame_buffer[(int)screen_height][(int)screen_width];
        // Create a bitmap_image object with Screen_Width X Screen_Height resolution and initialize its background color with black.

        bitmap_image image(static_cast<unsigned int>(screen_width), static_cast<unsigned int>(screen_height));

        image.set_all_channels(0, 0, 0);

        input1.open(stage3FilePath);

        Triangle tri[triangle_cnt];

        // Triangle tri[triangle_cnt];
        for (int i = 0; i < triangle_cnt; i++)
        {

            double max_y = -INFINITY;
            double min_y = INFINITY;
            for (int j = 0; j < 3; j++)
            {
                input1 >> tri[i].vertices[j].x >> tri[i].vertices[j].y >> tri[i].vertices[j].z;
            }
            tri[i].color.R = Random();
            tri[i].color.G = Random();
            tri[i].color.B = Random();

            for (int j = 0; j < 3; j++)
            {
                min_y = min(tri[i].vertices[j].y, min_y);
                // cout << j << " " << tri[i].vertices[j].y << endl;
                max_y = max(max_y, tri[i].vertices[j].y);
            }

            int top_scanline, bottom_scanline;

            
            if(Top_Y<max_y){
                top_scanline = 0;
            }
            else if(max_y<Top_Y){
                top_scanline = (int)ceil((Top_Y - max_y) / dy);
            }
            
            if(Bottom_Y>min_y){
                bottom_scanline = screen_height;
            }
            else if(min_y>Bottom_Y){
                bottom_scanline = screen_height - (int)ceil((min_y-Bottom_Y) / dy);
            }

            


            // cout << top_scanline << " mehreen " << bottom_scanline << endl;

            for (int row_no = top_scanline; row_no <= bottom_scanline; row_no++)
            {

                double ys = Top_Y - row_no * dy; // during scanning from top to bottom, we have to check the middle of each ceil
                // cout<<"y scanline "<<y_scanline<<endl;
                double xa;//min_x
                double xb;//max_x

                double za, zb;

                xa = LeftIntersecting(tri[i], ys).first;
                xb = RightIntersecting(tri[i], ys).first;

                za = LeftIntersecting(tri[i], ys).second;
                zb = RightIntersecting(tri[i], ys).second;

                

                int left_intersecting_column ,right_intersecting_column;

                if(Left_X>xa){
                    left_intersecting_column = 0;
                }
                else if(Left_X<xa){
                    left_intersecting_column = (int)round((xa - Left_X) / dx);
                }

                if(Right_X<xb){
                    right_intersecting_column = screen_width;
                }
                else if(Right_X>xb){
                    right_intersecting_column = screen_width - (int)round((Right_X -  xb) / dx);

                }

                for (int col_no = left_intersecting_column; col_no < right_intersecting_column; col_no++)
                {

                    // During scanning from left to right, we have to check the middle values of each cell.
                    double xp = Left_X + col_no * dx; // x coordinate of the scanline
                    //  current column
                    double z = (xp - xa) * (zb - za) / (xb - xa) + za;

                    // calculation for zbuffer
                    if ((z >= z_front_limit) && (z <= z_max) && (z < zBuffer[row_no][col_no]))
                    {
                        // Calculating current z value is closer than previous z value for this pixel

                        zBuffer[row_no][col_no] = z;

                        // color

                        frame_buffer[row_no][col_no].R = tri[i].color.R;
                        frame_buffer[row_no][col_no].G = tri[i].color.G;
                        frame_buffer[row_no][col_no].B = tri[i].color.B;

                        image.set_pixel(col_no, row_no, frame_buffer[row_no][col_no].R, frame_buffer[row_no][col_no].G, frame_buffer[row_no][col_no].B);
                    }
                }
            }
        }
        input1.close();

        output.open(zBufferFilePath);
        // out.open(extraPath);
        output << fixed << setprecision(6);

        out << FOLDER << ":" << endl;

        for (int j = 0; j < screen_height; j++)
        {
            for (int k = 0; k < screen_width; k++)
            {

                if (zBuffer[j][k] < z_max)
                {
                    // out << "J " << j + 1 << " K " << k + 1 << endl;
                    output << zBuffer[j][k] << '\t';
                    // cout<<zBuffer[j][k]<< '\t';
                }
            }
            output << endl;
            // cout<<endl;
        }

        output.close();
        // out.close();

        image.save_image(bmpFilePath);

        zBuffer.clear();
        image.clear();
    

    return 0;
}