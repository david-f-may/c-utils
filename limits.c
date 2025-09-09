/* Source File: limits.c */

#include <stdio.h>
#include <limits.h>

int main ()
{
    printf ("Data type\t\t\t\tBytes\t\t\t  Min Value\t\t\t  Max Value\n");
    printf ("short int\t\t\t\t  %ld\t\t%20d\t\t%20u\n",sizeof(short), SHRT_MIN, SHRT_MAX);
    printf ("unsigned short int\t\t\t  %ld\t\t%20d\t\t%20u\n",sizeof(unsigned short), 0, USHRT_MAX);
    printf ("int\t\t\t\t\t  %ld\t\t%20d\t\t%20u\n",sizeof(int), INT_MIN, INT_MAX);
    printf ("unsigned int\t\t\t\t  %ld\t\t%20u\t\t%20u\n",sizeof(unsigned int), 0, UINT_MAX);
    printf ("long int\t\t\t\t  %ld\t\t%20ld\t\t%20ld\n",sizeof(long int), LONG_MIN, LONG_MAX);
    printf ("unsigned long int\t\t\t  %lu\t\t%20d\t\t%20lu\n",sizeof(unsigned long int), 0, ULONG_MAX);
    printf ("long long int\t\t\t\t  %ld\t\t%20lld\t\t%20lld\n",sizeof(long long int), LLONG_MIN, LLONG_MAX);
    printf ("unsigned long long int\t\t\t  %ld\t\t%20llu\t\t%20llu\n",sizeof(unsigned long long int), 0, ULLONG_MAX);
    return 0;
}
