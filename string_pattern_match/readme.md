在n个空位中填入K个1的所有可能 -> comb

字符串模式匹配算法：
1.暴力匹配 -> match_brute_force
2.karp-rabin -> match_with_rolling_hash_v1  (如果哈希到整型，是不怕溢出的，因为输入和哈希函数相同，输出一定相同) 有在rolling过程中不断取模的，防止溢出后哈希的非预期分布
                 match_with_rolling_hash_v2  修改rolling公式除法变乘法，可能降低O(n)常系数
                 滚动哈希需要模运算防止溢出，且模运算为mod；v1中rolling有除法与mod不兼容
3.bm算法 -> bm_match
            bm_match_normal一个半规范版本
            bm_match_normal_v2规范版本
    利用好后缀与坏后缀加速匹配
4.kmp算法 ->match_kmp
            match_kmp_normal规范版本
    利用好前缀加速匹配
    
测两种计算滚动哈希方法速度 -> test_rolling_hash_speed
    同一个rolling两种实现，均不考虑溢出，计算结果不同
    考虑溢出后也不同，不管了
    方法v2更快

测各种匹配算法速度 -> test_pattern_match_speed
    被测试session不要有cout，时间不准
    在一般情况下bruteforce比roling-hash快，但是一些cornercase下bf很慢
    boyal-moore比另外两个快
    
最长公共子串：

最长回文子串：