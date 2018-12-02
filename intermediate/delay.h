#include<stdio.h>
#include<time.h>
void delay(double );
void delay(double no_of_sec)
{
double ms=1000000*no_of_sec;
clock_t start_time=clock();
while((double)clock()<(double)start_time+ms);
}




