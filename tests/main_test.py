#!/usr/bin/env python3

#######################################################
#
#  FileName    [main_test.py]
#
#  SystemName  [Main test -- IFJ compiler]
#
#  Author      [Adam Pankuch]
#
#  Date        [26/11/2018]
#
########################################################

import os
import sys
import subprocess

class BinariesNotFound(Exception): pass
class TestFilesNotFound(Exception): pass

errors = {
    "SUCCESS" : 0,
    "ERR_LEX" : 1,
    "ERR_SYN" : 2,
    "ERR_SEM_UNDEF" : 3,
    "ERR_SEM_TYPE" : 4,
    "ERR_SEM_FUNC" : 5,
    "ERR_SEM_OTHER" : 6,
    "ERR_ZERO_DIV" : 9,
    "ERR_INTERNAL" : 99
}

def check_all_needed_files():
    if not (os.path.isfile('../parser') and os.path.isfile('../ic18int') and os.path.isfile('./ifj18.rb')):
        raise BinariesNotFound()


def prepare_filenames(folder):
    if not os.path.isdir(folder + '/logs'):
        os.mkdir(folder + '/logs')

    test_input = folder + '/test_input.rb'
    user_input = folder + '/user_input.txt'

    generate = folder + '/logs/generate.IFJcode18'
    inter_out = folder + '/logs/interpret.output'
    ruby_out = folder + '/logs/ruby.output'
    valgrind = folder + '/logs/valgrind.log'

    return test_input, user_input, generate, inter_out, ruby_out, valgrind


def return_call(names, run_valgrind):
    with open(names[0], "r") as fr:
        first_line = fr.readline().strip()

    add = ' false 0 ' + run_valgrind
    if first_line.startswith('# ERROR'):
        err_name = first_line.replace('# ERROR', '').strip()
        add = ' true ' + str(errors[err_name]) + ' ' + run_valgrind

    call = './test_run.sh'
    for name in names:
        call += ' ' + name
    call += add

    return call


def run_test(folder, run_valgrind):
    names = prepare_filenames(folder)
    if not (os.path.isfile(names[0]) and os.path.isfile(names[1])):
        raise TestFilesNotFound(folder)
    
    call = return_call(names, run_valgrind)
    
    print("{0:.<25}".format(folder[6:]), end=" ", flush=True)
    subprocess.call(call, shell=True)


if os.getcwd()[-5:] != 'tests':
    os.chdir('./tests')

try:
    if len(sys.argv) == 1:
        run_valgrind = 'false'
    else:
        if sys.argv[1] == "-v":
            run_valgrind = 'true'

    check_all_needed_files()
    for folder in sorted(os.listdir('tests')):
        folder = 'tests/' + folder
        if os.path.isdir(folder):
            run_test(folder, run_valgrind)
except BinariesNotFound:
    print('One of files: ../parser ../ic18int ./ifj18.rb is missing')
    sys.exit(1)
except TestFilesNotFound as err:
    print('Test file test_input.rb or user_input.txt is missing in folder: ', err)
    sys.exit(1)
except KeyError as err:
    print('Key:', err, 'not in known errors')
    print('Error occurred in folder:', folder)
    sys.exit(1)
