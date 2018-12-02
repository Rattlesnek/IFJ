

def fun (a)
    if a <= 0 then
        a - 1
    else
        x = a - 1
        x = baz x
        x = x + a
    end
end

def baz (b)
    if a <= 0 then
        a - 1
    else
        x = a - 1
        x = fun x
        x + a
    end
end


y = fun 5
print y
print "\n"
     
 

