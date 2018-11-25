#!/usr/bin/env bash
if [[ ! -f "parser" ]]
then
    echo "parser binary does not exists"
    exit
fi

if [[ ! -f "ic18int" ]]
then
    echo "ic18int binary does not exists"
    exit
fi


if [[ -f $1 ]]
then
    cat $1 | ./parser >generated.IFJcode18
    ./ic18int generated.IFJcode18
else
    echo "HELP: ./run.sh file_with_ruby_code"
fi