#!/bin/bash

# This file is for sharing notes with Jeff.

MANHOME=./
BUILDDIR=./_build/
BUILDMAN=./_build/man/
TMPRST=$(pwd)/tmprst/
mkdir -p $TMPRST
MDTMPRST=./tmpmdrst/
mkdir -p $MDTMPRST
MDTMPRST=$( cd $MDTMPRST; pwd -P )
BUILDRST=./rst/
BUILDHTML=./_build/html/
MANDIRS="ompi/mpi/man oshmem/shmem/man"
TSTMAN_ORIG=./tmpman_orig/
TSTMAN_NEW=./tmpman_new/
TSTMAN_DIFF=./tmpman_diff/

mkdir -p $BUILDMAN
mkdir -p $BUILDRST
BUILDRST=$(cd $BUILDRST ; pwd -P )
cp conf.py $BUILDRST/
cp index.rst $BUILDRST/

# For each man page, if the man page includes another,
# convert nroff ".so" command to rst ".. include::"
# else use pandoc to convert the man page to rst.
if [[ 0 -eq 1 ]] ; then
    echo "About to convert man to rst"
    SAVEME=$( pwd )
    ERRORFILE=pandoc_man2rst.output
    cat /dev/null > $ERRORFILE
    for d in $MANDIRS ; do
        for f in $( find $MANHOME/$d -name \*.\*in ) ; do
            f2=$( echo $f | sed -e "s/\.\([0-9]*\)in/.\1/" )
            out=$TMPRST/${f2}.rst
            echo "converting $f to $out" >> $ERRORFILE
            # force .so nroff into: .. include:: filename
            if [[ $( grep -w '^\.so' $f | wc -l ) -eq 1 ]] ; then
                out=$BUILDRST/${f2}.rst
                mkdir -p $( dirname $out )
                fname=$( grep -w '^\.so' $f | awk '{printf"%s.rst",$2}' )
                [[ -z "$fname" ]] && echo "WARNING: ERROR: See $fname"
                pname=$( basename $out | awk -F\. '{print $1}' )
                delim=$( echo $pname | sed -e "s/[a-z,A-Z,0-9,_,\-]/=/g" )
                echo $delim > $out
                echo $pname >> $out
                echo $delim >> $out
                echo " " >> $out
                echo ".. include:: ../${fname}" >> $out
            else
                mkdir -p $( dirname $out )
                cd $( dirname $f )
                f2=$( basename $f)
                pandoc -f man -t rst $f2 1> $out 2>> $ERRORFILE
                cd $SAVEME
            fi
        done
    done

    # Check for warning messages
    echo "WARNING messages from $ERRORFILE:" $( grep WARNING $ERRORFILE | wc -l )
fi

# For each man md page
# use pandoc to convert the man page to rst.
if [[ 0 -eq 1 ]] ; then
    echo "About to convert md to rst"
    SAVEME=$( pwd )
    ERRORFILE=pandoc_md2rst.output
    cat /dev/null > $ERRORFILE
    for d in $MANDIRS ; do
        for f in $( find $MANHOME/$d -name \*.md ) ; do
            f2=$( echo $f | sed -e "s/\.\([0-9]*\).md/.\1/" )
            out=$MDTMPRST/${f2}.rst
            mkdir -p $( dirname $out )
            echo "converting $f to $out"
            echo "converting $f to $out" >> $ERRORFILE
            cd $( dirname $f )
            f2=$( basename $f)
            pandoc -f gfm -t rst $f2 1> $out 2>> $ERRORFILE
            cd $SAVEME
        done
    done

    # Check for warning messages
    echo "WARNING messages from $ERRORFILE:" $( grep WARNING $ERRORFILE | wc -l )
fi

# fix up rst files
SAVEME=$( pwd )
if [[ 1 -eq 1 ]] ; then
    echo "About to fix up rst"
    date
    for d in $MANDIRS ; do
        for f in $( cd $TMPRST/$d; find . -name \*.rst ) ; do
            infile="$TMPRST/$d/$f"
            out="$BUILDRST/$d/$f"
            mkdir -p $( dirname $out )
            echo "Fixing $infile as $out"
            python3 fixup_rst.py $infile $out
        done
    done
    date
fi

