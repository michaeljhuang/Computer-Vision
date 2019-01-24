// Michael Huang Pd 5

#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <unordered_set>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <algorithm>

using namespace std;

int CMAX;
int XMAX;
int YMAX;
//initializing
vector<vector<vector<int> > > orig_image;
vector<vector<double> > gray_image;
vector<vector<double> > dx;
vector<vector<double> > dy;
vector<vector<double> > magnitudes;
vector<vector<double> > angles;

vector<vector<int> > xm;
vector<vector<int> > ym;


void makeImage(vector<vector<vector<int> > > image, string file_name)
{
   ofstream out;
   out.open(file_name);
   out << "P3 " << XMAX << " " << YMAX << " " << CMAX << "\n";
   for(int y = 0; y < YMAX; y++)
   {
      for(int x = 0; x < XMAX; x++)
         for(int z = 0; z < 3; z++)
            out << image[y][x][z] << " ";
      out << "\n";
   }
   out.close();
}

bool setPixel(double x, double y, int v1, int v2, int v3, vector<vector<vector<int> > > image)
{
   if(0<=x&&x<XMAX&&0<=y&&y<YMAX)
   {
      image[(int) x][(int) y][0] = (int) v1;
      image[(int) x][(int) y][1] = (int) v2;
      image[(int) x][(int) y][2] = (int) v3;
      return true;
   }
   return false;
}

pair<double, double> randPos()
{
   return make_pair(rand()%XMAX, rand()%YMAX);
}

pair<double, double> randPos(int x, int y, int radius)
{
   double r = (rand()%(radius*100))/100.0;
   double angle = (rand()%36000)/100.0;
   return make_pair((int)(x+r*cos(angle)), (int)(y+r*sin(angle)));
}

void drawLine(double x1, double y1, double x2, double y2, vector<vector<vector<int> > > image)
{
   double dx = x2-x1;
   double dy = y2-y1;
   if(dx==0&&dy==0)
      return;
   if(abs(dx)>=abs(dy))
   {
      double m = 1.0*dy/dx;
      double incr = abs(dx)/dx;
      for(double x = x1; incr*x < incr*x2; x+=incr)
      {
         setPixel(x, (int)(1.0*(x-x1)*m+y1), CMAX, 0, 0, image);
      }
   }
   else
   {
      double m = 1.0*dx/dy;
      double incr = abs(dy)/dy;
      for(double y = y1; incr*y < incr*y2; y+=incr)
      {
         setPixel((int)(1.0*(y-y1)*m+x1), y, CMAX, 0, 0, image);
      }
   }
}

bool equals(pair<double, double> p1, pair<double, double> p2)
{
   return p1.first==p2.first&&p1.second==p2.second;
}

vector<vector<vector<int> > > initImage(string file_name)
{
   vector<vector<vector<int> > > image;
   freopen(file_name.c_str(), "r", stdin);
   string unusable;
   cin > > unusable;
   cin > > XMAX;
   cin > > YMAX;
   cin > > CMAX;
   int a;
   for(int y = 0; y < YMAX; y++)
   {
      vector<vector<int> > v1;
      for(int x = 0; x < XMAX; x++)
      {
         vector<int> v2;
         for(int z = 0; z < 3; z++)
         {
            cin > > a;
            v2.push_back(a);
         }
         v1.push_back(v2);
      }
      image.push_back(v1);
   }
   return image;
}

vector<vector<double> > gray_scale(vector<vector<vector<int> > > image)
{
   vector<vector<double> > new_image;
   for(int y = 0; y < YMAX; y++)
   {
      vector<double> v1;
      for(int x = 0; x < XMAX; x++)
      {
         double avg = (0.0+image[y][x][0]+image[y][x][1]+image[y][x][2])/3;
         v1.push_back(avg);
      }
      new_image.push_back(v1);
   }
   return new_image;
}


vector<vector<double> > apply_filter(vector<vector<double> > image, vector<vector<int> > mask)
{
   vector<vector<double> > new_image;
   for(int y = 0; y < YMAX; y++)
   {
      vector<double> v1;
      for(int x = 0; x < XMAX; x++)
      {
         double val = 0.0;
         if(y==0||y==YMAX-1||x==0||x==XMAX-1)
         {}
         else 
         {
            for(int a = 0; a < mask.size(); a++)
               for(int b = 0; b < mask[a].size(); b++)
                  val += mask[a][b]*image[y+b-1][x+a-1];
         }
         v1.push_back(abs(val));
      }
      new_image.push_back(v1);
   }
   return new_image;
}

