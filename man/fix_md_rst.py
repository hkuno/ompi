#!/usr/bin/env python3

# This script walks through an rst file generated from md and improves its
# formatting. 
#
# Headings 
#  - Replace initial NAME with program name and underline with '~'.
#  - Leave first level headings are underlined with '='.
#  - Leave second level headings are underlined with '-'.

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

# optionally print to output file
out_fname=""
if (len(sys.argv) > 2):
    out_fname=sys.argv[2]

# append to output_lines instead of printing lines
output_lines = list()

# delimiter line (occurs after the heading text)
dline=re.compile("^[=]+")

# name
name=re.compile("^name$", flags=re.IGNORECASE | re.MULTILINE)

# Read input as an array of lines, then 
# walk through the input, identifying sections by their headings.
with open(in_fname) as fp:
    in_lines = fp.readlines()

# PATTERNS
# delimiter line (occurs after the heading text)
dline=re.compile("^[=]+")

# So we don't repeat combined or replaced lines
SKIP=0

# Walk through all the lines, working on a section at a time
for i in range(len(in_lines)):
  curline = in_lines[i].rstrip()
  nextline = curline
  if (i < (len(in_lines) - 1)):
      nextline = in_lines[i+1].rstrip()

  if name.match(curline) and dline.match(nextline):
      # Substitute program name because html index needs it.
      # Only substitute delimeter for the first NAME heading because 
      # build-doc seems to expect a single-rooted hierarchy.
      output_lines.append(f"{CMDNAME}\n{re.sub('[A-Z,a-z,0-9,_,-]','~',CMDNAME)}")
      SKIP += 1
  elif (SKIP == 0):
      output_lines.append(f"{curline}")
  else: 
      SKIP -= 1

if (out_fname):
  with open(out_fname,'w') as outfile:
    sys.stdout = outfile
    for line in output_lines:
      print(line) 
else:
    for line in output_lines:
      print(line) 
