#include "api.h"
int main(){
    string path = "../rsc/";

    Image src(path + "source1.jpg");
    Image tgt(path + "target1.jpg");
    vector<Point_2> v;
    ReadVertex(path + "vertex1.txt", v);
    //PlainCopy(src,tgt,430,330,v);
    //MVCCloneWithMesh(src,tgt,430,330,v);
    MVCCloneWithMesh(src,tgt,20,10,v);
    tgt.show("finish");
	waitKey(0);
    tgt.save("../res/result1.jpg");
    return 0;
}
