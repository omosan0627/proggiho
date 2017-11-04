#!/usr/bin/ruby
require 'openssl'

class N2Sort
	def initialize(n)
		@a=Array.new(n)
	end
	def fill_random(n)
		(0..n-1).each { |i|
			str_8b=OpenSSL::Random.random_bytes(8)
			x=str_8b.unpack('q')[0]
			# want Fixnum, not BigNum
			@a[i]=x&((2**(8*0.size)-1)>>2)
		}
	end
	def sort(n)
		(0..n-1).each { |i|
			z=@a[0]
			(0..n-2-i).each { |j|
				x,y = z,@a[j+1]
				if x>y then
					@a[j],z = y,x
				else
					@a[j],z = x,y
				end
			}
			@a[n-1-i]=z
		}
	end
	def verify(n)
		(0..n-2).each { |i|
			if(@a[i]>@a[i+1]) then
				return false
			end
		}
		return true
	end
	def measure(n)
		fill_random(n)
		
		start_time=Time.now
		sort(n)
		end_time=Time.now
		elapsed_time=(end_time-start_time).to_f()
		
		return verify(n)?elapsed_time:Float::NAN
	end
end

def run
	nn=[500,2000,4000,6000,8000,10000]
	s=N2Sort.new(nn[-1])
	nn.each { |n|
		print n.to_s()+","
		4.times {
			print "\t"
			print s.measure(n)
			print ","
		}
		print "\n"
	}
end

run
