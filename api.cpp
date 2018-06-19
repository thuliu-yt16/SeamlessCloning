#include "api.h"
CGAL::Bounded_side check(const Point_2& p, const vector<Point_2>& v){
    return CGAL::bounded_side_2(v.begin(), v.end(),p, K());
}
double dis(const Point_2& v1, const Point_2& v2){
    return sqrt(CGAL::squared_distance(v1, v2));
}
double tan_2(double a,double b, double c){
    double s = (a + b + c) * 0.5;
    return sqrt((s - b) * (s - c) / s / (s - a));
}

void PlainCopy(const Image& src, Image& tgt, int offsetx, int offsety, const vector<Point_2>& v){
    for(int i = 0; i < src.w; i ++){
        for(int j = 0; j < src.h; j ++){
            Point_2 p(i, j);
            if(check(p, v) == CGAL::ON_BOUNDED_SIDE)
                tgt.drawPixel(offsetx + i, offsety + j, src.getPixel(i,j));
        }
    }
}

void ReadVertex(const string& filename, vector<Point_2>& v){
    ifstream in;
    in.open(filename, ios::in);
    int cnt;
    in >> cnt;
    int x,y;
    for(int i = 0; i < cnt; i ++){
        in >> x >> y;
        v.push_back(Point_2(x,y));
    }
    in.close();
}

void CalculateMeanValueCoordinate(const Point_2& p, double* lambda, const vector<Point_2>& v){
    int vsize = v.size();
    double* l = new double[vsize]; // length of pi - x;
    double* t = new double[vsize]; // tan(1/2 * L pi x pi+1)
    double* w = new double[vsize]; // wi = tan(1/2 * ai-1) + tan( 1/2 * ai) / pi - x
    double totw = 0.0;
    for(int i = 0; i < vsize; i ++){
        l[i] = dis(p,v[i]);
    }
    for(int i = 0; i < vsize - 1; i ++){
        t[i] = tan_2(dis(v[i + 1], v[i]), l[i + 1], l[i]);
    }
    t[vsize - 1] = tan_2(dis(v[0],v[vsize - 1]), l[0], l[vsize - 1]);
    for(int i = 1; i < vsize; i ++){
        totw += w[i] = (t[i - 1] + t[i]) / l[i];
    }
    totw += w[0] = (t[vsize - 1] + t[0]) / l[0];
    totw = 1.0 / totw;
    for(int i = 0; i < vsize; i ++){
        lambda[i] = w[i] * totw;
    }
    delete[] l;
    delete[] t;
    delete[] w;
}
void MVCClone(const Image& src, Image& tgt, int offsetx, int offsety,const vector<Point_2>& v){
    int vsize = v.size();
    int sw = src.w;
    int sh = src.h;

    Color* diff = new Color[vsize];
    for(int i = 0; i < vsize; i ++){
        diff[i] = tgt.getPixel(v[i].x() + offsetx, v[i].y() + offsety) - src.getPixel(v[i].x(),v[i].y());
    }
    double*** lam = new double**[sw];
    for(int i = 0; i < sw; i ++){
        lam[i] = new double* [sh];
        for(int j = 0; j < sh; j ++){
            lam[i][j] = new double [vsize];
            Point_2 p(i,j);
            if(check(p,v) == CGAL::ON_BOUNDED_SIDE){
                CalculateMeanValueCoordinate(p,lam[i][j],v);
                Color r(0.0,0.0,0.0);
                for(int k = 0; k < vsize; k ++){
                    r += lam[i][j][k] * diff[k];
                }
                tgt.drawPixel(i + offsetx, j + offsety, src.getPixel(i,j) + r);
            }
        }
    }
    delete[] diff;
    for(int i = 0; i < sw; i ++){
        for(int j = 0; j < sh; j ++)
            delete[] lam[i][j];
        delete[] lam[i];
    }
    delete[] lam;
}
void Triangulation(const vector<Point_2>& v, CDT& cdt){

    //TODO
}

