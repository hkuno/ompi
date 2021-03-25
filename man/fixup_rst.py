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

# heading delimiter (occurs after the heading text)
dline=re.compile("^[=]+")

# patterns that can come before dline (indicate heading level/type)
lowercase=re.compile(".*[a-z]")
head2cand=re.compile("^[A-Z][A-Za-z]*")
Syntaxcand=re.compile(".*Syntax$")
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

LANGUAGE="FOOBAR_ERROR"
PARAM=False
CODEBLOCK=False
SKIP=0
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
        print("{}{}".format(foo_lines[i-1],re.sub('=','-',foo_lines[i])), end='')
      elif Syntaxcand.match(foo_lines[i-1]):
        # level 2 heading
        print("{}{}".format(foo_lines[i-1],re.sub('=','~',foo_lines[i])), end='')
        LANGUAGE=(re.split(' ',foo_lines[i-1])[0]).lower()
      elif head2cand.match(foo_lines[i-1]):
        # level 2 heading
        print("{}{}".format(foo_lines[i-1],re.sub('=','~',foo_lines[i])), end='')
    elif codeblock.match(foo_lines[i]):
      # codeblock
      print(f".. code-block:: {LANGUAGE}\n   :linenos:")
      SKIP+=1
      CODEBLOCK=True
    else:
      if (SKIP == 0):
        if PARAM and not dline.match(foo_lines[i+1]):
          # parameter bullet-item
          
          curline = re.sub('\n','',foo_lines[i])
          curline = re.sub('[ ]*','',curline)
          nextline = re.sub('^[ ]*','',foo_lines[i+1])
          print(f"* ``{curline}``: {nextline}",end='')
#          print("* ``{}``: {}".format(re.sub('\n','',foo_lines[i]),foo_lines[i+1]), end='')
          SKIP+=2
        else:
          # body text
          if CODEBLOCK:
            print(foo_lines[i-1], end='')
          else:
            prevline = re.sub(r'[\*]*MPI_[A-Z][A-Za-z_]*[\*]*',mpicmdrepl,foo_lines[i-1])
            prevline = re.sub(r'\*[a-z][a-z]*',cmdrepl,prevline)
            print(prevline,end='')
      else: SKIP-=1

print(foo_lines[len(foo_lines)-1],end='')
