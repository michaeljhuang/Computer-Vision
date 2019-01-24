/* Michael Huang 
Dr. Zacharias
Pd 5 Computer Vision
19 September 2018*/
#include <iostream>// Packages to include
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <cmath>


using namespace std;
int size = 800;//size of the image
vector<pair<int,int> > points;// Points
int image[800][800];// Picture grid

double dist(int x1, int y1, int x2, int y2) {
   return sqrt((double)((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));// Distance Formula given coordinates
}
//Bressenham 
void draw_line(int xstart, int ystart, int xend, int yend){
      
   int x1;
   int y1;
   int x2;
   int y2;
      
   int dX = abs(xend - xstart);// Delta X,Y
   int dY = abs(yend - ystart);
      
   double m = (double)dY/dX;// Slope
      	//driving axis
   int index;
   int multipler = 1;
   if(dY > dX) {// Casework to decide the points that make Bressenham work
      if(ystart > yend) {
         x1 = xend;
         y1 = yend;
         x2 = xstart;
         y2 = ystart;
      }
      else {// Decide what points are which
         x1 = xstart;
         y1 = ystart;
         x2 = xend;
         y2 = yend;
      }
      if(x1 > x2) {
         multipler = -1;
      }
      index = x1;
      m = (double)dX/dY;
   }
   else {
      if(xstart > xend) {
         x1 = xend;
         y1 = yend;
         x2 = xstart;
         y2 = ystart;
      }
      else {
         x1 = xstart;
         y1 = ystart;
         x2 = xend;
         y2 = yend;
      }
      if(y1 > y2) {
         multipler = -1;
      }
      else {
         multipler = 1;
      }
      index = y1;
   }
   double epsilon = m - 1.0;
      
   if(index == x1) {
      for(int k = y1; k < y2 - 1; k++) {
         image[index][k] = 0;
         if(epsilon >= 0) {
            index+= multipler;
            epsilon--;
         }
         epsilon += m;
      }
   }
   else {// Drawing
      for(int k = x1; k < x2 - 1; k++) {
         image[k][index] = 0;
         if(epsilon >= 0) {
            index+= multipler;
            epsilon--;
         }
         epsilon += m;
      }
   }
}
void draw_circle(int xc, int yc, int r){// Bresenham Circle
   int d = 3 - 2 * r;
   int xcur = 0; 
   int ycur = r;
   while(ycur >= xcur) {
      if(0<=xc+xcur&&xc+xcur<=800 &&0<=yc+ycur && yc+ycur<=800)// BOUNDARY CHECKS
         image[xc + xcur][yc + ycur] = 0;
      if(0<=xc-xcur&&xc-xcur<=800 &&0<=yc+ycur && yc+ycur<=800)
         image[xc - xcur][yc + ycur] = 0;
      if(0<=xc+xcur&&xc+xcur<=800 &&0<=yc-ycur && yc-ycur<=800)
         image[xc + xcur][yc - ycur] = 0;
      if(0<=xc-xcur&&xc-xcur<=800 &&0<=yc-ycur && yc-ycur<=800)
         image[xc - xcur][yc - ycur] = 0;
      if(0<=xc+ycur&&xc+ycur<=800 &&0<=yc+xcur && yc+xcur<=800)
         image[xc + ycur][yc + xcur] = 0;
      if(0<=xc-ycur&&xc-ycur<=800 &&0<=yc+xcur && yc+xcur<=800)
         image[xc - ycur][yc + xcur] = 0;
      if(0<=xc+ycur&&xc+ycur<=800 &&0<=yc-xcur && yc-xcur<=800)
         image[xc + ycur][yc - xcur] = 0;
      if(0<=xc-ycur&&xc-ycur<=800 &&0<=yc-xcur && yc-xcur<=800)
         image[xc - ycur][yc - xcur] = 0;
   
      xcur++;
   
      if(d > 0) {
         ycur--;
         d = d + 4 * (xcur - ycur) + 10;
      }
      else 
         d = d + 4 * xcur + 6; 
   }
}

int main(){
	//Random seeding so that it does not generate the same random numbers every time
   srand(time(NULL));
   
	//Makes the field white
   for(int i = 0; i < size; i++) {
      for(int j = 0; j < size; j++) {
         image[i][j] = 1;
      }
   }
   for(int i = 0; i < 3; i++) {
      points.push_back(make_pair(rand() % size + 1, rand() % size + 1));// Make Points
   }

   cout << "Coordinates: " << endl;// Print out Coordinates
   for(int i = 0; i < points.size(); i++) {
      cout << points[i].first << ' ' << points[i].second << endl;
   }
   draw_line(points[0].first,points[0].second,points[1].first,points[1].second);
   draw_line(points[0].first,points[0].second,points[2].first,points[2].second);
   draw_line(points[1].first,points[1].second,points[2].first,points[2].second);// Drawing Triangle
   //Relelvant Measurements
   double sideA = dist(points[1].first, points[1].second, points[2].first, points[2].second);
   double sideB = dist(points[0].first, points[0].second, points[2].first, points[2].second);
   double sideC = dist(points[0].first, points[0].second, points[1].first, points[1].second);
   double total = sideA + sideB + sideC;
   double semiperimeter = total / 2.0;
   double area = sqrt(semiperimeter * (semiperimeter - sideA) * (semiperimeter - sideB) * (semiperimeter - sideC));

	//INCIRCLE from internet
   int x_inc = (int)((sideA * points[0].first + sideB * points[1].first + sideC * points[2].first) / total);
   int y_inc = (int)((sideA * points[0].second + sideB * points[1].second + sideC * points[2].second) / total);
   int r_inc = (int)(area / semiperimeter);
   draw_circle(x_inc,y_inc,r_inc);
   //Circumcircle from Wikipedia
   double denom = 2.0 * (points[0].first * (points[1].second - points[2].second)
      + points[1].first * (points[2].second - points[0].second) + points[2].first * (points[0].second - points[1].second));
   int x_cir = (int)(((points[0].first*points[0].first + points[0].second*points[0].second) * (points[1].second - points[2].second) 
      + ((points[1].first*points[1].first + points[1].second*points[1].second) * (points[2].second - points[0].second)) 
      + ((points[2].first*points[2].first + points[2].second*points[2].second) * (points[0].second - points[1].second))) / denom);
   int y_cir = (int)(((points[0].first*points[0].first + points[0].second*points[0].second) * (points[2].first - points[1].first) 
      + ((points[1].first*points[1].first + points[1].second*points[1].second) * (points[0].first - points[2].first)) 
      + ((points[2].first*points[2].first + points[2].second*points[2].second) * (points[1].first - points[0].first))) / denom);
   int r_cir = (int)(sideA * sideB * sideC / (4 * area));
   draw_circle(x_cir,y_cir,r_cir);  
   // EULER LINE
   double xcen = (double) (points[0].first + points[1].first + points[2].first) / 3.0;
   double ycen = (double) (points[0].second + points[1].second + points[2].second) / 3.0;

   double slope = (ycen - 1.0 * y_cir) / (xcen - 1.0 * x_cir);
   double intercept = ycen - slope * xcen;

   vector< pair<int, int> > EulerPoints;

   int xp;
   int yp;
   //case1: y = 0, x within range
   xp = (int)(-1.0 * intercept / slope);
   if(xp >= 0 && xp < size) {
      EulerPoints.push_back(make_pair(xp, 0));
   }
	//case2: y = size, x within range
   xp = (int)((1.0 * size - intercept) / slope);
   if(xp >= 0 && xp < size) {
      EulerPoints.push_back(make_pair(xp, size));
   }
	//case3: x = 0, y within range
   yp = (int) intercept;
   if(yp >= 0.0 && yp < size) {
      EulerPoints.push_back(make_pair(0, intercept));
   }
	//case4: x = size, y within range
   yp = (int)(slope * 1.0 * size + intercept);
   if(yp >= 0 && yp < size) {
      EulerPoints.push_back(make_pair(size, yp));
   }
   draw_line(EulerPoints[0].first,EulerPoints[0].second,EulerPoints[1].first,EulerPoints[1].second);
   string header = "P3  800  800  1";//Printsout
   ofstream file("EulerLine.ppm");
   file << header << "\n";
   for(int i = 0; i < size; i++) {
      string out = "";
      for(int j = 0; j < size; j++) {
         if(image[i][j] == 1)
            out += "1 1 1 ";
         else if(image[i][j] == 0)
            out += "0 0 0 ";
      }
      file << out;
      file << "\n";
   }
   file.close();
   return 0;
}


