/* pesudo-stack operation performance */
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <linux/compiler.h>

static double dtime(void);
extern int main(void);

static double dtime()
{
	double q;
	struct timeval tnow;

	gettimeofday(&tnow,NULL);
	q = (double)tnow.tv_sec + (double)tnow.tv_usec * 1.0e-6;

	return q;
}

int main() {
	static long memsize[] = {
		16l, 32l, 64l, 128l, 256l, 512l, 1024l,
		2048l, 4096l, 8192l, 16384l, 32768l, 65536l,
		131072l, 262144l, 524288l, 1048576l,
		2097152l, 4194304l, 8388608l, 10000000l};
	long i;
	long max_n=memsize[sizeof(memsize)/sizeof(memsize[0])-1];
	long *page_a;
	if(!(page_a=(long*)valloc(sizeof(long)*max_n))) {
		puts("memory allocation failed");
		exit(EXIT_FAILURE);
	}

	/* initial initialization array, own address */
	for(i=0; i<max_n; i++) { page_a[i]=i; }
	fputs("array init done\n",stderr);

	puts("N,\tseconds,\tMop/s,\tchecksum");
	for(i=0; i<sizeof(memsize)/sizeof(memsize[0]); i++) {
		long n=memsize[i];
		long nloops=10L*max_n/n;
		double start_time,end_time;
		long chksum=0l;
		double elapsed_time, mops;
		long i;
		printf("%ld,\t",n); fflush(stdout);
		// OSMemoryBarrier();
		start_time=dtime();
		// OSMemoryBarrier();

		i=nloops;
		do {
			long j;
			for(j=0; j<n; j++) { page_a[j]=j; }
			for(j=n-1; j>=0; j--) { chksum += page_a[j]; }
		} while(--i);

		// OSMemoryBarrier();
		end_time=dtime();
		
		elapsed_time=end_time-start_time;
		mops=1.E-6*2.*((double)(n*nloops))/elapsed_time;
		printf("%f,\t%f,\t%016lX\n",elapsed_time,mops,chksum);
	}

	free(page_a);
	exit(EXIT_SUCCESS);
}

