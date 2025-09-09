#define HASH(x)  (((x)<<15)+((x)<<10)+((x)<<5)+(x))
#define HASH_INC ((HASH('z')) - (HASH('A')) - (HASH('0')))
#define HASH_NUM (('z') - ('A'))

int main (void)
{
    int hash_inc = HASH_INC;
    printf ("HASH('z') == %d\n", HASH('z'));
    printf ("HASH('A') == %d\n", HASH('A'));
    printf ("HASH_INC = %d\n", hash_inc);
    printf ("HASH_num = %d\n", HASH_NUM);
}