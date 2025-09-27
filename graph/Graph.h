#include"../Vector/Vector.h"
#include"../List/List.h"
typedef enum {UNDISCOVERED,DISCOVERED,VISITED} VStatus;
typedef enum {UNDETERMINED,TREE,CROSS,FORWARD,BACKWARD}EType;

template<typename Tv,typename Te>
class Graph{
    private:
        void reset(){
            for(int i=0;i<n;i++){
                status(i)=UNDISCOVERED;
                dTime(i)=-1;
                fTime(i)=-1;
                parent(i)=-1;
                priority(i)=INT_MAX;
                for(int j=0;j<n,j++){
                    if(exists(i,j))type(i,j)=UNDETERMINED;
                }
            }
        }
        void BFS(int v,int& clock){
            Queue<int> Q;
            status(v)=DISCOVERED;
            Q.enqueue(v);
            while(!Q.empty()){
                v=Q.dequeue();
                dTime ( v ) = ++clock;
                for ( int u = firstNbr ( v ); -1 < u; u = nextNbr ( v, u ) )
                    if ( UNDISCOVERED == status ( u ) ) {
                        status ( u ) = DISCOVERED; Q.enqueue ( u ); 
                        type ( v, u ) = TREE; parent ( u ) = v;
                    } else { 
                        type ( v, u ) = CROSS;
                    } 
                status ( v ) = VISITED;
            }
        }
        void DFS(int v,int& clock){
            dTime(v)=++clock; status(v)=DISCOVERED;
            for(int u=firstNbr(v);u>-1;u=nextNbr(v,u)){ 
                switch(status(u)){
                    case UNDISCOVERED:
                        type(v,u)=TREE; parent(u)=v; DFS(u,clock);break;
                    case DISCOVERED:
                        type(v,u)=BACKWARD; break;
                    default:
                        type(v,u)=(dTime(v)<dTime(u))?FORWARD:CROSS;break;
                }
            }
            status(v)=VISITED; fTime(v)=++clock;
            // Stack<int> s;基于栈的迭代版
            // Stack<int> s_next;
            // s.push(v); 
            // s_next.push(n);
            // while(!s.empty()){
            //     v=s.top();
            //     if(status(v)==UNDISCOVERED){
            //         dTime(v)=++clock; status(v)=DISCOVERED;
            //     }
            //     int u=s_next.top();
            //     if(u=nextNbr(v,u)==-1){
            //         status(v)=VISITED; fTime(v)=++clock;
            //         s.pop();s_next.pop();
            //     }else{
            //         switch(status(u)){
            //             case UNDISCOVERED:
            //                 type(v,u)=TREE; parent(u)=v; s_next.pop(); s_next.push(u); s.push(u); s_next.push(n);
            //             case DISCOVERED:
            //                 type(v,u)=BACKWARD; s_next.pop(); s_next.push(u); 
            //             default:
            //                 type(v,u)=(dTime(v)<dTime(u))?FORWARD:CROSS;s_next.pop(); s_next.push(u); 
            //         }
            //     }
            // }
        }
        void BCC(int,int&,Stack<int>&);
        bool TSort(int v,int& clock,Stack<Tv>* S){
            dTime(v)=++clock; status(v)=DISCOVERED;
            for(int u=firstNbr(v);u>-1;u=nextNbr(v,u)){ 
                switch(status(u)){
                    case UNDISCOVERED:
                        type(v,u)=TREE; parent(u)=v; if(!TSort(u,clock,S)){return false;}break;
                    case DISCOVERED:
                        type(v,u)=BACKWARD; return false;
                    default:
                        type(v,u)=(dTime(v)<dTime(u))?FORWARD:CROSS;break;
                }
            }
            status(v)=VISITED; fTime(v)=++clock;
            S->push(vertex(v));
            return true;
        }
        template<typename PU>
        void PFS(int s,PU pu){
            priority(s)=0; status(s)=VISITED; parent(s)=-1;//设置工作顶点
            while(1){
                for(int u=firstNbr(s);u>-1;u=nextNbr(s,u)){ //设置工作顶点邻居的优先级和父顶点
                    pu(this,s,u);
                }
                for(int shortest=INT_MAX,w=0;w<n;w++){//从未被发现顶点中寻找优先级最小(不为INT_MAX)作为工作顶点，并连边
                    if(status(w)==UNDISCOVERED){
                        if(priority(w)<shortest){
                            shortest=priority(w);
                            s=w;
                        }
                    }
                }
                if(status(s)==VISITED)break;
                status(s)=VISITED;
                type(parent(s),s)=TREE;
            }
        }
    public:
        int n;
        int e;
        virtual int insert(Tv const&v)=0;//插入点，返回编号
        virtual Tv remove(int i)=0;//删除点与其关联边，返回该点
        virtual Tv& vertex(int i)=0;//引用点
        virtual int indegree(int i)=0;//入度
        virtual int outdegree(int i)=0;
        virtual int firstNbr(int i)=0;
        virtual int nextNbr(int i ,int j)=0; //i的邻居中和u有关的？
        virtual VStatus& status(int i)=0;
        virtual int& dTime(int i)=0;
        virtual int& fTime(int i)=0;
        virtual int& parent(int i)=0;
        virtual int& priority(int i)=0;

