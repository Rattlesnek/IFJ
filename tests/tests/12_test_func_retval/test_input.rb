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


def bazmeg(a, b)
    if a < b then
        a + 1
    else
        b + 1
    end
end

y = bazmeg(1, 2)
print y
print "\n"

y = bazmeg 3.5, 2
print y
print "\n"


def baz_while()
    i = 0
    while i < 2 do
        print i
        i = i + 1
    end
end


y = baz_while
print "\n"
print y
print "\n"

def fun_assign(ifx, a, b)
    sum = a + b
    diff = a + b

    
    if ifx then
        x = sum
    else
        y = diff
    end
end

x = fun_assign(nil, 1, 2)
print "\n"
print x
print "\n"

x = fun_assign("as", 1, 2)
print "\n"
print x
print "\n"

def bazing()
    1
end

def ck()
    1.5
end

x = ck()
print x

def idk()
    "as"
end

x = idk
print x

def ids(a, b)
    a + b
end

x = ids 2.5, 1
print x

def ints()
    1 + 2
end

x = ints
print x

def floats()
    1.5 - 2
end

x = floats
print x

def strings()
    "as" + "5"
end 

x = strings
print x


def ifs()
    if 1 then
    else
        1
    end
end

x = ifs()
print x

def empty()


end

x = empty()
print x