#!/usr/bin/env bash

#######################################################
#
#  FileName    [test_run.sh]
#
#  SystemName  [Test -- IFJ compiler]
#
#  Author      [Adam Pankuch]
#
#  Date        [26/11/2018]
#
########################################################

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
ERR_ZERO_DIV=9
ERR_INTERNAL=99


run_with_valgrind()
{
    valgrind --leak-check=yes --error-exitcode=20 --log-file="$3" ../parser <$1 >$2
    if [[ "$?" == "20" ]]
    then
        printf "${RED}(X)${NC}: "
    else
        printf "${GREEN}(*)${NC}: "
    fi
}

if [[ $# == 9 ]] && [[ -f $1 ]] && [[ -f $2 ]]
then
    testing=$1
    user_input=$2
    generate=$3
    inter_out=$4
    ruby_out=$5
    valgr=$6
    err_test=$7
    expected_err=$8
    run_valgr=$9

    if [[ "$run_valgr" == "true" ]]
    then
        run_with_valgrind $testing $generate $valgr
    else
        ../parser <$testing >$generate
        retval=$?
        if [[ $retval == $SUCCESS ]]
        then
            printf "${GREEN}compile ok ${NC}"
        else
            if [[ "$err_test" == "true" ]]
            then
                printf "${GREEN}compile fail (exit code: $retval)\n${NC}" 
            else
                printf "${RED}compile fail (exit code: $retval)\n${NC}" 
            fi
        fi
    fi

    if [[ "$err_test" == "false" ]]
    then
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
        ../parser <$testing >$generate 2>/dev/null
        return_code=$?
        
        if [[ $return_code == $SUCCESS ]]
        then
            ./ic18int $generate <$user_input >$inter_out
            return_code=$?
        fi

        if [[ $return_code == "$expected_err" ]]
        then
            printf "${GREEN}SUCCESS (error same as expected)${NC}\n"
        else
            printf "${RED}FAIL (expected error: $expected_err got: $return_code) ${NC}\n"
        fi
    fi
else
    echo "HELP: ./test.sh f_testing f_input f_generate f_interpret_out f_ruby_out f_valgrind {true, false} {0,1,2,3,4,5,6,7,99}"
fi