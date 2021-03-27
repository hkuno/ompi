#!/usr/bin/env python3

# Regular expression module
# See https://docs.python.org/3/library/re.html
import re
import sys

fname=sys.argv[1]

# Helpful for when learning python -- "prettyprint"
from pprint import pprint

# Read in as an array of lines. 
with open(fname) as fp:
    foo_lines = fp.readlines()

# PATTERNS

# delimiter line (occurs after the heading text)
dline=re.compile("^[=]+")

# Heading patterns

# heading1 does NOT contain lowercase letters
lowercase=re.compile(".*[a-z]")
alpha=re.compile(".*[a-zA-Z]")

# heading2 may contain lowercase letters
head2cand=re.compile("^[A-Z][A-Za-z]*")

# Syntax heading2 indicates language
Syntaxcand=re.compile(".*Syntax$")

# Indicates parameters in body
paramcand=re.compile(".*PARAMETER")


# codeblock pattern
codeblock=re.compile("^::")

# repl functions
def mpicmdrepl(match):
    match = match.group()
    match = match.replace('*','')
    return ('``' + match + '``')

def cmdrepl(match):
    match = match.group()
    return ('``' + match + '``')

# for labeling codeblocks
LANGUAGE="FOOBAR_ERROR"

# for keeping track of state
PARAM=False
CODEBLOCK=False

# for tracking combined lines
SKIP=0

# We walk through all the lines, looking ahead one or two lines
for i in range(len(foo_lines)):
  if (i > 0):
    if dline.match(foo_lines[i]):
      CODEBLOCK=False
      SKIP+=1
      if (not lowercase.match(foo_lines[i-1]) and paramcand.match(foo_lines[i-1])):
        PARAM=True
      else:
        PARAM=False
      if not lowercase.match(foo_lines[i-1]):
        # level 1 heading
        print(f"{foo_lines[i-1]}{re.sub('=','-',foo_lines[i])}",end='')
      elif Syntaxcand.match(foo_lines[i-1]):
        # level 2 heading
        print(f"{foo_lines[i-1]}{re.sub('=','~',foo_lines[i])}",end='')
        LANGUAGE=(re.split(' ',foo_lines[i-1])[0]).lower()
      elif head2cand.match(foo_lines[i-1]):
        # level 2 heading
        print(f"{foo_lines[i-1]}{re.sub('=','~',foo_lines[i])}",end='')
    elif codeblock.match(foo_lines[i]):
      # codeblock
      print(f"\n.. code-block:: {LANGUAGE}\n   :linenos:")
      SKIP+=1
      CODEBLOCK=True
    else:
      if (SKIP == 0):
        if PARAM:
          # parameter bullet-item
          if (alpha.match(foo_lines[i-1])):
            curline = re.sub('\n','',foo_lines[i-1])
            curline = re.sub('^[ ]*','',curline)
            nextline = re.sub('^[ ]*','',foo_lines[i])
            print(f"* ``{curline}``: {nextline}",end='')
            SKIP+=1
          else:
            print(f"{foo_lines[i-1]}",end='')
        elif CODEBLOCK:
          # body text
            print(foo_lines[i-1], end='')
        else:
            # e.g., turn **MPI_Abort** into MPI_Abort
            prevline = re.sub(r'[\*]*MPI_[A-Z][A-Za-z_]*[\*]*',mpicmdrepl,foo_lines[i-1])

            # e.g., turn MPI_Abort into ``MPI_Abort``
            prevline = re.sub(r'\*[a-z][a-z]*',cmdrepl,prevline)
            print(f"{prevline}",end='')
      else: 
          SKIP-=1

# print out last line
print(foo_lines[len(foo_lines)-1],end='')
