#!/usr/bin/env python3

# This script walks through a pandoc-generated rst file and improves its
# formatting so that it looks more like:
#      https://github.com/sphinx-doc/sphinx/tree/3.x/doc/man
#
# Headings 
#  - First level headings are underlined with '='.
#  - Second level headings are underlined with '~'.
#
# Highlighting
#  - The first mention of program (e.g., MPI_Abort) is annotated like the following:
#      :program:`MPI_Abort`
#  - All MPI_* commands are listed verbatim, with no emphasis: ``MPI_Abort``
#
# Special sections (code blocks, parameter lists)
#  - Put parameters are on a single line: * ``parameter``:  description
#  - Prefix code blocks with:
#      .. code-block:: [LANGUAGE]
#         :linenos:
#
#  TODO
#  - Add ".. :seelso::" for MPI commands that do not forward to this page
#    (Note to self: verify that doing this would actually produce good output.)
#  - Convert existing "SEE ALSO" sections into ".. :seelso::" 

import re
import sys
import os

APPNAME=os.path.basename(__file__)
DIRNAME=os.path.dirname(__file__)

def usage():
    print(f"{APPNAME} <input_file> [<output_file>]\n")

# Get input and optional output files
in_fname = sys.argv[1]

# TODO: optionally print to output file
if (len(sys.argv) > 2):
    out_fname=sys.argv[2]
    with open(out_fname,'w') as outfile:
        indname=os.path.dirname(in_fname)
else:
    outfile=sys.stdout

# TODO: append to output_lines instead of printing lines
output_lines = list()

# Read input as an array of lines, then 
# walk through the input, identifying sections by their headings.
with open(in_fname) as fp:
    in_lines = fp.readlines()

# PATTERNS
# delimiter line (occurs after the heading text)
dline=re.compile("^[=]+")

# Heading patterns

# heading2 may contain lowercase letters
heading2=re.compile("^[A-Z][A-Za-z]*")

# Syntax heading2 indicates language
syntaxsect=re.compile(".*Syntax$")

# Indicates parameters in body
paramsect=re.compile(".*PARAMETER")

# codeblock pattern
codeblock=re.compile("^::")

# repl functions
def mpicmdrepl(match):
    match = match.group()
    match = match.replace('*','')
    return ('``' + match + '``')

# for labeling codeblocks
LANGUAGE="FOOBAR_ERROR"

# for keeping track of state
PARAM=False
CODEBLOCK=False

# for tracking combined lines
SKIP=0

# Walk through all the lines, working on a section at a time
for i in range(len(in_lines)):
  curline = in_lines[i].rstrip()
  if (i < len(in_lines) - 1):
    nextline = in_lines[i+1].rstrip()
  if (i > 0):
    prevline = in_lines[i-1].rstrip()

  if dline.match(nextline):
    CODEBLOCK=False
    SKIP+=1
    if paramsect.match(curline):
      PARAM=True
    else:
      PARAM=False
    if (curline.isupper()):
      # level 1 heading
      print(f"{curline}\n{re.sub('=','-',nextline)}")
    else:
      print(f"{curline}\n{re.sub('=','~',nextline)}")
    if syntaxsect.match(curline):
      LANGUAGE=(re.split(' ',curline)[0]).lower()
  elif codeblock.match(curline):
      print(f"\n.. code-block:: {LANGUAGE}\n   :linenos:")
      CODEBLOCK=True
      SKIP+=1
  else:
      if (SKIP == 0):
        if CODEBLOCK:
          print(in_lines[i-1])
        elif PARAM:
          # combine into parameter bullet-item (Note: check if multiline param)
          paramline1 = re.sub('\n','',curline)
          paramline1 = re.sub('^[ ]*','',paramline1)
          paramline2 = nextline
          while not nextline:
            paramline2 += re.sub('^[ ]*','',nextline)
            nextline = in_lines[i+1]
            SKIP+=1

          print(f"* ``{paramline1}``: {paramline2}")
        else:
            # e.g., turn **MPI_Abort** and *MPI_Abort* into ``MPI_Abort``
            curline = re.sub(r'[\*]*MPI_[A-Z][A-Za-z_]*[\*]*',mpicmdrepl,curline)
            print(f"{curline}")
      else: 
          SKIP-=1
