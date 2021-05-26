#include <vector>
#include "CA.h"
#include "CB.h"
#include "CSerializer.h"
#include "CStoreFileStrategy.h"
using namespace std;
int main(){
    vector<pCS> VecSrc;
    vector<pCS> Vecdst;

    CA a;
    CB b;

    pCS pa1 = new CA(5,1.1,1.2,1,2);
    pCS pb1 = new CB(6,7,3.14);
    pCS pa2 = new CA(5,1.1,1.2,1,2);
    pCS pb2 = new CB(6,7,3.14);


    VecSrc.push_back(pa1);
    VecSrc.push_back(pb1);
    VecSrc.push_back(pa2);
    VecSrc.push_back(pb2);

    CSerializer Ser(new CStoreFileStrategy("a.txt"));

    Ser.Register(&a);
    Ser.Register(&b);

    Ser.Serialize(VecSrc);
    Ser.DeSerialize(Vecdst);

    for(int i = 0; i < Vecdst.size() ; i++){
            CA* pa = dynamic_cast<CA*>(Vecdst[i]);
            if(pa != NULL)
                    pa->DispData();
            CB* pb = dynamic_cast<CB*>(Vecdst[i]);
            if(pb != NULL)
                    pb->DispData();
     }

    return 0;
}
