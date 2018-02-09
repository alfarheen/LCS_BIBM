/* Copyright (c) 2012 the authors listed at the following URL, and/or
the authors of referenced articles or incorporated external code:
http://en.literateprograms.org/Radix_sort_(C)?action=history&offset=20060515025941

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Retrieved from: http://en.literateprograms.org/Radix_sort_(C)?oldid=4760
*/

//
// Added capability to sort int
//

#ifndef RADIX_SORT_LIB
#define RADIX_SORT_LIB

#include<stdlib.h>
#include<stdio.h>
#include<assert.h>
#include<limits.h>
#include<strings.h>
#include<string.h>

void radix_sort_uint(int *a, size_t size, int bits)
{
	int mask;
	int rshift=0u;
	int *p, *b, *b_orig;
	int i;
	int key;
	int cntsize;
	int *cntarray;
	b=b_orig=(int *)malloc(size*sizeof(int));
	cntsize=1u<<bits;
	cntarray=(int *)calloc(cntsize, sizeof(int));

	assert(cntarray);
	assert(b_orig);

	for(mask=~(INT_MAX<<bits); mask; mask<<=bits, rshift+=bits) {
		for(p=a; p<a+size; ++p) {
			key=(*p & mask)>>rshift;
			++cntarray[key];
		}

		for(i=1; i<cntsize; ++i) cntarray[i]+=cntarray[i-1];

		for(p=a+size-1; p>=a; --p) {
			key=(*p & mask)>>rshift;
			b[cntarray[key]-1]=*p;
			--cntarray[key];
		}

		p=b; b=a; a=p;

		bzero(cntarray, cntsize * sizeof(int));
	}

	if(a==b_orig) memcpy(b, a, size*sizeof(int));

	free(b_orig);
	free(cntarray);
}

/*
int main()
{
	int i;
	int a[]={
		123,INT_MAX-2,654,INT_MAX,654,2123,543,131,653,123,
		533,INT_MAX-4,532,213,INT_MAX-3,824,1124,42,134,411,
		491,341,1234,527,388,245,1992,654,243,987};

	printf("Before radix sort:\n");
	for(i=0; i<sizeof a/sizeof(int); ++i) 
		printf(" %u", a[i]);
	putchar('\n');

	radix_sort_uint(a, sizeof a/sizeof(int), 4);

	printf("After radix sort:\n");
	for(i=0; i<sizeof a/sizeof(int); ++i) 
		printf(" %u", a[i]);
	putchar('\n');

	return 0;
}
*/

#endif

