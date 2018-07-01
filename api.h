#ifndef API_H
#define API_H
#include "all.h"
#include "image.h"
CGAL::Bounded_side check(const Point_2& p, const vector<Point_2>& v);

double dis(const Point_2& v1, const Point_2& v2);

double tan_2(double a,double b, double c);

void PlainCopy(const Image& src, Image& tgt, int offsetx, int offsety, const vector<Point_2>& v);

void ReadVertex(const string& filename, vector<Point_2>& v);

void CalculateMeanValueCoordinate(const Point_2& p, double* lambda, const vector<Point_2>& v);

void MVCClone(const Image& src, Image& tgt, int offsetx, int offsety,const vector<Point_2>& v);

void CreateMesh(CDT& cdt);

Face_handle LocationQuery(const Point_2& p, CDT& cdt);

void GetVertices(CDT& cdt, vector<Point_2>& vs);

void MVCCloneWithMesh(const Image& src, Image& tgt, int offsetx, int offsety, const vector<Point_2>& v);

Vector3 GetWeight(const Point_2& p, const vector<Point_2>& vertex);
#endif
