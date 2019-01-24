#pragma GCC optimize("O3")
#pragma GCC target("sse4")

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <stdlib.h>
#include <stack>
#include <ctime>

#define pb push_back
#define mp make_pair

using namespace std;

typedef long long ll;

class Point {
	public:
		int x, y;

		bool operator < (Point b) {
			if(y != b.y) {
				return y < b.y;
			}
			return x < b.x;
		}
};

int size = 800;
int N;

Point pivot;

//-1 if counterclockwise, 1 if clockwise, 0 if collinear
//cross product between vector ab and vector ac
int ccw(Point a, Point b, Point c) {
    int area = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
    if (area > 0)
        return -1;
    else if (area < 0)
        return 1;
    return 0;
}

//distance squared
int sqdist(Point a, Point b) {
	return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

//to sort points in polar order wrt pivot
//if it's counterclockwise, that means that polar angle between pivot and A is less than the one between pivot and B
bool polar(Point a, Point b)  {
    int order = ccw(pivot, a, b);
    if (order == 0)
        return sqdist(pivot, a) < sqdist(pivot, b);
    return (order == -1);
}

int main(int argc, char **argv) {
	int image[size][size];
	N = stoi(argv[1]);

	//black field
	for(int i = 0; i < size; i++) {
		for(int j = 0; j < size; j++) {
			image[i][j] = 0;
		}
	}

	//random seeding
	srand(time(NULL));

	Point points[N];
	//put in points into array
	for(int i = 0; i < N; i++) {
		int x = rand() % size;
		int y = rand() % size;

		image[x][y] = 1;

		Point p;
		p.x = x;
		p.y = y;

		points[i] = p;
	}

	//GRAHAM SCAN
	stack<Point> hull;

	//find pivot point based on lowest y coordinate. if multiple, lowest x coordinate
	int minY = 0;
	for(int i = 1; i < N; i++) {
		if(points[i] < points[minY]) {
			minY = i;
		}
	}

	//make first element into the pivot
	Point temp = points[0];
	points[0] = points[minY];
	points[minY] = temp;

	pivot = points[0];

	cout << "Pivot (x,y): " << pivot.x << " " << pivot.y << endl;

	//sort remaining points in polar order
	sort(points + 1, points + N, polar);

	hull.push(points[0]);
    hull.push(points[1]);
    hull.push(points[2]);

    for (int i = 3; i < N; i++) {
        Point top = hull.top();
        hull.pop();
        while (ccw(hull.top(), top, points[i]) != -1)   {
            top = hull.top();
            hull.pop();
        }
        hull.push(top);
        hull.push(points[i]);
    }

    cout << "Hull Size: " << hull.size() << endl;

    //get all points along convex hull and add to vector in pair<int, int> form
    vector< pair<int, int> > convex;
    while (!hull.empty()) {
        Point p = hull.top();
        hull.pop();

        convex.pb(mp(p.x, p.y));
    }

    //reverse the order because the elements were popped off from stack
    reverse(convex.begin(), convex.end());
    //add first element again because convex hull has to be enclosed
    convex.pb(convex[0]);

    cout << "Convex Hull Coordinates: " << endl;
    for(int i = 0; i < convex.size(); i++) {
    	cout << convex[i].first << " " << convex[i].second << endl;
    }
    

    //Bresenham's line-drawing algorithm
    for(int i = 0; i < convex.size() - 1; i++) {
		int xstart = convex[i].first;
		int ystart = convex[i].second;

		int xend = convex[i + 1].first;
		int yend = convex[i + 1].second;

		int x1;
		int y1;
		int x2;
		int y2;

		int delX = abs(xend - xstart);
		int delY = abs(yend - ystart);

		double m = (double)delY/delX;

		//driving axis
		int ind;
		int multipler;
		if(delY > delX) {
			if(ystart > yend) {
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
			if(x1 > x2) {
				multipler = -1;
			}
			else {
				multipler = 1;
			}
			ind = x1;
			m = (double)delX/delY;
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
			ind = y1;
		}
		double epsilon = m - 1.0;

		if(ind == x1) {
			//cout << "Driving Axis is Y " << ind << endl;
			for(int k = y1; k < y2 - 1; k++) {
				image[ind][k] = 2;
				if(epsilon >= 0) {
					ind+= multipler;
					epsilon--;
				}
				epsilon += m;
			}
		}
		else {
			//cout << "Driving Axis is X " << ind << endl;
			for(int k = x1; k < x2 - 1; k++) {
				image[k][ind] = 2;
				if(epsilon >= 0) {
					ind+= multipler;
					epsilon--;
				}
				epsilon += m;
			}
		}
	}

    
	//output to ppm file
	string header = "P3 800 800 1";
	ofstream file("convexhull.ppm");
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