        virtual bool exists(int i,int j)=0;
        virtual void insert(Te const& edge,int i,int j,int w)=0;
        virtual Te remove(int i,int j)=0;
        virtual EType& type(int i,int j)=0;
        virtual Te& edge(int i,int j)=0;
        virtual int& weight(int i,int j)=0;

        void bfs(int s){//套一层，避免图不连通
            reset();
            int v=s;
            int clock=0;
            do{
                if(status(v)==UNDISCOVERED){
                    BFS(v,clock);
                }
            }while(s!=(v=((++v)%n)));
        }
        void dfs(int s){
            reset();
            int v=s;
            int clock=0;
            do{
                if(status(v)==UNDISCOVERED){
                    DFS(v,clock);
                }
            }while(s!=(v=((++v)%n)));
        }
        void bcc(int i);
        Stack<Tv>* tSort(int s){
            Stack<Tv>* S=new Stack<Tv>;
            reset();
            int v=0;
            int clock=0;
            do{
                if(status(v)==UNDISCOVERED){
                    if(!TSort(v,clock,S)){
                        while(!S.empty()){
                            S.pop();
                        }
                        break;
                    }
                }
            }while(s!=(v=((++v)%n)));
            return S;
        }
        void prim(int i);
        void dijkstra(int i);
        template <typename PU> 
        void pfs ( int s, PU pu){
            reset();
            int v=s;
            do{
                if(status(v)==UNDISCOVERED){
                    PFS(v,pu);
                }
            }while(s!=(v=((++v)%n)));
        }
};
template <typename Tv, typename Te> struct DfsPU {
    virtual void operator() ( Graph<Tv, Te>* g, int uk, int v ) {
        if ( g->status ( v ) == UNDISCOVERED )
            if ( g->priority ( v ) > g->priority ( uk ) - 1 ) {
                g->priority ( v ) = g->priority ( uk ) - 1; 
                g->parent ( v ) = uk;
            }
    }
};
template <typename Tv, typename Te> struct BfsPU {
    virtual void operator() ( Graph<Tv, Te>* g, int uk, int v ) {
        if ( g->status ( v ) == UNDISCOVERED )
            if ( g->priority ( v ) > g->priority ( uk ) + 1 ) {
                g->priority ( v ) = g->priority ( uk ) + 1; 
                g->parent ( v ) = uk;
            }
    }
};
template <typename Tv, typename Te> struct PrimPU {
    virtual void operator() ( Graph<Tv, Te>* g, int uk, int v ) {
        if ( g->status ( v ) == UNDISCOVERED )
            if ( g->priority ( v ) > g->weight(uk,g) ) {
                g->priority ( v ) = g->weight(uk,g); 
                g->parent ( v ) = uk;
            }
    }
};
template <typename Tv, typename Te> struct DijkstraPU {
    virtual void operator() ( Graph<Tv, Te>* g, int uk, int v ) {
        if ( g->status ( v ) == UNDISCOVERED )
            if ( g->priority ( v ) > g->weight(uk,g)+g->priority(uk) ) {
                g->priority ( v ) = g->weight(uk,g)+g->priority(uk); 
                g->parent ( v ) = uk;
            }
    }
};
template<typename Tv>
struct Vertex{
    Tv data;
    int inDegree;
    int outDegree;
    Vstatus status;
    int dTime;
    int fTime;
    int parent;
    int priority;
    Vertex(Tv const& d =(Tv)0){ //构造
        data=d;
        inDegree=0;
        outDegree=0;
        status=UNDISCOVERED;
        dTime=-1;
        fTime=-1;
        parent=-1;
        priority=INT_MAX;
    }
};
template<typename Te>
struct Edge{
    Te data;
    int weight;
    EType type;
    int v;//头顶点
    Edge(Te const& d,int w){
        data=d;
        weight=w;
        type=UNDETERMINED;
    }
};
//基于邻接矩阵
//静态操作O(1)（找邻居操作未必）
//边的动态操作O(1)
//点的动态操作均摊O(n)
//空间O(n^2),考虑到Vector装填因子不小于一个定值（书上说是0.5）
//对于无向图，邻接矩阵为对称阵，可压缩为一个向量，但是此时顶点动态操作O(n^2)，不可取

