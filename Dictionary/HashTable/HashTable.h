// 散列表hashtable：以向量为基础，每个单元成为bucket，存放entry或者entry的引用
// 容量R，[0,R)为合法秩区间，叫address space
// hash():key->hash(key)确定了一组词条在address space中的分布          如果key分散有必要hash，即便key分布紧密也可以定义类似于hash(key)=key-20110000这种hash

// perfect hashing: hashO(1)且为双射(单射就行)，此时三种dict基本操作都是O(1)的 bitmap

// 对于实际问题，key分散，直接让hashtable规模等于key可能取值范围空间利用率太低
// load factor:hashtable中非空桶数与桶单元总数比值
// 不妨假设key为[0,R)内的整数，hash:[0,R)->[0,M)R>>M(散列表长度)>>N(实际词条数)
// 降低发生collisison的概率，最好是关键码映射到各桶的概率接近1/M,这样避免：1.大部分key集中分布加剧散列冲突2.某一区间key很少导致空间利用率低下 （clustering：词条集中于散列表少数桶中）
// 总之，随机性越强，规律性越弱的hash越好

// division method
// hash(key)=key mod M
// M应为素数，否则hash均匀度将大幅降低，发生冲突概率随着M所含素因子增多迅速加大；
// 因为实际应用中词条访问具有周期性，若周期与M有公共素因子，那么词条集中于素因子倍数位置，冲突概率剧增
// 散列表长M，将间隔为T的任意M个元素用除余法插入，构成M/g个同余类，在M中相距g的M/g个位置上有g个元素冲突，可见M与潜在的T最大公约数越小越好
// 使用素数g为1（除非T是M的倍数，那g=M），将间隔为T的任意M个元素用除余法插入，一个冲突都没有
// e.g.{1000,1015,...,1135}若M=20，gcd(M,T)=5会clustering,改为M=19,冲突没了，空间利用率也高了；取M等于11，没冲突，空间利用率超高
// M个桶与M的M个同余类对应，词条被放入M/g个桶，如果不采取冲突解决措施，空间利用率不会超过1/g
// M取1<<k?相当于截取key的低位k个bit作为hash，高位完全没影响，降低了随机性和均匀性

// multiply-add-divide method MAD法
// 除余法仍然保有key的一些连续性，相邻key的散列表地址相邻，极小key都在散列表起始区段，0是个不动点，MAD拥有更强的随机性
// hash(key)=(a*key+b)mod M,M为素数，a>0,b>0,a mod M!=0(在M为素数的情况下等价于a，M互素)
// 保持除余法对于周期性key的良好性质，且克服连续性缺陷，相邻key被分散到不同地址(a决定)，且起点变了(b决定)

// selecting digits数字分析法
// 从key特定进制的展开中取特定的若干位构成一个整型地址(比如10进制展开后的所有奇数位)
// mid-square平方取中法:从key的平方的十进制或二进制展开中取居中的若干位
// folding折叠法:将key的十进制或二进制展开分割成等宽的若干段，相加 e.g.段长取3，hash(123456789)=123+456+789=1368,或者往复折返式相加，hash(123456789)=123+654+789=1566
// 位异或法:将key的二进制展开分割成等宽若干段，异或 hash(411)=hash((110011011)b)=(110)b^(011)b^(011)b=(110)b=6 往复折返式:hash(411)=hash((110011011)b)=(110)b^(110)b^(011)b=(011)b=3
// 一般来说，最后要对M取模，保证结果落在合法散列地址空间(看起来上面的一些函数的结果会随着输入变大而变大)

// 伪随机数法
// 拥有一个伪随机数发生器rand(key)
// hash(key)=rand(key)mod M,hash的目标是随机性，随机数发生器的目标也是，优化目标一致，考虑如何设计一个好的随机数发生器就好了（反之一个随机性强的哈希可以作为随机数发生器）
// 需要注意：不同计算环境提供的rand()不尽相同，故a系统下生成的散列表移植到b系统需谨慎

