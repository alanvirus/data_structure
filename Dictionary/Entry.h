#define EntryPosition(K,V) Entry<K,V>*
template<typename K,typename V> 
struct Entry{
        K key;
        V value;
        Entry(){}
        Entry(const K& k,const V& v):key(k),value(v){}
        Entry(Entry<K,V> const&e):key(e.key),value(e.value){}
        bool operator<(Entry<K,V> const&e){return key<e.key;}
        bool operator>(Entry<K,V> const&e){return key>e.key;}
        bool operator==(Entry<K,V> const&e){return key==e.key;}
        bool operator!=(Entry<K,V> const&e){return key!=e.key;}
};