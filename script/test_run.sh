#!/usr/bin/env bash
RED='\033[0;31m'
NC='\033[0m' 
GREEN='\e[32m'
DIM='\e[2m'

run_compiler()
{
    valgrind --leak-check=yes --error-exitcode=20 --log-file="$3" ../parser <$1 >$2
    if [[ "$?" == "20" ]]
    then
        printf "${RED}(X)${NC}: "
    else
        printf "${GREEN}(*)${NC}: "
    fi
}

if [[ $# -eq 6 ]] && [[ -f $1 ]] && [[ -f $2 ]]
then
    testing=$1
    user_input=$2
    generate=$3
    inter_out=$4
    ruby_out=$5
    valgr=$6

    run_compiler $testing $generate $valgr
    ./ic18int $generate <$user_input >$inter_out

    ruby -r ./ifj18.rb $testing <$user_input >$ruby_out
    
    diff $inter_out $ruby_out >/dev/null
    if [[ $? == 0 ]] 
    then
        printf "${GREEN}SUCCESS${NC}\n"
    else
        printf "${RED}FAIL${NC}\n"
    fi
else
    echo "HELP: ./test.sh f_testing f_input f_generate f_interpret_out f_ruby_out f_valgrind"
fi