// 再随机的hash，冲突都会发生，M个桶，放入n个entry，假设hash完全随机，至少发生一次冲突的概率为1-(M!/(M-n)!)/M^n,M为365，n为23发生冲突率就大于0.5了，此时load factor=6.3%
// 发生冲突率不变，M增大，装填因子变小
// 因此要处理冲突
// 1.multiple slots:每个桶存放定长向量组织的多个slot，每个slot存Entry,若每个桶有k个槽，装填因子为N/(kM)降低至原本的1/k,并且k多大很难事先确定，无法确保是否会有桶溢出
// 2.seperate chaining:独立链法,解决空间利用率和溢出问题，但是发生冲突后需要遍历列表导致查找成本增加，建议装填因子上限0.9
// 3.overflow area:公共溢出区法,设置一个公共溢出区，插入时发生冲突了就把entry放到到溢出区，在冲突不频繁时可以用该方法，溢出区本身可以套用任何一种词典实现，该方法可以理解为递归散列表
// 独立链和公共溢出区引入次级结构，并且独立链查找成本可能较高，因为列表物理上不一定关联，规模大了I/O操作很多
// 闭散列策略
// 冲突后在散列表内部寻找另一空bucket存放，每个bucket不再局限于存特定一组entry，而是对所有entry开放，因此该策略又称作open addressing开放地址
// 相比之下，之前的冲突处理策略可称为closed addressing封闭地址或开散列open hashing
// 没有附加空间，一定程度上增加了冲突可能，故需要装填因子需要适当降低，一般取0.5或更低;特别是在使用懒惰标记后，查找成本很高，因此有必要rehash保持低装填因子
// 1.linear probing线性试探法：插入时若hash[key]已被占用，则试探ht[(hash[key]+i)%M],i=1,2,...直到发现空桶则插入，查找同样逐个试探，试探到空桶都没找到则终止（装填因子小于0.5，肯定能终止）
//                                                     从i=0开始被称为probing chain，由于不同链会交错重合，导致同一chain中元素未必key冲突（key冲突一定在同一查找链），平均成功查找长度和失败查找长度需要根据具体情况计算
// 就一个散列表，物理连贯，具有良好的数据局部性，系统缓存可以发挥作用，查找几乎无需I/O操作 (那为什么散列表不直接存entry而是存entry*)
// 综合来看，闭散列策略对I/O负担的降低有效，在M不是很小，load factor不大时可能得冲突增加是小问题，因此实际应用更广泛
// 查找链上某一entry删除会导致途径该桶的查找链无法正常工作，因此需要lazyRemoval懒惰删除告知该位置存放过entry，当然插入时该位置可以被使用，也不必清除lazy标记
// 习题9-16为什么不能轻易优化懒惰标记的空桶
// 线性试探会加剧聚集
// 2.quadratic probing平方试探 (hash(key)+i^i)mod M i=0,1,... 试探位置的间距以线性速度增长 
// 是否会造成数据局部性失效? 设桶大小4B，典型缓存规模1024KB,足以覆盖长度为sqrt(1024/4)=16的查找链，设查找链最大长i，应该有(i-1)^2+1<=1024/4，所以除非链极长，否则还好
// M为素数的前提下，平方试探的查找链只会覆盖特定的一组桶，前(M+1)/2个桶必然互异，故装填因子小于等于50%肯定不会因为无法抵达空桶失败
//                 装填因子超过0.5后，至少(M+1)/2个entry，调整桶位置，插入操作会因无法抵达空桶失败，因为对任意(M+1)/2<=c<M,取d=M-c二者关于M同余，对于更大的c，取d=M-c%M则同余，故一共就(M+1)/2个同余类，会无法抵达空桶
// M为合数，c>=M/2,一定能找到同余的，[0,(M-2)/2]中也可能有同余的，故装填因子小于0.5也可能因为找不到空桶无法终止
// 3.伪随机试探：既然要试探位置随机且均匀分布，可以rand(j)mod M 生成查找链
// 4.double hashing再散列法: [hash(key)+i*hash_2(key)]mod M i=0,1,...
// 5.双向平方试探法: 解决平方试探查找链最多用一半桶的问题，0,1^2,-1^2,2^2,-2^2,... 当M为素数且M=4*k+3时，查找链前M项互异 ，需要基于素数表加条件筛选出4k+3

