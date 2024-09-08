列表实现 -> List.h/ListNode.h
    自适应列表

qs：什么叫算法稳定，为什么selectMax后者优先可以保证selectionSort稳定？
当有多个value相同时，通过后者优先可以保证排序后这些value的相对顺序不变，称为算法稳定 
mergeSort和insertionSort(search从后面开找)都是稳定的
radixSort过程中使用的排序子程序必须是稳定排序，并且radixSort是稳定的

k为桶数量
          时间复杂度          空间复杂度        适用场景
桶排序     O(n+nlogn-nlogk+K)   O(n+k)           元素均匀分布
计数排序   O(n+maxV-minV)     O(n+maxV-minV)   maxV-minV很小，元素分布密集
基数排序   O(dn)              O(n+k)           1.非负整数或可以作为非负整数比较 2.maxV-minV很小（让d小一点）
（对于基数排序，使用二进制，d最大，k=2最小，此时O(nd)很可能还不如O(nlogn);使用maxV做基数，k=maxV最大，d=1最小，此时退化为计数排序）

使用Bitmap方法实现无序向量或无序列表去重是O（N），要求元素可取集合大小有限，如果是实数则不行