void CreateMesh(CDT& cdt, const vector<Point_2>& v){
    vector<Vertex_handle> vh;
    int vsize = v.size();
    for(auto p : v){
        vh.push_back(cdt.insert(CDTPoint(p.x(),p.y())));
    }
    for(int i = 0; i < vsize; i ++){
        cdt.insert_constraint(vh[i],vh[(i == vsize - 1) ? 0 : (i + 1)]);
    }
    cout << "Number of vertices : " << cdt.number_of_vertices() << endl;
    cout << "Meshing the triangulation..." << endl;
    CGAL::refine_Delaunay_mesh_2(cdt, Criteria(0.125,16));
    cout << "Number of vertices : " << cdt.number_of_vertices() << endl;
}

Face_handle LocationQuery(const Point_2& p, CDT& cdt){
    return cdt.locate(CDTPoint(p.x(),p.y()));
}
void GetVertices(CDT& cdt, vector<Point_2>& vs){
    for(CDT::Finite_vertices_iterator vit = cdt.finite_vertices_begin(); vit != cdt.finite_vertices_end(); vit ++){
        CDTPoint p = vit -> point();
        vs.push_back(Point_2(p.x(),p.y()));
    }
}
void ConvertToPoint_2(const Face_handle fh, vector<Point_2>& facep){
    facep.clear();
    for(int i = 0; i < 3; i ++){
        CDTPoint p = fh -> vertex(i) -> point();
        facep.push_back(Point_2(p.x(), p.y()));
    }
}
void ShowMesh(Image img, const CDT& cdt){
    for(CDT::Finite_edges_iterator eit = cdt.finite_edges_begin(); eit != cdt.finite_edges_end(); eit ++){
        CDT::Face& f = *(eit -> first);
        int i = eit -> second;
        CDTPoint p1 = f.vertex(CDT::cw(i)) -> point();
        CDTPoint p2 = f.vertex(CDT::ccw(i)) -> point();
        img.drawLine(p1.x(),p1.y(), p2.x(), p2.y(),Color(255,0,0),1);
    }
    img.show("mesh");
    img.save("mesh.jpg");
}

void MVCCloneWithMesh(const Image& src, Image& tgt, int offsetx, int offsety, const vector<Point_2>& v){
    int vsize = v.size();
    int sw = src.w;
    int sh = src.h;
    CDT cdt;
    //Triangulation(v,cdt);
    CreateMesh(cdt, v);

    vector<Point_2> meshv;
    GetVertices(cdt, meshv);

    Color* diff = new Color[vsize];
    for(int i = 0; i < vsize; i ++){
        diff[i] = tgt.getPixel(v[i].x() + offsetx, v[i].y() + offsety) - src.getPixel(v[i].x(),v[i].y());
    }
    double*** lam = new double**[sw];
    Color** r = new Color*[sw];
    for(int i = 0; i < sw; i ++){
        lam[i] = new double* [sh];
        r[i] = new Color[sh];
        for(int j = 0; j < sh; j ++){
            lam[i][j] = new double [vsize];
        }
    }
    for(auto p : meshv){
        int x = p.x();
        int y = p.y();
        CalculateMeanValueCoordinate(p, lam[x][y], v);
        Color c(0.0,0.0,0.0);
        for(int k = 0; k < vsize; k ++){
            c += lam[x][y][k] * diff[k];
        }
        r[x][y] = c;
        tgt.drawPixel(x + offsetx, y + offsety, src.getPixel(x,y) + c);
    }
    for(int i = 0; i < sw; i ++){
        for(int j = 0; j < sh; j ++){
            Point_2 p(i,j);
            if(check(p,v) == CGAL::ON_BOUNDED_SIDE){
                Face_handle fh = LocationQuery(p, cdt);
                vector<Point_2> fp;
                ConvertToPoint_2(fh, fp);
                Color c = 0.33333 * (r[(int)fp[0].x()][(int)fp[0].y()] + r[(int)fp[1].x()][(int)fp[1].y()] + r[(int)fp[2].x()][(int)fp[2].y()]);
                tgt.drawPixel(i + offsetx, j + offsety, src.getPixel(i,j) + c);
            }
        }
    }

    delete[] diff;
    for(int i = 0; i < sw; i ++){
        for(int j = 0; j < sh; j ++)
            delete[] lam[i][j];
        delete[] lam[i];
        delete[] r[i];
    }
    delete[] lam;
    delete[] r;
    ShowMesh(src, cdt);
    waitKey(0);
}
