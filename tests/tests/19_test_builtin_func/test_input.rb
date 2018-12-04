a = 98
b = "JAMBALAJA"
c = 4
d = 25

def chris(z)
	a = 5
	ch = chr(a)
	print ch
	ch = chr(z)
	print ch
	ch = chr z
	print ch
	ch = chr 42
	print ch
end

def ordi(q,r)
	ordinal = ord( q, r)
	print ordinal
	ordinal = ord q, 8
	print ordinal
	ordinal = ord "Jak se vede", 3
	print ordinal
	ordinal = ord "Jde to skvele" , r
	print ordinal
end

def printing(o)
	print o
	print "Nazdarek"
end

def inputting(p)
	p = inputi
	print p
	p = inputi()
	print p

	p = inputs
	print p
	p = inputs()
	print p

	p = inputf
	print p
	p = inputf()
	print p
end

def substring(s,b,e)
	sub = substr( s, b, e)
	print sub
	sub = substr "AHOJ svete" , 1, 55
	print sub
	sub = substr s, b, e
	print sub
	sub = substr s, b, 22
	print sub
	sub = substr s, 1, e
	print sub
	sub = substr s, 1, 22
	print sub
end

chris(a)
ordi(b,c)
printing(b)
inputting(a)
substring(b,c,d)

