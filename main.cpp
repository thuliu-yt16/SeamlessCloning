#include "api.h"
int main(){
    string path = "../";

    Image src(path + "source.jpg");
    Image tgt(path + "target.jpg");
    vector<Point_2> v;
    ReadVertex(path + "vertex.txt", v);
    //PlainCopy(src,tgt,430,330,v);
    MVCCloneWithMesh(src,tgt,430,330,v);
    tgt.show("finish");
	waitKey(0);
    tgt.save("result__.jpg");
    return 0;
}
