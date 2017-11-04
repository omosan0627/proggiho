#!/usr/bin/ruby

class Stacperf
	MEMSIZE=[16, 32, 64, 128, 256, 512, 1024,
		2048, 4096, 8192, 16384, 32768, 65536,
		131072, 262144, 524288, 1048576,
		2097152, 4194304, 8388608, 10000000 ]
	def run
		a=Array.new(MEMSIZE[-1],0.0)

		print "N,\tseconds\t,Mop/s,\tchecksum\n"
		MEMSIZE.each { |n|
			print n.to_s() + ",\t"
			checksum=0.0
			nloops=(MEMSIZE[-1]/n).to_i()

			start_time=Time.now.to_f()
			
			nloops.times {
				(0..n-1).each { |i|
					a[i]=i.to_f()
				}
				(0..n-1).reverse_each { |i|
					checksum += a[i]
				}
			}

			end_time=Time.now.to_f()
			elapsed_time=end_time-start_time
			print elapsed_time.to_s()+",\t"
			
			mops=0.000001*2.0*n*nloops/elapsed_time
			print mops.to_s()+",\t"+checksum.to_s()+"\n"
		}
	end
end

sp=Stacperf.new()
sp.run()
