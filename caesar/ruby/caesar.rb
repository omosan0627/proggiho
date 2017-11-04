#!/usr/bin/ruby

i=STDIN
o=STDOUT

if(ARGV.length>=1) then
	i=File.new(ARGV[0],"r") if("-"!=ARGV[0])
end
o=File.new(ARGV[1],"w") if(ARGV.length>=2)

i.each_char { |c|
	cc=c.unpack("C")[0]
	cc=cc+1 if(/\w/ =~ c)
	o.putc(cc)
}

o.close
i.close
