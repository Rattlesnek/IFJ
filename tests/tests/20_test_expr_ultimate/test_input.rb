
float_1 = 1.5
float_2 = 2.5
int_1 = 1
int_2 = 2


id1 = float_1 + float_2 * 2.5
id2 = int_1 + int_2 * 6


a = 1 / 2.5 - 2.6 / 3 
print a

a = 100 / 2 / 2.5 - 6 / 2
print a

a = 100 / 2 / 2.5 / 3 - 6 / 2
print a

a = 100 / 2 - 85.5 / 2.5 - 5.2 / 1.25 - 52
print a

#################################

a = ( 10 - 5 ) - ( 1.5 - 2 )
print a

a = ( 10 - 2.5 / 2) - ( 2.5 / 2 - 10 / 3 ) / 2.5
print a

a = ( 1.5 - (150.5 / 5 - 100) / (1.5 - 1 / 5.2) ) / 2.5 * 2.89 - 2.8 * (1 / 23.5 - 2)
print a

####################################

a = float_1 / 10 - 2.6 / int_1
print a

a = int_1 / int_1 / float_1 - int_1 / float_2
print a

a = 100 / float_1 - 85.5 / int_1 / float_2 - float_1 + int_1
print a

#####################################

a = ( 10 - int_1 ) - ( float_2 - 2 )
print a

a = ( int_1 - 2.5 / int_1) - ( 2.5 / 2 - float_2 / int_1 ) / float_1
print a

a = ( 1.5 - (float_1 / int_1 - float_2) / (1.5 - float_2/ float_1) ) / float_2 * 2.89 - int_2 * (int_2 / float_2 - 2)
print a


##################################

a = float_1 / 10 - 2.6 / id1
print a

a = id2 / id1 / float_1 - id1 / float_2
print a

a = 100 / float_1 - 85.5 / id1 / float_2 - float_1 + id1
print a

###################################

a = ( 10 - id1 ) - ( id2 - id1 )
print a

a = ( int_1 - 2.5 / id1) - ( 2.5 / 2 - id2 / int_1 ) / float_1
print a

a = ( 1.5 - (float_1 / id2 - id2) / (1.5 - float_2/ id1) ) / float_2 * id1 - int_2 * (id2 / id1 - 2)
print a

######################################

id_float = id1 / 2.5
id_int = id2 / 3

a = id_float / id_int - id_int / id_float
print a

a = ( id_int - id_int ) - ( id_float - id_float )
print a

a = ( id2 - (float_1 / id_float - id2) / (float_1- id_float/ id1) ) / float_2 * id1 - int_2 * (id2 / id1 - id_float)
print a


############# MASTER
a = ( 1.5 - (150.5 / 5 - (float_1 / id_float - id2) / (float_1- id_float / ( id1 * ( 10 - id1 ) - ( id2 - id1 )) + 100 ) ) / ( ( ( int_1 - 2.5 / int_1) - ( 2.5 / 2 - float_2 / int_1 ) / float_1 ) - (1.5 - ( int_1 - 2.5 / int_1) - ( 2.5 / 2 - float_2 / int_1 ) / float_1 * 1 / 5.2) ) / 2.5 * 2.89 - 2.8 * (1 / 23.5 - 2) )
print a



def fun(id_float, id_int, float_1, float_2, int_1, int_2, id1, id2)
    a = 1 / 2.5 - 2.6 / 3 
    print a

    a = 100 / 2 / 2.5 - 6 / 2
    print a

    a = 100 / 2 / 2.5 / 3 - 6 / 2
    print a

    a = 100 / 2 - 85.5 / 2.5 - 5.2 / 1.25 - 52
    print a

    #################################

    a = ( 10 - 5 ) - ( 1.5 - 2 )
    print a

    a = ( 10 - 2.5 / 2) - ( 2.5 / 2 - 10 / 3 ) / 2.5
    print a

    a = ( 1.5 - (150.5 / 5 - 100) / (1.5 - 1 / 5.2) ) / 2.5 * 2.89 - 2.8 * (1 / 23.5 - 2)
    print a

    ####################################

    a = float_1 / 10 - 2.6 / int_1
    print a

    a = int_1 / int_1 / float_1 - int_1 / float_2
    print a

    a = 100 / float_1 - 85.5 / int_1 / float_2 - float_1 + int_1
    print a

    #####################################

    a = ( 10 - int_1 ) - ( float_2 - 2 )
    print a

    a = ( int_1 - 2.5 / int_1) - ( 2.5 / 2 - float_2 / int_1 ) / float_1
    print a

    a = ( 1.5 - (float_1 / int_1 - float_2) / (1.5 - float_2/ float_1) ) / float_2 * 2.89 - int_2 * (int_2 / float_2 - 2)
    print a


    ##################################

    a = float_1 / 10 - 2.6 / id1
    print a

    a = id2 / id1 / float_1 - id1 / float_2
    print a

    a = 100 / float_1 - 85.5 / id1 / float_2 - float_1 + id1
    print a

    ###################################

    a = ( 10 - id1 ) - ( id2 - id1 )
    print a

    a = ( int_1 - 2.5 / id1) - ( 2.5 / 2 - id2 / int_1 ) / float_1
    print a

    a = ( 1.5 - (float_1 / id2 - id2) / (1.5 - float_2/ id1) ) / float_2 * id1 - int_2 * (id2 / id1 - 2)
    print a

    ######################################

    a = id_float / id_int - id_int / id_float
    print a

    a = ( id_int - id_int ) - ( id_float - id_float )
    print a

    a = ( id2 - (float_1 / id_float - id2) / (float_1- id_float/ id1) ) / float_2 * id1 - int_2 * (id2 / id1 - id_float)
    print a

    ############# MASTER
    a = ( 1.5 - (150.5 / 5 - (float_1 / id_float - id2) / (float_1- id_float / ( id1 * ( 10 - id1 ) - ( id2 - id1 )) + 100 ) ) / ( ( ( int_1 - 2.5 / int_1) - ( 2.5 / 2 - float_2 / int_1 ) / float_1 ) - (1.5 - ( int_1 - 2.5 / int_1) - ( 2.5 / 2 - float_2 / int_1 ) / float_1 * 1 / 5.2) ) / 2.5 * 2.89 - 2.8 * (1 / 23.5 - 2) )
    print a
end


fun(id_float, id_int, float_1, float_2, int_1, int_2, id1, id2)

