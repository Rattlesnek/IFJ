def fun(a)
    print a
    a + 1
end


x = fun 1
print x
print "\n"


def baz (a, b)
    inter = 10
    print a
    print b
    
    a + b - inter
end

y = baz 2.5, 5
print y
print "\n"



def bum ()
    baz(1, 1)
end
     
y = bum()
print y
print "\n"
