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
#  - All MPI_* commands are listed verbatim, with no emphasis: ``MPI_Abort``
#
# Special sections (code blocks, parameter lists)
#  - Put parameters are on a single line: * ``parameter``:  description
#  - Prefix code blocks with:
#      .. code-block:: [LANGUAGE]
#         :linenos:
#  - Leave other verbatim blocks alone.
#  - Combine bullet items into a single line
#
#  TODO
#  - Write output to array and then print to file or stdout
#  - Replace NAME with the name of the program? Or add a line like the following?
#      :program:`MPI_Abort`
#  - Convert existing "SEE ALSO" sections into ".. :seelso::" 
#  - Add ".. :seelso::" for MPI commands that do not forward to this page
#    (Note to self: verify that doing this would actually produce good output.)

import re
import sys
import os

APPNAME=os.path.basename(__file__)
DIRNAME=os.path.dirname(__file__)

def usage():
    print(f"{APPNAME} <input_file> [<output_file>]\n")

# Get input and optional output files
in_fname = sys.argv[1]
CMDNAME = os.path.basename(in_fname).rsplit('.',100)[0]

# TODO: optionally print to output file
out_fname=""
if (len(sys.argv) > 2):
    out_fname=sys.argv[2]

# TODO: append to output_lines instead of printing lines
output_lines = list()

# Add a reference for each file
refline=".. _{}:\n".format(CMDNAME)
output_lines.append(refline)

# Read input as an array of lines, then 
# walk through the input, identifying sections by their headings.
with open(in_fname) as fp:
    in_lines = fp.readlines()

# PATTERNS
# include file
include_pat = re.compile("\.\. include::")

# delimiter line (occurs after the heading text)
dline=re.compile("^[=]+")

# name
name=re.compile("^name$", flags = re.IGNORECASE | re.MULTILINE)

# break out of a literal
# This is an over-simplification because really a literal block
# breaks when we return to the previous indentation level.
unliteral=re.compile("^[A-Za-z]")

# bullet item
bullet=re.compile("^[\s]*[^*]\- ")

# distinguish MPI_Cmd from MPI_ARG
contains_lowercase=re.compile(".*[a-z]")

# Indicates parameters in body
paramsect=re.compile(".*PARAMETER")

# includes ':'
contains_colon=re.compile(".*:")

# codeblock pattern
literalpat=re.compile("^::")

# languages
fortran_lang=re.compile(".*Fortran", re.IGNORECASE)
cpp_lang=re.compile(".*C\+\+", re.IGNORECASE)
c_lang=re.compile(".*C[^a-zA-Z]", re.IGNORECASE)

# repl functions
# :ref:`my-reference-label`:
seealsodict=dict()
def mpicmdrepl(match):
    match = match.group()
    match = match.replace('`','')
    match = match.replace('*','')
    if (contains_lowercase.match(match)):
        seealsodict[match]=match
    return (':ref:`' + match + '` ')

# for labeling codeblocks
LANGUAGE="FOOBAR_ERROR"

# We only need to detect languages that ompi supports
# Just pick the first match.
def get_cb_language(aline):
    #LANG="FOOBAR_ERROR: " + aline
    LANG=""
    if fortran_lang.match(aline):
      LANG="fortran" 
    elif cpp_lang.match(aline):
      LANG="c++" 
    elif c_lang.match(aline):
      LANG="c" 
    return(LANG)

# for keeping track of state
BULLETITEM=False
LITERAL=False
PARAM=False

# So we don't repeat combined or replaced lines
SKIP=0

