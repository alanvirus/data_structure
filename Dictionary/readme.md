call-by-rank（logical&physical） call-by-rank (only logical)or position(physical address)
数组：静态查找方便，动态维护困难 | 有序列表：只能顺序查找，增量式动态维护方便
r-value O（1）        O（n）     p/r-value    O（1）/O（n）    O（1）/O(n)     ->这种设计就是为了存value列设计的
r-key-value O（logn） O（n）     p/r-key-valueO（1）/O (n)     O（1）/O(n)     ->但是也可以存（key，value）列

平衡二叉搜索树：查找，插入，删除均在O(logn)内实现

和数组不一样的地方：key也是某种value
和搜索树不一样的地方：搜索树要求key可以相互比较，而词典则只要求key可以相互判等


词典ADT：
put(key,value):插入词条（key，value），成功返回true，失败返回false （已存在是true但不真插入；skiplist直接允许雷同插入；false）
get(key):根据key得到value，没有则返回NULL
remove(key):删除key对应的词条(key,value),成功返回true，失败返回false  （本来不存在是true;skiplist如果不存在则false，存在雷同删除最靠右的）
size() 

实现：
1. 在支持key的比较操作后，任何一种搜索树都可以用来实现词典 （java.util.TreeMap用红黑树实现） 
2. SkipList 增强版列表，同样需要支持key的比较
3. 散列函数+散列表
