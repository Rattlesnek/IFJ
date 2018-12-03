

def fun?(a) 
    c = 10 + a
    print c
    print a
end

def func?   (a, b, c)
    print a
    print b
    print c
    "as" + "bd\n"
end

def baz! (a, b)  
    print a
    print b
    1 + 3
end 

def bazmeg!()
    print "bazmeg\n"
end

a = 1
b = "ok\n"

fun? a

x = func?(a, b, "as\n")
print x

x = baz! 1.5, "ok"
print x

bazmeg!

def fun (a)
end


