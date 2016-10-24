/*
 * Copyright 1993-2010 NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property and 
 * proprietary rights in and to this software and related documentation. 
 * Any use, reproduction, disclosure, or distribution of this software 
 * and related documentation without an express license agreement from
 * NVIDIA Corporation is strictly prohibited.
 *
 * Please refer to the applicable NVIDIA end user license agreement (EULA) 
 * associated with this source code for terms and conditions that govern 
 * your use of this NVIDIA software.
 * 
 */


#include <cstdio>
#include <iostream>
#include "timer.h"
using namespace std;

#define N   32*1024*1024

void add( int *a, int *b, int *c ) {
    int tid = 0;    // this is CPU zero, so we start at zero
    while (tid < N) {
        c[tid] = a[tid] + b[tid];
        tid += 1;   // we have one CPU, so we increment by one
    }
}

int main( void ) {
    int* a, *b, *c;

		a= new int[N];
		b= new int[N];
		c= new int[N];

		Timer watch;

    // fill the arrays 'a' and 'b' on the CPU
    for (int i=0; i<N; i++) {
        a[i] = -i;
        b[i] = i * i;
    }

		watch.start();
    add( a, b, c );
		float time = watch.elapsed();
		watch.stop();
		cout << "kernel CPU took "<< time*1000 << " ms " << endl;

    // display the results
		if(N < 100){
    for (int i=0; i<N; i++) {
        printf( "%d + %d = %d\n", a[i], b[i], c[i] );
    }
		}

		delete [] a; a=NULL;
		delete [] b; b=NULL;
		delete [] c; c=NULL;
    return 0;
}
