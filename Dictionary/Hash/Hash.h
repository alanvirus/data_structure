//散列表hashtable：以向量为基础，每个单元成为bucket，存放entry或者entry的引用
//容量R，[0,R)为合法秩区间，叫address space
//hash():key->hash(key)确定了一组词条在address space中的分布          如果key分散有必要hash，即便key分布紧密也可以定义类似于hash(key)=key-20110000这种hash

//perfect hashing: hashO(1)且为双射(单射就行)，此时三种dict基本操作都是O(1)的 bitmap

//对于实际问题，key分散，直接让hashtable规模等于key可能取值范围空间利用率太低
//load factor:hashtable中非空桶数与桶单元总数比值
//不妨假设key为[0,R)内的整数，hash:[0,R)->[0,M)R>>M(散列表长度)>>N(实际词条数)
//降低发生collisison的概率，最好是关键码映射到各桶的概率接近1/M,这样避免：1.大部分key集中分布加剧散列冲突2.某一区间key很少导致空间利用率低下 （clustering：词条集中于散列表少数桶中）
//总之，随机性越强，规律性越弱的hash越好

//division method
//hash(key)=key mod M
//M应为素数，否则hash均匀度将大幅降低，发生冲突概率随着M所含素因子增多迅速加大；
//因为实际应用中词条访问具有周期性，若周期与M有公共素因子，那么词条集中于素因子倍数位置，冲突概率剧增
//散列表长M，将间隔为T的任意M个元素用除余法插入，构成M/g个同余类，在M中相距g的M/g个位置上有g个元素冲突，可见M与潜在的T最大公约数越小越好
//使用素数g必为1，将间隔为T的任意M个元素用除余法插入，一个冲突都没有
//e.g.{1000,1015,...,1135}若M=20，gcd(M,T)=5会clustering,改为M=19,冲突没了，空间利用率也高了；取M等于11，没冲突，空间利用率超高

//除余法仍然保有key的一些连续性，相邻key的散列表地址相邻，极小key都在散列表起始区段，0是个不动点，MAD拥有更强的随机性
//multiply-add-divide method MAD法
//hash(key)=(a*key+b)mod M,M为素数，a>0,b>0,a mod M!=0