# Walk through all the lines, working on a section at a time
# Leave LITERAL sections alone.
# In PARAMETER sections, combine bullets into a single line.
# If not a LITERAL section, list MPI commands as a reference:
# :ref:`my-reference-label`:
# If a code-block section, add notation.
for i in range(len(in_lines)):
  curline = in_lines[i].rstrip()
  if unliteral.match(curline):
    LITERAL=False
  if (i > 0):
    prevline = in_lines[i-1].rstrip()
  if (i == len(in_lines) - 1):
    if ((not include_pat.match(curline)) and (not LITERAL)):
      curline = re.sub(r'[\*]*[\`]*MPI_[A-Z][\*,()\[\]0-9A-Za-z_]*[()\[\]0-9A-Za-z_]*[\`]*[\*]*',mpicmdrepl,curline)
    if (not SKIP):
      output_lines.append(f"{curline}")
  else:
    nextline = in_lines[i+1].rstrip()
    if (i + 3 < len(in_lines)):
      nextnextnextline = in_lines[i+3].rstrip()
    else:
      nextnextnextline = ""
    if dline.match(nextline):
      LITERAL=False
      PARAM=False
      SKIP+=1
      if paramsect.match(curline):
        PARAM=True
      if (curline.isupper()):
        if name.match(curline): 
          # Substitute program name because html index needs it.
          # Only substitute delimeter for the first NAME heading because 
          # build-doc seems to expect a single-rooted hierarchy.
          output_lines.append(f"{CMDNAME}\n{re.sub('[A-Z,a-z,0-9,_,-]','~',CMDNAME)}")
        else:
          output_lines.append(f"{curline}\n{nextline}")
      else:
        # level 2 heading
        output_lines.append(f"{curline}\n{re.sub('=','-',nextline)}")
    elif literalpat.match(curline):
        LITERAL=True
        prevlangline=prevline
        nextlangline=nextline
        d=1
        while (not prevlangline) or dline.match(prevlangline):
          prevlangline = in_lines[i-d].rstrip()
          curlangline = in_lines[i-d+1].rstrip()
          d += 1
        LANGUAGE = get_cb_language(prevlangline)
        if (not LANGUAGE):
          if not dline.match(nextnextnextline):
            output_lines.append(f"{curline}")
        else:
          output_lines.append(f".. code-block:: {LANGUAGE}\n   :linenos:\n")
          SKIP+=1
    else:
#      print(f"166: SKIP is {SKIP}; LITERAL is {LITERAL}; PARAM is {PARAM}")
      if (SKIP == 0):
        if LITERAL:
          output_lines.append(f"{curline}")
        elif PARAM:
          # combine into parameter bullet-item (Note: check if multiline param)
          if not curline:
            output_lines.append(f"{curline}") 
          else:
            paramline2=""
            paramline1 = re.sub('^[ ]*','',curline)
            if (contains_colon.match(curline)):
              paramline1,paramline2 = re.split(r':',paramline1)
            if not paramline2:
              paramline2 = re.sub('^[ ]*','',nextline)
              SKIP+=1
            d=1
            nextpline=in_lines[i+d].rstrip()
            while (nextpline):
              d += 1
              nextpline=in_lines[i+d].rstrip()
              paramline2 += ' ' + re.sub('^[ ]*','',nextpline)
              SKIP += 1
            output_lines.append(f"* ``{paramline1}``: {paramline2}\n")
              # e.g., turn **MPI_Abort** and *MPI_Abort* into ``MPI_Abort``
        elif ((not LITERAL) and (not dline.match(nextline))):
          curline = re.sub(r'[\*]*[\`]*MPI_[A-Z][\*,()\[\]0-9A-Za-z_]*[()\[\]0-9A-Za-z_][\`]*[\*]*',mpicmdrepl,curline)
          if bullet.match(curline):
            d=1
            nextbline=in_lines[i+d].rstrip()
            bline2 = nextline
            while (nextbline):
              d += 1
              nextbline=in_lines[i+d].rstrip()
              bline2 += ' ' + re.sub('^[ ]*','',nextbline)
              SKIP += 1
            output_lines.append(f"{curline} {bline2}\n")
          else:
            output_lines.append(f"{curline}")
      else: 
        SKIP-=1


seealso='\n.. seealso::'
for k, v in seealsodict.items():
    if ( k != CMDNAME ):
        seealso=seealso + ' :ref:`' + k + '`'

output_lines.append(seealso)

if (out_fname):
  with open(out_fname,'w') as outfile:
    sys.stdout = outfile
    for line in output_lines:
      print(line) 
else:
    for line in output_lines:
      print(line) 
