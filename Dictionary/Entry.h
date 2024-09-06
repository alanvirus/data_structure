template<typename K,typename V> 
struct Entry{
        K key;
        V value;
        Entry(){}
        Entry(const K& k,const V& v):key(k),value(v){}
};