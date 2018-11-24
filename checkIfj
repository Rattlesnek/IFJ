RED='\033[0;31m'
NC='\033[0m' 
GREEN='\e[32m'
DIM='\e[2m'

SUCCESS=0
ERR_LEX=1
ERR_SYN=2
ERR_SEM_UNDEF=3
ERR_SEM_TYPE=4
ERR_SEM_FUNC=5
ERR_SEM_OTHER=6
ERR_ZERO_DIV=7
ERR_INTERNAL=9


test_valgrind()
{
    valgrind --leak-check=yes --error-exitcode=1 ./parser < sc_test.rb &> /dev/null  
    if [[ $? -eq 0 ]]
    then
        printf "${GREEN}(*)${NC}: "
    else
         printf "${RED}(*)${NC}: "
    fi
}

test_function() {
     touch sc_test.rb
     echo "$1" > sc_test.rb
     printf "${DIM}"
     ./parser < sc_test.rb
     if [[ $? -eq $2 ]] 
     then
         printf "${NC}"
         printf "${GREEN}SUCCESS ${NC}"
         if [[ "${3}" == "-v" ]]
         then
            test_valgrind $1 $2 
         fi
         printf "${1}\n\n"
     else
         printf "${NC}"
         printf "${RED}FAIL ${NC}"
         if [[ "${3}" == "-v" ]]
         then
            test_valgrind $1 $2
         fi
         printf "${1}\n\n"
     fi
}

test_f "a = 1+1"
# BUILTIN FUNKCE

if [[ "${1}" == "-builtin" ]]
then

    if [[ "${2}" == "-success" ]]
    then
        echo "--------------return SUCCESS--------------"
        test_function "a = inputs" $SUCCESS $3
        test_function "a = inputs()" $SUCCESS $3
        test_function "a = inputi" $SUCCESS $3
        test_function "a = inputi()" $SUCCESS $3
        test_function "a = inputf" $SUCCESS $3
        test_function "a = inputf()" $SUCCESS $3
        test_function "a = chr 1" $SUCCESS $3
        test_function "a = chr(1)" $SUCCESS $3
        test_function "a = length \"string\"" $SUCCESS $3
        test_function "a = length(\"string\")" $SUCCESS $3
        test_function "a = ord \"string\", 1" $SUCCESS $3
        test_function "a = ord(\"string\", 1)" $SUCCESS $3
        test_function "print \"tortuga\",1" $SUCCESS $3
    fi

    if [[ "${2}" == "-err_sem_func" ]]
    then
        echo "--------------return ERR_SEM_FUNC--------------"
        test_function "a = inputs 1" $ERR_SEM_FUNC $3
        test_function "a = inputs(1)" $ERR_SEM_FUNC $3
        test_function "a = inputi 1" $ERR_SEM_FUNC $3
        test_function "a = inputi(1)" $ERR_SEM_FUNC $3
        test_function "a = inputf 1" $ERR_SEM_FUNC $3
        test_function "a = inputf(1)" $ERR_SEM_FUNC $3
        test_function "a = chr 1,2" $ERR_SEM_FUNC $3
        test_function "a = chr(1,2)" $ERR_SEM_FUNC $3
        test_function "a = length \"string\", 1" $ERR_SEM_FUNC $3 
        test_function "a = length(\"string\", 1)" $ERR_SEM_FUNC $3
        test_function "a = ord \"string\", 1, 2" $ERR_SEM_FUNC $3
        test_function "a = ord(\"string\", 1, 2)" $ERR_SEM_FUNC $3
    fi

fi

if [[ "$1" == "-expr" ]]
then
    if [[ "${2}" == "-success" ]]
    then
        test_function "a = 1+1" $SUCCESS $3
        test_function "a = 1-1" $SUCCESS $3
        test_function "a = 1*1" $SUCCESS $3
        test_function "a = 1/1" $SUCCESS $3
        test_function "a = (1+1)/2*3" $SUCCESS $3
    fi

    if [[ "${2}" == "-err_syn" ]]
    then
        test_function "a = 1++1" $ERR_SYN $3
        test_function "a = -1" $ERR_SYN $3
        test_function "a = 2.3 -- 2" $ERR_SYN $3
    fi
fi

if [[ "$1" == "--help" ]] || [[ $# -eq 0 ]]
then
    printf "./test.sh [-type-of-test] [-type-of-error] [-valgrind]\n"
    printf "[-type-of-test ]: -builtin (to test builtin functions)\n"
    printf "[-type-of-error]: -success (to test successful expr)\n"
    printf "                  -err_sem_func (to test wrong number of params)\n"
    printf "[-valgrind     ]: -v (to test memory leaks)\n"
fi

rm sc_test.rb