vector<vector<double> > calculate_magnitudes(vector<vector<double> > dx, vector<vector<double> > dy)
{
   vector<vector<double> > new_magnitudes;
   for(int y = 0; y < YMAX; y++)
   {
      vector<double> v1;
      for(int x = 0; x < XMAX; x++)
      {
         double val = sqrt(dx[y][x]*dx[y][x]+dy[y][x]*dy[y][x]);
         v1.push_back(val);
      }
      new_magnitudes.push_back(v1);
   }
   return new_magnitudes;
}

vector<vector<double> > calculate_angles(vector<vector<double> > dx, vector<vector<double> > dy)
{
   vector<vector<double> > new_angles;
   for(int y = 0; y < YMAX; y++)
   {
      vector<double> v1;
      for(int x = 0; x < XMAX; x++)
      {
         double val;
         if(dx[y][x]==0)
            val = 255;
         else
            val = atan(1.0*dy[y][x]/dx[y][x])*255/1.57079633;
         v1.push_back(val);
      }
      new_angles.push_back(v1);
   }
   return new_angles;
}
void generate_masks()
{
   vector<int> x1{-1, 0, 1};
   vector<int> x2{-2, 0, 2};
   vector<int> x3{-1, 0, 1};
   xm.push_back(x1);
   xm.push_back(x2);
   xm.push_back(x3);
   vector<int> y1{-1, -2, -1};
   vector<int> y2{0, 0, 0};
   vector<int> y3{1, 2, 1};
   ym.push_back(y1);
   ym.push_back(y2);
   ym.push_back(y3);
}
vector<vector<double> > limit_image(vector<vector<double> > image, double cutoff)
{
   vector<vector<double> > new_image;
   for(int y = 0; y < YMAX; y++)
   {
      vector<double> v1;
      for(int x = 0; x < XMAX; x++)
      {
         double val;
         if(image[y][x]<=cutoff)
            val = 0;
         else
            val = CMAX;
         v1.push_back(val);
      }
      new_image.push_back(v1);
   }
   return new_image;
}
void makeImage(vector<vector<double> > image, string file_name)
{
   ofstream out;
   out.open(file_name);
   out << "P3 " << XMAX << " " << YMAX << " " << CMAX << "\n";
   for(int y = 0; y < YMAX; y++)
   {
      for(int x = 0; x < XMAX; x++)
         for(int z = 0; z < 3; z++)
            out << (int) abs(image[y][x]) << " ";
      out << "\n";
   }
   out.close();
}

int main(void) 
{
   long long seed = time(0);
   srand(seed);
   
   cout << "Input file name: ";
   string file_name;
   cin > > file_name;
   if(file_name.find(".ppm")==string::npos)
      file_name = file_name+"/"+file_name+".ppm";
   
   auto start = chrono::high_resolution_clock::now();
   
   orig_image = initImage(file_name);

   
   gray_image = gray_scale(orig_image);
   makeImage(gray_image, file_name.substr(0, file_name.find_last_of(".")) +"_gray"+file_name.substr(file_name.find_last_of(".")));

   
   generate_masks();

   
   dx = apply_filter(gray_image, xm);
   makeImage(dx, file_name.substr(0, file_name.find_last_of(".")) +"_dx"+file_name.substr(file_name.find_last_of(".")));

   
   dy = apply_filter(gray_image, ym);
   makeImage(dy, file_name.substr(0, file_name.find_last_of(".")) +"_dy"+file_name.substr(file_name.find_last_of(".")));

   
   magnitudes = calculate_magnitudes(dx, dy);
   makeImage(magnitudes, file_name.substr(0, file_name.find_last_of(".")) +"_magnitudes"+file_name.substr(file_name.find_last_of(".")));

   
   angles = calculate_angles(dx, dy);
   makeImage(angles, file_name.substr(0, file_name.find_last_of(".")) +"_angles"+file_name.substr(file_name.find_last_of(".")));

   
   for(int x = 0; x <= CMAX; x += 10)
   {
      vector<vector<double> > limited_image = limit_image(magnitudes, x);
      makeImage(limited_image, file_name.substr(0, file_name.find_last_of(".")) +"_limit_"+to_string(x)+file_name.substr(file_name.find_last_of(".")));
   }
   
   auto end = chrono::high_resolution_clock::now();
   auto duration = end-start;
   cout << "Execution Time (seconds): " << duration.count()/pow(10, 9) << endl;
   
   return 0;
}
