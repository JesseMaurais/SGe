#ifndef __Vector__
#define __Vector__

struct Vector
{
	union
	{
	 double v[3];
	
	 struct { double x,y,z; };
	 struct { double i,j,k; };
	 struct { double s,t,r; };
	 struct { double R,G,B; };
	};

	Vector(void);
	Vector(double *p);
	Vector(double a);
	Vector(double a, double b);
	Vector(double a, double b, double c);

	double Dot(Vector V);
	void Cross(Vector U, Vector V);
	double Square(void);
	double Magnitude(void);
	double Normalize(void);
	void Right(Vector A, Vector B, Vector C);
	Vector Project(Vector V);
	double Quadratic(double t);
	double Linear(double t);

	Vector operator - (void);
	Vector operator + (Vector V);
	Vector operator - (Vector V);
	Vector operator * (Vector V);
	Vector operator / (Vector V);
	Vector operator + (double t);
	Vector operator - (double t);
	Vector operator * (double s);
	Vector operator / (double s);
	bool operator == (Vector V);
};

#endif // file