//关键码->hashCode()->散列码->hash()->桶地址
//为了支持后续不同尺度的散列空间和各种散列函数，散列码取值范围应覆盖系统支持的最大整数范围，并且散列码冲突应尽可能少；hashCode()应与判等器保持一致，这句没懂
//byte,shaort,int,char:可直接强转为32位整数作为散列码
//long long和double超过32位，可以将高32位低32位相加作为散列码，多个整数的集合也可以用，相加后截取低32位
//字符串:直接把字符转整数相加冲突太多，可使用多项式散列码，取a>=2,x=x[0]*a^(n-1)+x[1]+a^(n-2)+...+x[n-1],a的低比特位不得全为0，实验得对英语单词，a=33,37,39,41不错
#include "../Dictionary.h"
#include "../Bitmap/Bitmap.h"
#include "../Entry.h"
static size_t hashCode(char c){
    return (size_t)c;
}
static size_t hashCode(int k){
    return (size_t)k;
}
static size_t hashCode(long long k){
    return (size_t)((k>>32)+(int)k);
}
static size_t hashCode(char s[]){
    int h=0;
    for(size_t n=strlen(s),i=0;i<n;i++){
        h= (h<<5)|(h>>27);//循环左移5位
        h+=(int)s[i];
    }
    return (size_t)h;
}
template <typename K, typename V>
class HashTable : public Dictionary<K, V>
{
private:
    Entry<K, V> **ht;
    int M;
    int N;
    Bitmap *lazyRemoval;
#define lazilyRemoved(x) (lazyRemoval->test(x))
#define markAsRemoved(x) (lazyRemoval->set(x))
protected:
    int probe4Hit(const K &k);  // 遇到懒惰标记的空桶会继续，remove和get使用
    int probe4Free(const K &k); // 遇到空桶就停，put使用
    bool put_when_rehash(Entry<K, V> * to_move);
    void rehash();
    void release(Entry<K, V> *to_del){
        delete to_del;
    }
public:
    HashTable(int c = 5); // 创建一个容量不小于c的散列表
    ~HashTable();
    int size() const
    {
        return N;
    }
    bool put(K k, V v);
    V *get(K k);
    bool remove(K k);
};
template <typename K, typename V>
void HashTable<K, V>::rehash(){
    int oldM=M;
    Entry<K, V> ** old_ht= ht;
    M = primeNLT(2*M, 1048576, "../Bitmap/prime-1048576-bitmap.txt");
    ht = new Entry<K, V> *[M];
    memset(ht, 0, sizeof(Entry<K, V> *) * M);
    delete lazyRemoval;
    lazyRemoval=new Bitmap(M);
    N=0;
    // memcpy_s(ht,M*sizeof(Entry<K, V> *),old_ht,oldM*sizeof(Entry<K, V> *)); 哈希的M都变了，需要重新插入
    for(int i=0;i<oldM;i++){
        if(old_ht[i]){
            put_when_rehash(old_ht[i]);
            //书中使用put且未release，原因是存放词条已转移?
        }
    }
    delete[] old_ht;
}
template <typename K, typename V>
int HashTable<K, V>::probe4Hit(const K &k)
{
    int r = hashCode(k) % M; // 这个%M好像确实在hashCode外更合适,在考虑扩容M会变后更合适了
    while ((ht[r] && (ht[r]->key != k)) || (!ht[r] && lazilyRemoved(r)))
    {
        r = (r + 1) % M;//如果采用平方试探，改为r=(r+2*(i++)+1)%M,i初始为0，实际上不用乘法
    }
    return r;
}
template <typename K, typename V>
int HashTable<K, V>::probe4Free(const K &k)
{
    int r = hashCode(k) % M;
    while (ht[r])
    {
        r = (r + 1) % M;
    }
    return r;
}
template <typename K, typename V>
V *HashTable<K, V>::get(K k)
{
    int r = probe4Hit(k);
    return ht[r] ? &(ht[r]->value) : nullptr;
}
template <typename K, typename V>
bool HashTable<K, V>::remove(K k)
{
    int r = probe4Hit(k);
    if (!ht[r])
    {
        return false;
    }
    release(ht[r]); // 这一步别忘了，这不是智能指针
    ht[r] = nullptr;
    markAsRemoved(r);
    N--; // 这也别忘了
    return true;
}
template <typename K, typename V>
bool HashTable<K, V>::put(K k, V v){ 
    if(ht[probe4Hit(k)])return false; //允许雷同key的话直接删掉该语句，get/remove语义改为与目标词条等key的任一词条
    int r=probe4Free(k);
    hr[r]=new Entry<K,V>(k,v);
    N++;
    if(N*2>M)rehash();
    return true;
}
template <typename K, typename V>
bool HashTable<K, V>::put_when_rehash(Entry<K, V>* to_move){
    int r=probe4Free(to_move->key);
    hr[r]=to_move;
    N++;
    return true;
}
template <typename K, typename V>
HashTable<K, V>::HashTable(int c)
{
    M = primeNLT(c, 1048576, "../Bitmap/prime-1048576-bitmap.txt");
    N = 0;
    ht = new Entry<K, V> *[M];
    memset(ht, 0, sizeof(Entry<K, V> *) * M);//可增加校验环版bitmap，在桶被插入时set对应p，删除时设置为-1-p 每个桶有三种状态，因此两个bit（该bitmap+lazybitmap）可以表示：00初始 10/11非空 01被删除
    lazyRemoval = new Bitmap(M);
}
int primeNLT(int c, int max, char *prime_file)
{ //[c,max)中的最小素数
    Bitmap b(prime_file, max);
    while (c < max)
    {
        if (!b.test(c))
        {
            return c;
        }
        c++;
    }
    // 素数表存小了直接返回c，实用中不能如此处理
    return c;
}
template <typename K, typename V>
HashTable<K, V>::~HashTable()
{
    for (int i = 0; i < M; i++)
    {
        if (ht[i])
            release(ht[i]); // 记得先析构各桶中指针指向的词条列表，c++没有RAII
    }
    delete[] ht;
    delete lazyRemoval;
}
