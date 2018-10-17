#!/usr/bin/env python3
import sys
import collections

# Spustenie: 
# ./first.py [subor s pravidlami -- grammar.txt]

# Vystup: 
# Vypise mnozinu First kazdeho z neterminalov v subore rules.txt

# Mnozinu First hlada 2 roznymi algoritmami: Rekurzivne a Nerekurzivne
# 
# Rekurzivny alg.
#  - vypise "mnozinu First" -- rovnake terminaly moznu nachadzat viac krat
#  - ak sa tam jeden terminal nachadza viac krat znamena to,
#    ze navrh je nedeterministicky 
#   
# Nerekurzivny alg.
#  - vypise mnozinu First -- je len na overenie spravnosti rekurzivneho :D


rec_cnt = 0
nor_cnt = 0

def print_first(first, allow_term=True, allow_no_right_side=True):
    for key, vals in sorted(first.items()):
        if not allow_term and is_terminal(key):
            continue
        if not allow_no_right_side and vals == []:
            continue
        print("{0:<14}--first-- ".format(key), end="")
        for val in sorted(vals):
            print(val, "", end="")
        print()


def is_comment_or_empty(line):
    return not ( line == "" or line.startswith("#") )

def is_terminal(item):
    return not ( item.startswith("<") and item.endswith(">") )


def first_search_rec(first, rules):

    def recursive_adding_terminals(store, key):
        global rec_cnt
        #print("key", key) 
        if key not in rules.keys():
            print("== Possible error: recursion may have stopped ==")
            return
        for item_seq in rules[key]:
            item = item_seq[0]
            # informative variable
            rec_cnt += 1
            #print("item", item)
            if is_terminal(item):
                store.append(item)
            else:
                recursive_adding_terminals(store, item)

    # start recursive search 
    for key in rules.keys():
        recursive_adding_terminals(first[key], key)


def first_search_norm(first, rules):
    global nor_cnt

    for key in first.keys():
        if is_terminal(key):
            first[key].append(key)
    
    while True:
        added = False
        for key in first.keys():
            for item_seq in rules[key]:
                item = item_seq[0]
                for val in first[item]:   
                    # informative variable
                    nor_cnt += 1
                    if val not in first[key]:
                        added = True
                        first[key].append(val)
        if added == False:
            break
    


rules = collections.defaultdict(list)
nonterm_plus_term = set()  

# parse file and save to rules dict and nonterm_plus_term set
try:
    filename = sys.argv[1] 
    for line in open(filename):
        line = line.strip()
        if is_comment_or_empty(line):
            left_side, right_side = line.split("->")
            left_side = left_side.strip()
            right_side = tuple( right_side.strip().split(" ") )
            # add to rules
            rules[left_side].append(right_side)

            nonterm_plus_term.add(left_side)
            for expr in right_side:
                nonterm_plus_term.add(expr)
except IndexError:
    print("Missing file", file=sys.stderr)
    sys.exit(1)
except IOError:
    print("File does not exist", file=sys.stderr)
    sys.exit(1)


# RECURSIVE
print("\nRecursive: (shows non-terminal more than once if there are multiple paths to that non-terminal)")
# calculate
first_rec = collections.defaultdict(list)
first_search_rec(first_rec, rules)
# output
print("number of cycles:", rec_cnt, end="\n\n")
print_first(first_rec)


# NON - RECURSIVE
print("\nNon-recursive:")
# calculate
first_norm = {expr : [] for expr in nonterm_plus_term}
first_search_norm(first_norm, rules)
# output
print("number of cycles:", nor_cnt, end="\n\n")
print_first(first_norm, allow_term=False, allow_no_right_side=False)

           