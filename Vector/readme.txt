sequence: vector(array/linear array) + list
array/vector中的概念：
1.predecessor前驱：任意0<=i<j<n,A[i]为A[j]的前驱，其中A[j-1]为immediate predecessor直接前驱(要求j>=1),任一元素的所有前驱构成prefix前缀
  successor后驱：任意0<=j<i<n,A[i]为A[j]的后驱，其中A[j+1]为immediate successor(要求j<=n-2)，任一元素的所有后驱构成suffix后缀
2.call-by-rank 与call-by position等价： A[i]物理地址为A+i*s
  rank:元素的前驱个数
3.vector相较于array：不要求元素属于同一基本类型，甚至不见得同时具有某一数值属性，不保证能比较大小

4.无序向量：仅支持比对(T重载了==,!=),但未必支持比较的向量
  有序向量:元素支持比较，且所有元素按次序单调分布

5.排序的时间复杂度常系数主要取决于比较次数，也就是所谓的查找长度(search length)

6.排序算法的分类：
内部排序算法：数据规模内存足以容纳
外部排序算法：数据规模大，需要借助外部甚至分布式存储器，在排序计算过程中的任一时刻内存只能容纳其中一小部分数据

离线算法offline algorithm:输入数据以批处理形式整体给出
online algorithm：数据实时生成，算法启动后陆续到达

依赖体系结构：
串行算法
并行算法

是否采用随机策略：
确定式算法
随机式算法

本书介绍的主要是确定式串行脱机的内部排序算法

7.排序算法的稳定性stability:将向量A转为有序向量S后，设A[i]对应于S[k_i]
对任意A[i]==A[j],都有i<j当前仅当k_i<k_j,称为stable algorithm：数据实时生成，算法启动后陆续到达
稳定的排序算法可以实现对多个关键码key按照字典序的排序（先排优先级低的基数排序）