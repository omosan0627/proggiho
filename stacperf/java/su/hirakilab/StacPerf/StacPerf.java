package su.hirakilab.StacPerf;

public class StacPerf {
	private volatile long start_time, end_time;
	private static long memsize[] = {
		16l, 32l, 64l, 128l, 256l, 512l, 1024l,
		2048l, 4096l, 8192l, 16384l, 32768l, 65536l,
		131072l, 262144l, 524288l, 1048576l,
		2097152l, 4194304l, 8388608l, 10000000l	};
	private void run() {
		long max_n=memsize[memsize.length-1];
		long a[]=new long[(int)max_n];
		
		System.out.println("N,\tseconds,\tMop/s,\tchecksum");
		for(long n : memsize) {
			start_time=System.nanoTime();
			long nloops=10l*max_n/n;
			long i=nloops;
			long checksum=0l;
			
			do {
				for(long j=0; j<n; j++) {
					a[(int)j]=j;
				}
				for(long j=n-1; j>=0; j--) {
					checksum += a[(int)j];
				}
			} while(0<--i);
			end_time=System.nanoTime();

			double elapsed_time=1.E-9*(double)(end_time-start_time);
			double mops=1.E-6*2.*n*nloops/elapsed_time;
			System.out.println(Long.toString(n).concat(",\t").
				concat(Double.toString(elapsed_time)).
				concat(",\t").
				concat(Double.toString(mops)).
				concat(",\t").
				concat(Long.toString(checksum)));
		}
		
	}
	public static void main(String[] args) {
		StacPerf sp=new StacPerf();
		sp.run();
	}
}
