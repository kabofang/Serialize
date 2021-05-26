#include <vector>
#include "CC.h"
#include "CD.h"
#include "CStructuredJson.h"
#include "CStoreFileStrategy.h"
using namespace std;
int main(){
    vector<pCS> VecSrc;
    vector<pCS> Vecdst;

    CC c;
    CD d;

    pCS pc1 = new CC(5,1.1,1.2,1,2);
    pCS pd1 = new CD(6,7,3.14);
    pCS pc2 = new CC(5,1.1,1.2,1,2);
    pCS pd2 = new CD(6,7,3.14);


    VecSrc.push_back(pc1);
    VecSrc.push_back(pd1);
    VecSrc.push_back(pc2);
    VecSrc.push_back(pd2);

    CStructuredJson Ser(new CStoreFileStrategy("a.json"));

    Ser.Register(&c);
    Ser.Register(&d);

    Ser.Serialize(VecSrc);
    Ser.DeSerialize(Vecdst);

    for(int i = 0; i < Vecdst.size() ; i++){
            CC* pc = dynamic_cast<CC*>(Vecdst[i]);
            if(pc != NULL)
                    pc->DispData();
            CD* pd = dynamic_cast<CD*>(Vecdst[i]);
            if(pd != NULL)
                    pd->DispData();
    }

    return 0;
}
