#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <openssl/rand.h>
// #include <libkern/OSAtomic.h>

static double dtime(void);
static void show_array(int64_t *a, long l, long r);
static void bubble_sort(int64_t *a, long n);
static int verify_sort(int64_t *a, long n);
static double measure_merge_sort(int64_t *a, int64_t *b, long n);
static double measure_bubble_sort(int64_t *a, long n);
extern int main(void);

static void pv(long a, long b) {
	printf("(%ld %ld)", a, b);
}

static void sp() {
	printf("\n");
}

static double dtime()
{
	double q;
	struct timeval tnow;

	gettimeofday(&tnow,NULL);
	q = (double)tnow.tv_sec + (double)tnow.tv_usec * 1.0e-6;

	return q;
}

static void show_array(int64_t *a, long l, long r) {
	for(; l < r; l++) {
		printf("%ld%c", a[l], l != r - 1 ? ' ' : '\n');
	}
}

static void bubble_sort(int64_t *a, long n) {
	long i,j;
	for(i=0; i<n; i++) {
		long x,y,z=a[0];
		for(j=0; j<n-1-i; j++) {
			int c;
			x=z;
			y=a[j+1];
			c=(x>y);
			a[j]=c?y:x;
			z=c?x:y;
		}
		a[n-1-i]=z;
	}
}

///////////////////////////////////ここからmerge_sort

static void merge_sort(int64_t *a, int64_t * b, long l, long r) { //a:マージしたい列,bは補助,lとrはmergeする区間。aの[l, r)にある要素をソートする
	if(r - l <= 1) return; 
	long m = (l + r) / 2;
	merge_sort(a, b, l, m); merge_sort(a, b, m, r); //[l, r)を[l, m)[m, r)に分割してそれぞれソート
	// pv(114514, 0); sp();
	// show_array(a, l, r);
	long lv = l, rv = m;
	long at = l;
	//[l, m)と[m, r)をbにマージする
	while(1) {
		if(lv == m && rv == r) break;
		else if(lv == m) {
			b[at++] = a[rv++];
		}
		else if(rv == r) {
			b[at++] = a[lv++];
		}
		else {
			if(a[lv] < a[rv]) b[at++] = a[lv++];
			else b[at++] = a[rv++];
		}
	}
	// pv(114514, 1); sp();
	// show_array(b, l, r);
	for(;l < r;l++) a[l] = b[l];//最後にbをaにコピーする。
}

/////////////////////////////////merge_sort end

static int verify_sort(int64_t *a, long n) {
	long i;
	for(i=0; i<n-1; i++) {
		/* printf("%016llX\n",a[i]); */
		if(a[i]>a[i+1]) {
			return -1;
		}
	}
	return 0;
}

static double measure_merge_sort(int64_t *a, int64_t *b, long n) {
	double start_time,end_time,elapsed_time;
	
	if(RAND_pseudo_bytes((void*)a, sizeof(int64_t)*n)<0) {
		fputs("array initializaton failed\n",stderr);
		return NAN;
	}
	// long i;
	// for(i = 0; i < n; i++) a[i] = n - i;
	// show_array(a, 0, n);
	
	//OSMemoryBarrier();
	start_time=dtime();
	//OSMemoryBarrier();

	//bubble_sort(a,n);
	merge_sort(a, b, 0, n);
	// sp();
	// show_array(a, 0, n);

	//OSMemoryBarrier();
	end_time=dtime();

	elapsed_time=end_time-start_time;
	return !verify_sort(a,n)?elapsed_time:NAN;
}

static double measure_bubble_sort(int64_t *a, long n) {
	double start_time,end_time,elapsed_time;
	
	if(RAND_pseudo_bytes((void*)a, sizeof(int64_t)*n)<0) {
		fputs("array initializaton failed\n",stderr);
		return NAN;
	}
	// long i;
	// for(i = 0; i < n; i++) a[i] = n - i;
	// show_array(a, 0, n);
	
	//OSMemoryBarrier();
	start_time=dtime();
	//OSMemoryBarrier();

	bubble_sort(a,n);
	// merge_sort(a, b, 0, n);
	// sp();
	// show_array(a, 0, n);

	//OSMemoryBarrier();
	end_time=dtime();

	elapsed_time=end_time-start_time;
	return !verify_sort(a,n)?elapsed_time:NAN;
}

extern int main() {
	static const long n[] = {
		500l, 2000l, 4000l, 6000l, 8000l, 10000l, 12000l, 14000l, 16000l, 18000l, 20000l,
		30000l, 40000l, 60000l, 100000l, 160000l
	};
	long i=0;
	int64_t *m1, *m2;
	if(!(m1=valloc(sizeof(int64_t)*n[sizeof(n)/sizeof(n[0])-1]))) {
		fputs("memory allocation failed\n",stderr);
		return EXIT_FAILURE;
	}

	if(!(m2=valloc(sizeof(int64_t)*n[sizeof(n)/sizeof(n[0])-1]))) {
		fputs("memory allocation failed\n",stderr);
		return EXIT_FAILURE;
	}


	for(i=0; i<sizeof(n)/sizeof(n[0]); i++) {
	// for(i=0; i < 1; i++) {
		long j=4;
		printf("%ld,",n[i]); fflush(stdout);
		do {
			double t=measure_bubble_sort(m1,n[i]);
			printf("%f,",t); fflush(stdout);
		} while(--j);
		putchar('\n');
	}

	for(i=0; i<sizeof(n)/sizeof(n[0]); i++) {
	// for(i=0; i < 1; i++) {
		long j=4;
		printf("%ld,",n[i]); fflush(stdout);
		do {
			double t=measure_merge_sort(m1,m2,n[i]);
			printf("%f,",t); fflush(stdout);
		} while(--j);
		putchar('\n');
	}

	free(m1);

	exit(EXIT_SUCCESS);
}