//基于邻接表
//静态点操作O(1)
//静态边操作O(d)O(n)
//边动态操作O(d)O(n)
//点动态操作O(n+e)
//空间O(n+e)
template<typename Tv,typename Te>
class GraphMatrix: public Graph<Tv,Te>{
    public:
        Vector<Vertex<Tv>> V;//顶点集
        Vector<Vector<Edge<Te>*>>E;//邻接矩阵
        
        GraphMatrix(){n=e=0;}//构造
        ~GraphMatrix(){//析构
            for(int j=0;j<n;j++)
                for(int k=0;k<n;k++)
                    delete E[j][k];
        }
        //点动态增删 
        virtual int insert(Tv const&v){//插入顶点,均摊O(n),最坏O(n^2),或许可以通过设置不同vector的不同扩容策略来避免极端情况
            for(int i=0;i<n;i++){
                E[i].insert(nullptr);
            }
            n++;
            E.insert(Vector<Edge<Te>*>(n,n,nullptr));
            return V.insert(Vertex<Tv>(v));
        }
        virtual Tv remove(int i){//删除点与其关联边，返回该点,均摊O(n),最坏O(n^2)
            for(int j=0;j<n;j++){
                if(exists(i,j)){
                    delete E[i][j];
                    V[j].inDegree--;
                }
            }
            for(int j=0;j<n;j++){
                if(exists(j,i)){
                    delete E[j][i];
                    V[j].outDegree--;
                    E[j].remove(i);
                }
            }
            E.remove(i);
            n--;
            return V.remove(i).data;
        }
        //点静态搜索（call-by-rank） O(1)
        virtual Tv& vertex(int i){return V[i].data;}//引用点
        virtual int indegree(int i){return V[i].inDegree;}//入度
        virtual int outdegree(int i){return V[i].outDegree;}
        virtual VStatus& status(int i){return V[i].status;}
        virtual int& dTime(int i){return V[i].dTime;}
        virtual int& fTime(int i){return V[i].fTime;}
        virtual int& parent(int i){return V[i].parent;}
        virtual int& priority(int i){return V[i].priority;}

        //查找邻居节点
        virtual int firstNbr(int i){//O(n)
            return nextNbr(i,n);
        }
        virtual int nextNbr(int i ,int j){//O(j)
            do{
                j--;
            }
            while((-1<j)&&(!exists(i,j)));
            return j;
        } 

        //边静态搜索 call-by-rank O(1)
        virtual bool exists(int i,int j){
            return (0<=i)&&(i<n)&&(0<=j)&&(j<n)&&E[i][j]!=nullptr;
        }
        virtual EType& type(int i,int j){
            return E[i][j]->type;
        }
        virtual Te& edge(int i,int j){
            return E[i][j]->data;
        }
        virtual int& weight(int i,int j){
            return E[i][j]->weight;
        }

