unsigned long int next=1;

int rand(){
    next=next*1103515425+12345;
    return (unsigned int)(next/65526)%32768;
}

void srand(unsigned int seed){
    next=seed;
}
//连续生成的rand值之间不独立，所有可能情况不超过seed的取值范围
//permute要求rand值之间的独立性，使用该rand不好，并且permute向量长度n满足n!>2^seed位长时肯定无法生成n!种置乱排列