# fix up rst files generated from md
SAVEME=$( pwd )
if [[ 1 -eq 1 ]] ; then
    echo "About to fix up rst generated from md files"
    date
    for d in $MANDIRS ; do
        if [[ -d $MDTMPRST/$d ]]; then
            for f in $( cd $MDTMPRST/$d; find . -name \*.rst ) ; do
                infile="$MDTMPRST/$d/$f"
                out="$BUILDRST/$d/$f"
                mkdir -p $( dirname $out )
                echo "Fixing $infile as $out"
                python3 fix_md_rst.py $infile $out
            done
        fi
    done
    date
fi

# use sphinx-build to create html files
if [[ 1 -eq 1 ]] ; then
    echo "About to create html files"
    date
    SAVEME=$( pwd )
    cd $BUILDDIR
    ERRORFILE=sphinx-build_rst2html.output
    cat /dev/null > $ERRORFILE
    sphinx-build -b html -c rst rst/ html/ >& $ERRORFILE
    cd $SAVEME
    date
fi


# use sphinx-build to create man files
if [[ 1 -eq 1 ]] ; then
    echo "About to create man files"
    date
    ERRORFILE=sphinx-build_rst2man.output
    SAVEME=$( pwd )
    cd $BUILDDIR/
    sphinx-build -vvv -b man -c rst  rst/ man/ >& $ERRORFILE
    cd $SAVEME
date
fi

# check the man pages by comparing against original man pages
j=0
if [[ 1 -eq 1 ]] ; then
    SAVEME=$( pwd -P )
    mkdir -p $TSTMAN_NEW
    TSTMAN_NEW=$( cd $TSTMAN_NEW; pwd -P )
    cd $BUILDMAN
    for i in $( /bin/ls ) ; do
        if [[ ! -d $TSTMAN_NEW/$i ]] ; then
            out=${TSTMAN_NEW}/${i}
            ( man ./${i} >& $out ) &
            j=$(( j + 1 ))
            [[ $(( $j%5 )) -eq 0 ]] && wait
        fi
    done
    cd $SAVEME
fi
wait

if [[ 1 -eq 1 ]] ; then
    mkdir -p $TSTMAN_ORIG
    TSTMAN_ORIG=$( cd $TSTMAN_ORIG; pwd -P )
    SAVEME=$( pwd -P )
    cd $MANHOME
    i=0
    for d in $MANDIRS ; do
        for f in $( find $d -name \*.\*in -type f ) ; do
            if [[ ! -d $TSTMAN_NEW/$i ]] ; then
                sbname=$( basename $f | sed -e "s/in$//" )
                out=$TSTMAN_ORIG/$sbname
                (man ${f} >& $out) &
                i=$(( i + 1 ))
                [[ $(( $i%5 )) -eq 0 ]] && wait
            fi
        done
    done
    cd $SAVEME
fi
wait

if [[ 1 -eq 1 ]] ; then
    SAVEME=$( pwd -P )
    TSTMAN_ORIG=$( cd $TSTMAN_ORIG; pwd -P )
    TSTMAN_NEW=$( cd $TSTMAN_NEW; pwd -P )
    mkdir -p $TSTMAN_DIFF
    TSTMAN_DIFF=$( cd $TSTMAN_DIFF; pwd -P )
    echo "TSTMAN_DIFF is $TSTMAN_DIFF"
    j=0
    for i in $( /bin/ls $TSTMAN_NEW ) ; do
        if [[ ! -d $TSTMAN_NEW/$i ]] ; then
            out=${TSTMAN_DIFF}/$i
            f1=${TSTMAN_NEW}/${i}
            f2=${TSTMAN_ORIG}/${i}
            if [[ -f $f1 ]] && [[ -f $f2 ]] ; then
                ( diff $f1 $f2 >& $out ) &
                j=$(( j + 1 ))
                [[ $(( $j%5 )) -eq 0 ]] && wait
            else
                echo "Missing: $f1 or $f2"
            fi
        fi
    done
fi
wait


# TO DO LIST:
# Decide what the directory structure should be with docs and man
# Fix the index.rst and make files, which I copied from docs.
# Fix ompi/mpi/man/./man3/MPI_Type_hindexed.3.rst by hand
