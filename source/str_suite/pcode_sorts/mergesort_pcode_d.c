/* Copyright (c) 2012 the authors listed at the following URL, and/or
the authors of referenced articles or incorporated external code:
http://en.literateprograms.org/Merge_sort_(C)?action=history&offset=20110810210421

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation codes (the
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

Retrieved from: http://en.literateprograms.org/Merge_sort_(C)?oldid=17235
*/

// modified to accommodate pcode_d

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../types.c"

#define MIN_MERGESORT_LIST_SIZE    4


int sort(int codes, struct pcode_d* pc, int start, int end);
void mergesort(struct pcode_d a[], int size, struct pcode_d temp[]);


int sort(int codes, struct pcode_d* pc, int start, int end) {
    struct pcode_d *pc2=pc;
	struct pcode_d *temp=(struct pcode_d*)calloc(end-start, sizeof(struct pcode_d));
		
		if(codes>0 && !pc2)
		{
			fprintf(stderr, "\n*** Issue allocating memory! ***\n");
			free(temp);
			return FAILED1;
		}
		pc2=pc2+start;
		mergesort(pc2, end-start, temp);
	
	free(temp);
	return SUCCESS1;
}


void mergesort(struct pcode_d a[], int size, struct pcode_d temp[]) {
    int i, i1, i2, j, tempi;
	struct pcode_d v;
	
	
	if (size < MIN_MERGESORT_LIST_SIZE) 
	{
		for (i=0; i < size; i++) {
			v = a[i];
			for (j = i - 1; j >= 0; j--) {
			   if (a[j].di <= v.di) break;
				a[j + 1] = a[j];
			}
			a[j + 1] = v;
		}
	}else {
		
		mergesort(a, size/2, temp);
		mergesort(a + size/2, size - size/2, temp);
		i1 = 0;
		i2 = size/2;
		tempi = 0;
		while (i1 < size/2 && i2 < size) {
			if (a[i1].di <= a[i2].di) {
				temp[tempi] = a[i1];
				i1++;
			} else {
				temp[tempi] = a[i2];
				i2++;
			}
			tempi++;
		}

		while (i1 < size/2) {
			temp[tempi] = a[i1];
			i1++;
			tempi++;
		}
		while (i2 < size) {
			temp[tempi] = a[i2];
			i2++;
			tempi++;
		}

		memcpy(a, temp, size*sizeof(struct pcode_d));
	}
}
