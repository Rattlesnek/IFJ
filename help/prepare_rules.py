#!/usr/bin/env python3
import sys
import collections


def is_comment_or_empty(line):
    return not ( line == "" or line.startswith("#") )

def is_terminal(item):
    return not ( item.startswith("<") and item.endswith(">") )


rules = collections.defaultdict(dict)
nonterm_plus_term = [] 

# parse file and save to rules dict and nonterm_plus_term set
try:
    filename = sys.argv[1] 
    for line in open(filename):
        line = line.strip()
        if is_comment_or_empty(line):
            left_side, right_side = line.split("->")
            rule_num, left_side = left_side.split(".")

            rule_num = int(rule_num)
            left_side = left_side.strip()
            right_side = tuple( reversed( right_side.strip().split(" ") ) )
            # add to rules
            rules[left_side][rule_num] = right_side
except IndexError:
    print("Missing file", file=sys.stderr)
    sys.exit(1)
except IOError:
    print("File does not exist", file=sys.stderr)
    sys.exit(1)



for key in rules.keys():
    for num in rules[key].keys():
        print("{ ", end="")
        for item in rules[key][num]:
            if not is_terminal(item):
                item = "\"[" + item[1:-1] + "]\""
                print(item, ", ", sep="", end="")
            else:
                item = "\"" + item + "\""
                print(item, ", ", sep="", end="")
        print("},")