        //边动态增删 O(1)
        virtual void insert(Te const& edge,int i,int j,int w){//显然基于有向图
            if(exists(i,j))return;
            E[i][j]=new Edge<Te>(edge,w);
            e++;
            V[i].outDegree++;
            V[j].inDegree++;
        }
        virtual Te remove(int i,int j){
            if(!exists(i,j))return;
            Te eBak=edge(i,j);
            delete E[i][j];
            E[i][j]=nullptr;
            e--;
            V[i].outDegree--;
            V[j].inDegree--;
            return eBak;
        }
};
template<typename Tv,typename Te>
class GraphList: public Graph<Tv,Te>{
    public:
        Vector<Vertex<Tv>> V;//顶点集
        Vector<List<Edge<Te>*>>E_tail;//邻接表（尾节点列表）
        Vector<List<Edge<Te>*>>E_head;//邻接表（头节点列表）
        GraphMatrix(){n=e=0;}//构造
        ~GraphMatrix(){//析构
            for(int j=0;j<n;j++){
                while(!E_tail[j].empty()){
                    delete E_tail[j].remove(E_tail[j].first());
                }
                while(!E_head[j].empty()){
                    delete E_head[j].remove(E_head[j].first());
                }
            }
        }
        //点动态增删 
        virtual int insert(Tv const&v){//O(1)
            n++;
            E.insert(List<Edge<Te>*>());
            return V.insert(Vertex<Tv>(v));
        }
        virtual Tv remove(int i){//删除点与其关联边,O(n+e)
            for(int j=0;j<n;j++){
                if(exists(i,j)){
                    delete E[i][j];
                    V[j].inDegree--;
                }
            }
            for(int j=0;j<n;j++){
                if(exists(j,i)){
                    delete E[j][i];
                    V[j].outDegree--;
                    E[j].remove(i);
                }
            }
            E.remove(i);
            n--;
            return V.remove(i).data;
        }
        //点静态搜索（call-by-rank） O(1)
        virtual Tv& vertex(int i){return V[i].data;}//引用点
        virtual int indegree(int i){return V[i].inDegree;}//入度
        virtual int outdegree(int i){return V[i].outDegree;}
        virtual VStatus& status(int i){return V[i].status;}
        virtual int& dTime(int i){return V[i].dTime;}
        virtual int& fTime(int i){return V[i].fTime;}
        virtual int& parent(int i){return V[i].parent;}
        virtual int& priority(int i){return V[i].priority;}

        //查找邻居节点
        virtual int firstNbr(int i){//O(n)
            return nextNbr(i,n);
        }
        virtual int nextNbr(int i ,int j){//O(jd)->O(j)
            do{
                j--;
            }
            while((-1<j)&&(!exists(i,j)));
            return j;
        } 

        //边静态搜索 call-by-rank O(d)
        virtual bool exists(int i,int j){
            if((0<=i)&&(i<n)&&(0<=j)&&(j<n)){
                for(int k=0;k<E[i].size();k++){
                    if(E[i][k]->data->v==j){
                        return true;
                    }
                }
            }
            return false;
        }
        virtual EType& type(int i,int j){
            for(int k=0;k<E[i].size();k++){
                if(E[i][k]->data->v==j){
                    return E[i][k]->data->type;
                }
            }
            //可能要返回个默认值才不报错
        }
        virtual Te& edge(int i,int j){
            for(int k=0;k<E[i].size();k++){
                if(E[i][k]->data->v==j){
                    return E[i][k]->data->data;
                }
            }
            //可能要返回个默认值才不报错
        }
        virtual int& weight(int i,int j){
            for(int k=0;k<E[i].size();k++){
                if(E[i][k]->data->v==j){
                    return E[i][k]->data->weight;
                }
            }
            //可能要返回个默认值才不报错
        }

        //边动态增删 用existsO(d),insert没用，remove用 
        virtual void insert(Te const& edge,int i,int j,int w){//显然基于有向图
            if(exists(i,j))return;
            Edge<Te>* Edge=new Edge<Te>(edge,w);
            Edge->v=j;
            E[i].insertasLast(Edge);//应该根据j大小插入
            e++;
            V[i].outDegree++;
            V[j].inDegree++;
        }
        virtual Te remove(int i,int j){
            if(!exists(i,j))return;
            Te eBak=edge(i,j);
            for(int k=0;k<E[i].size();k++){
                if(E[i][k]->data->v==j){
                    delete E[i][k]->data;
                    E[i][k]->data=nullptr;
                }
            }
            e--;
            V[i].outDegree--;
            V[j].inDegree--;
            return eBak;
        }
};
