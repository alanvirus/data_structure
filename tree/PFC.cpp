#include"BinTree.h"
typedef BinTree<char> PFCTree;

#include"../Vector/Vector.h"
typedef Vector<PFCTree*> PFCForest;

#include "../Dictionary/Bitmap/Bitmap.h"
#include "../Dictionary/SkipList/SkipList.h"
typedef SkipList<char,char*> PFCTable;

#define  N_CHAR  (0x80 - 0x20)

PFCForest* initForest(){
    PFCForest* forest=new PFCForest;
    for(int i=0;i<N_CHAR;i++){
        forest->insert(i,new PFCTree());
        (*forest)[i]->insertAsRoot(0x20+i);
    }
    return forest;
}

PFCTree* generateTree(PFCForest* forest){
    while(1<forest->size()){
        PFCTree*s=new PFCTree;
        s->insertAsRoot('^');
        Rank r1=rand()%forest->size();
        s->attachAsLC(s->root(),(*forest)[r1]);
        forest->remove(r1);
        Rank r2=rand()%forest->size();
        s->attachAsRC(s->root(),(*forest)[r2]);
        forest->remove(r2);
        forest->insert(forest->size(),s);
    }
    return (*forest)[0];
}

void generateCT(Bitmap* code,int length,PFCTable* table, BinNode<char>* v){
    if(IsLeaf(*v)){
        table->put(v->data,code->bits2string(length));
        return;
    }
    if(HasLChild(*v)){
        code->clear(length);
        generateCT(code,length+1,table,v->lc);
    }
    if(HasRChild(*v)){
        code->set(length);
        generateCT(code,length+1,table,v->rc);
    }
}
PFCTable* generateTable(PFCTree* tree){
    PFCTable* table=new PFCTable;
    Bitmap* code=new Bitmap;
    generateCT(code,0,table,tree->root());
    delete code;
    return table;
}
int encode(PFCTable*table, Bitmap&codeString, char*s){
    int m=strlen(s);
    int x=0;
    for(int i=0;i<m;i++){
        char** pCharCode=table->get(s[i]);
        if(!pCharCode)pCharCode=table->get(' ');
        int n=strlen(*pCharCode);
        for(int j=0;j<n;j++){
            (*pCharCode)[j]=='1'?codeString.set(x):codeString.clear(x);
            x++;
        }
    }
    return x;
}
void decode(PFCTree* tree,Bitmap&code,int n){
    BinNode<char>*x=tree->root();
    int i=0;
    while(i<n){
        if(IsLeaf(*x)){
            cout<<x->data;
            x=tree->root();
        }
        x=code.test(i)?x->rc:x->lc;
        i++;
    }
}
int main(){
    PFCForest* forest=initForest();
    PFCTree* tree=generateTree(forest);
    delete forest;
    PFCTable* table=generateTable(tree);
}