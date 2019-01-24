#pragma GCC optimize("O3")
#pragma GCC target("sse4")

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <stdlib.h>
#include <stack>

#define pb push_back
#define mp make_pair

using namespace std;

typedef long long ll;
int size = 800;
int N = 100;

class Point {
	public:
		int x, y;
};

vector<Point> points;

double area(Point p1, Point p2, Point p3) {
	return abs(((p1.x * p2.y + p2.x * p3.y + p3.x * p1.y) - (p1.y * p2.x + p2.y * p3.x + p3.y * p1.x)) / 2.0);
}

bool removeable(Point p) {
	return p.x == -1 && p.y == -1;
}

int main() {
	int image[size][size];

	//black image field
	for(int i = 0; i < size; i++) {
		for(int j = 0; j < size; j++) {
			image[i][j] = 0;
		}
	}

	srand(time(NULL));

	//make random points and put it into vector
	for(int i = 0; i < N; i++) {
		Point p;
		p.x = rand() % size;
		p.y = rand() % size;

		image[p.x][p.y] = 1;

		points.pb(p);
	}

	while(1) {
		//pick 3 random points
		int ind1 = rand() % points.size();
		int ind2 = rand() % points.size();
		int ind3 = rand() % points.size();

		bool flag = false;
		for(int i = 0; i < points.size(); i++) {
			if(i != ind1 && i != ind2 && i != ind3) {
				double a1 = area(points[i], points[ind1], points[ind2]);
				double a2 = area(points[i], points[ind2], points[ind3]);
				double a3 = area(points[i], points[ind3], points[ind1]);
				double realArea = area(points[ind1], points[ind2], points[ind3]);

				if(a1 + a2 + a3 <= realArea) {
					Point temp;
					temp.x = -1;
					temp.y = -1;
					points[i] = temp;

					flag = true;
				}
			}
		}

		remove_if(points.begin(), points.end(), removeable);

		if(!flag) {
			break;
		}
	}

	for(int i = 0; i < points.size(); i++) {
		image[points[i].x][points[i].y] = 3;
	}
   //Bresenham's line drawing algorithm
	for(int i = 0; i < points.size(); i++) {
		for(int j = i + 1; j < points.size(); j++) {
			int x1 = points[i].x;
			int y1 = points[i].y;

			int x2 = points[j].x;
			int y2 = points[j].y;

			int delX = x2 - x1;
			int delY = y2 - y1;

			double m = (double)delY/delX;

			//driving axis
			int ind;
			if(delY > delX) {
				ind = x1;
				m = (double)delX/delY;
			}
			else 
				ind = y1;
			double epsilon = m - 1.0;

			if(ind == x1) {
				cout << "Driving Axis is Y " << ind << endl;
				for(int k = y1; k < y2 - 1; k++) {
					image[ind][k] = 0;
					if(epsilon >= 0) {
						ind++;
						epsilon--;
					}
					epsilon += m;
				}
			}
			else {
				cout << "Driving Axis is X " << ind << endl;
				for(int k = x1; k < x2 - 1; k++) {
					image[k][ind] = 0;
					if(epsilon >= 0) {
						ind++;
						epsilon--;
					}
					epsilon += m;
				}
			}
		}
	}


	//output
	string header = "P3 800 800 1";
	ofstream file("convexhullTriangle.ppm");
	file << header << endl;
	//0 = black, 1 = white, 2 = yellow, 3 = red
	for(int i = 0; i < size; i++) {
		string out = "";
		for(int j = 0; j < size; j++) {
			if(image[i][j] == 0) {
				out += "0 0 0 ";
			}
			else if(image[i][j] == 1) {
				out += "1 1 1 ";
			}
			else if(image[i][j] == 2) {
				out += "1 1 0 ";
			}
			else {
				out += "1 0 0 ";
			}
		}
		file << out << endl;
	}
	cout << "DONE" << endl;
	file.close();
	return 0;
   
}

