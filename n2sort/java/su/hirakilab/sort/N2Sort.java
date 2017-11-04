package su.hirakilab.sort;

import java.security.SecureRandom;

public class N2Sort {
	private final java.util.Random r=new SecureRandom();
	private volatile long start_time, end_time;
	private long[] a;

	public N2Sort(int max_n) {
		a=new long[max_n];
	}

	private void bubble_sort(int n) {
		for(int i=0; i<n-1; i++) {
			long x,y,z=a[0];
			for(int j=0; j<n-1-i; j++) {
				x=z;
				y=a[j+1];
				boolean c=x>y;
				a[j]=c?y:x;
				z=c?x:y;
			}
			a[n-1-i]=z;
		}
	}

	private boolean verify(int n) {
		for(int i=0; i<n-1; i++) {
			//System.out.println(Long.toHexString(a[i]));
			if(a[i]>a[i+1]) {
				return false;
			}
		}
		return true;
	}

	public double measure(int n) {
		for(int i=0; i<a.length; i++) {
			a[i]=r.nextLong();
		}
		
		start_time=System.nanoTime();
		
		bubble_sort(n);
		
		end_time=System.nanoTime();
		long elapsed_time_ns=end_time-start_time;
		double elapsed_time_s=((double)elapsed_time_ns)/1.E9;

		return verify(n)?elapsed_time_s:Double.NaN;
	}
	
	public static void main(String[] args) {
		final int[] n={
			500,2000,4000,6000,8000,10000
		};
		N2Sort sort=new N2Sort(n[n.length-1]);
		for(int x:n) {
			System.out.print(Integer.toString(x).concat(","));

			for(long i=0; i<4; i++) {
				double t=sort.measure(x);
				System.out.print("\t".concat(Double.toString(t)).concat(","));
			}

			System.out.print('\n');
		}
	}
}
