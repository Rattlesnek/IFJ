

def fun (a, b)
    print "in fun\n"
    
    if a < b then
        x = "if\n"
        print a
    else
        x = "else\n"
        print b
    end
    print x
    print "\n"
end


def baz (a, b)
    
    print "\nbaz a is\n"
    print a 
    print "\nbaz b is\n"
    print b
    print "\n"

    x = 10
    while a < b do
        print "while\n"
        a = a + 1
        print a
        print "\n"
        y = a
    end

    num = y + x
    print num
    print "end\n"
end
 

a = 0
b = 1
while a < 5 do

    fun(a , 2)

    baz a, b 
    print a
    print b

    b = b + 2
    a = a + 1
end
