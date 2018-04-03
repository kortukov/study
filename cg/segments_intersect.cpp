//Алгоритм поиска точки пересечения двух отрезков.

#include <iostream>
#include <fstream>
#include <float.h>
#include <cmath>
#define determinant(a, b, c, d) (a*d - b*c)
using namespace std;

const long double EPS = LDBL_EPSILON ;

struct Point{
	long double x, y;
	Point(long double a = 0.0, long double b = 0.0)
	{
		x = a;
		y = b;
	}
	bool operator< (const Point& M) const
	{
		return x < M.x - EPS || abs(x - M.x) < EPS && y < M.y - EPS;
	}
	bool operator == (const Point& M) const
	{
		return abs(x - M.x) < EPS && abs(y - M.y) < EPS;
	}
	
};

ostream& operator<<(ostream& os, const Point& ob) 
{
		os<<'('<<ob.x<<','<<ob.y<<") ";
		return os;
}

struct Line{
	long double a, b, c;
	Line(){}
	Line(Point M, Point N)
	{
		a = M.y - N.y;
		b = N.x - M.x;
		c = -a * M.x - b * M.y;
		norm();
	}
	void norm()
	{
		long double n = sqrt(a*a + b*b);
		if (abs(n) > EPS){
			a /= n; b /= n; c /= n;
		}	
	}
	long double dist(Point M) 
	{
		return a*M.x + b*M.y + c;
	}
};

inline bool between(long double l, long double r, long double x)
{
	return min(l,r) <= x + EPS && x <= max(l,r) + EPS;
}

inline bool intersect_one(long double a, long double b, long double c, long double d)
{
	if (a > b) swap (a, b);
	if (c > d) swap (c, d);
	return max(a,c) <= min(b, d) + EPS;
}

bool intersect(Point A, Point B, Point C, Point D, Point &left, Point& right)
{
	//proection
	if(!intersect_one(A.x, B.x, C.x, D.x) || !intersect_one(A.y, B.y, C.y, D.y)){
		cout << "bounding box false \n";
		return false;
	}
	if (A == C || A == D){ left = right = A; return true; }
	if (B == C || B == D){ left = right = B; return true; }
	Line M(A, B);
	Line N(C, D);
	long double det = determinant(M.a, M.b, N.a, N.b);
	if (abs(det) < EPS) { //if collinear
		if (abs(M.dist(C)) > EPS || abs(N.dist(A)) > EPS){
			cout << "Collinear not on same line \n";
			return false;
		}
		if (B < A) swap (A, B);
		if (D < C) swap (C, D);
		left = max(A, C);
		right = min(B, D);
		cout << "On same line\n";
		return true;		
	}
	else {//Kramer rule
		cout << "Kramer rule\n";
		left.x = right.x = - determinant(M.c, M.b, N.c, N.b) / det;
		left.y = right.y = - determinant(M.a, M.c, N.a, N.c) / det;
		cout << left.x << ", " << left.y << endl;
		cout << between(A.x, B.x, left.x)	<< between(A.y, B.y, left.y) << between(C.x, D.x, left.x) << between(C.y, D.y, left.y) << endl;
		return between(A.x, B.x, left.x)
		&& between(A.y, B.y, left.y)
		&& between(C.x, D.x, left.x)
		&& between(C.y, D.y, left.y);
	}
}

int main(int argc, char** argv)
{	
	ifstream file;
	ofstream fileres;
	long double x1, y1, x2, y2, x3, y3, x4, y4;
	Point A, B, C, D;
	char* name = new char[100];
	if (argc < 2){
		cout << "Too few arguments\n";
		return 1;
	}
	try{
		file.open(argv[1], ios::in);
		if(! file.is_open()) throw ("Can't open source file\n");
		fileres.open("result.txt", ios::out);
		if(! fileres.is_open()) throw ("Can't open result file\n");
	}
	catch(const char* exception){
		cout << exception;
		return -1;
	}
	
	while(1){
		file >> name >> x1 >> y1 >> x2 >>y2 >> x3 >> y3 >> x4 >> y4;
		if (file.eof()) break;
		Point left, right;
		A = Point(x1, y1);
		B = Point(x2, y2);
		C = Point(x3, y3);
		D = Point(x4, y4);
		if (intersect(A, B, C, D, left, right)){
			if (left == right)
				fileres << name << ' ' << A << B << C << D << "Intersection  "<< left << right << endl;	
			else
				fileres << name << ' ' << A << B << C << D << "Intersection by segment:  "<< left << right << endl;
		} 
		else {
			fileres << name << ' ' << A << B << C << D << "No intersection" << endl;
			
		}
		
	}
	file.close();
	fileres.close();
	delete name;
	return 0;
}




