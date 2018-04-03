//Алгоритм определения взаимного расположения точки и отрезка

#include <iostream>
using namespace std;

struct Point{
	double x, y;
	Point(double a = 0.0, double b = 0.0)
	{
		x = a;
		y = b;
	}
	Point operator+(const Point& Ob) const
	{
		return Point(x + Ob.x, y + Ob.y);
	}
	Point operator-(const Point& Ob) const
	{
		return Point(x - Ob.x, y - Ob.y);
	}
	Point operator*(double n)
	{
		return Point(x*n, y*n);
	}
};

ostream& operator<<(ostream& os, const Point& ob) 
{
		os<<'('<<ob.x<<','<<ob.y<<")\n";
		return os;
}

enum turn_t {Left = 1, Right = -1, Collinear = 0};	


double cross(const Point& A, const Point& B)
{
	//returns cross-product
	return A.x*B.y - B.x * A.y;
}

turn_t turn(const Point& A, const Point& B, const Point& C)
{
	double det = cross(B - A, C - A);
	if (det > 0)
		return Left;
	if (det < 0)
		return Right;
	
	return Collinear;
}

int main()
{
	double a, b;
	//cout<<"Enter points A, B and M; format: x y\n";
	//cout<<"A: ";
	//cin>>a>>b;
	Point A(0.0, 0.0);
	//cout<<"B: ";
	//cin>>a>>b;
	Point B(0.0, 1.0);
	//cout<<"M: ";
	//cin>>a>>b;
	a = (1.0/(1LL << 52));
	b = 1 * (1LL << 52);
	Point M(a, b);
	cout << M;
	//cout<<A<<B<<M;
	if (turn(A, B, M) == Left)
		cout<<"Left\n";
	else if (turn(A, B, M) == Right)
		cout<<"Right\n";
	else 
		cout<<"Collinear\n";
	return 0;
}
