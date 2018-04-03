//Алгоритм поиска ближайшей пары точек из заданного множества. 
//Метод разделяй-и-властвуй, O(n*log n)
//Использую глобальные переменные для массива точек, буфера, и ответа
#include <iostream>
#include <algorithm>
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
};

ostream& operator<<(ostream& os, const Point& ob) 
{
		os<<'('<<ob.x<<','<<ob.y<<") ";
		return os;
}

inline bool cmp_x (const Point &A, const Point &B)
{
	return A.x < B.x || A.x == B.x && A.y < B.y;
}

inline bool cmp_y (const Point &A, const Point &B)
{
	return A.y < B.y;
}

long double mindistance;
Point *Array, *Buf, ansP, ansQ;

inline void check(const Point &A, const Point &B)
{
	double distance = sqrt((A.x - B.x)*(A.x - B.x) + (A.y - B.y)*(A.y - B.y));
	if (distance < mindistance)
		mindistance = distance, ansP = A, ansQ = B;
}

void point_pair_mindistance(int l, int r)
{
	if (r - l <= 3){
		for (int i = l; i <=r; ++i)
			for (int j = i + 1; j <= r; ++j)
				check(Array[i], Array[j]);
		sort(Array + l, Array + r + 1, &cmp_y);
		return;
	}
	int m = l + r >> 1;
	long double medx = Array[m].x;
	point_pair_mindistance(l, m);
	point_pair_mindistance(m + 1, r);
	merge(Array+l, Array+m+1, Array+m+1, Array+r+1, Buf, &cmp_y);
	copy(Buf, Buf+r-l+1, Array+l);
	
	int bufsize = 0;
	for (int i = l; i <= r; ++i)
		if (abs (Array[i].x - medx) < mindistance){
			for (int j = bufsize - 1; j >= 0 && Array[i].y - Buf[j].y < mindistance; --j)
				check(Array[i], Buf[j]);
			Buf[bufsize++] = Array[i];	
		}
	
}


int main(int argc, char** argv)
{	
	int i, n;
	long double x, y;
	ifstream file;
	ofstream fileres;
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
	file >> n;
	Array  = new Point[n];
	Buf = new Point[n];
	for (i = 0; i < n; ++i){	
		file >> x >> y;
		Array[i].x = x;
		Array[i].y = y;
	}
	for (i = 0; i < n; ++i){	
		cout << Array[i] << endl;
	}
	cout << "--------"<< endl;
	sort(Array, Array + n, &cmp_x);
	mindistance = 1E20;
	point_pair_mindistance(0, n-1);
	cout << "Points " << ansP << ansQ << endl;
	cout << "Distance " << mindistance << endl;
	delete[] Array;
	delete[] Buf;
	file.close();
	fileres.close();
	return 0;
}
