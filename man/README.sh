#!/bin/bash

# This file is for sharing notes with Jeff.

MANHOME=./
BUILDDIR=./_build/
BUILDMAN=./_build/man/
TMPRST=$(pwd)/tmprst/
TMPRST2=./tmprst/
BUILDRST=./rst/
BUILDHTML=./_build/html/
MANDIRS="ompi/mpi/man oshmem/shmem/man"

# For each man page, if the man page includes another,
# convert nroff ".so" command to rst ".. include::"
# else use pandoc to convert the man page to rst.
if [[ 0 -eq 1 ]] ; then
    SAVEME=$( pwd )
    ERRORFILE=pandoc_man2rst.output
    cat /dev/null > $ERRORFILE
    for d in $MANDIRS ; do
        for f in $( find $MANHOME/$d -name \*.\*in ) ; do
            f2=$( echo $f | sed -e "s/\.\([0-9]*\)in/.\1/" )
            out=$TMPRST/${f2}.rst
            mkdir -p $( dirname $out )
            echo "converting $f to $out" >> $ERRORFILE
            # force .so nroff into: .. include:: filename
            if [[ $( grep '^.so' $f | wc -l ) -eq 1 ]] ; then
                fname=$( grep '^.so' $f | awk '{printf"%s.rst",$2}' )
                [[ -z "$fname" ]] && echo "ERROR: See $fname"
                echo ".. include:: ../${fname}" > $out
            else
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

# fix up rst files
SAVEME=$( pwd )
if [[ 1 -eq 1 ]] ; then
    for d in $MANDIRS ; do
        for f in $( cd $TMPRST2/$d; find . -name \*.rst ) ; do
            infile="$TMPRST2/$d/$f"
            out="$BUILDRST/$d/$f"
            mkdir -p $( dirname $out )
            python3 fixup_rst.py $infile > $out
        done
    done
fi

# use pandoc to convert every rst page to _build/man
# Note that the regenerated nroff files are not identical to the originals.
header="\"\#OMPI_DATE\#\" \"\#PACKAGE_VERSION\#\" \"\#PACKAGE_NAME\#\""
if [[ 0 -eq 1 ]] ; then
    ERRORFILE=pandoc_rst2man.output
    cat /dev/null > $ERRORFILE
    for d in $MANDIRS ; do
        SAVEME=$( pwd )
        cd $BUILDRST/$d
        for f in $( find ./ -name \*.rst ) ; do
            out=$( echo "$SAVEME/${BUILDMAN}/$d/$f" | sed -e "s/\.rst//" )
            odir=$( dirname $out )
            mkdir -p $odir
            echo "Converting $f to $out" 
            if [[ $( grep ".. include::" $f | wc -l ) -eq 1 ]] ; then
                pandoc -f rst -t man $f 1> $out 2>> $ERRORFILE
                fname=$( grep '.. include::' $f | awk '{printf"%s",$NF}' | sed -e "s:../::" )
                [[ -z "$fname" ]] && echo "ERROR: See $fname"
                echo ".so ${fname}" | sed -e "s/\.rst//" > $out
            else
                title=$( basename $f | sed -e "s/\.*//" )
                section=$( basename $( dirname $f ) | sed -e "s/man//" )
                echo "Converting $f to $out" >> $ERRORFILE
                echo ".TH $title $section $header" > $out
                pandoc -f rst -t man $f |\
                    sed -e "s/^\.SH/\n\n.SH/" |\
                    sed -e "s/^\.PP/\n\n.PP/" |\
                    sed -e "s/^\.fi/\n\n.fi/" 1>> $out 2>> $ERRORFILE
            fi
        done
        cd $SAVEME
    done
    # 233 .so files
    echo "Count of WARNING messages:" $( grep WARNING $ERRORFILE | wc -l )
    cd $SAVEME
fi

# use pandoc to create html files
if [[ 0 -eq 1 ]] ; then
    for d in $MANDIRS ; do
        SAVEME=$( pwd )
        cd $BUILDRST/$d
        for f in $( find ./ -name \*.rst ) ; do
            out=$( echo "$SAVEME/${BUILDHTML}/$d/$f" | sed -e "s/\.rst/\.html/" )
            odir=$( dirname $out )
            mkdir -p $odir
            echo "Converting $f to $out" 
            pandoc -s -f rst -t html $f 1> $out 2>> $ERRORFILE
        done
        cd $SAVEME
    done
    # 233 .so files
    echo "Count of WARNING messages:" $( grep WARNING $ERRORFILE | wc -l )
fi

# use sphinx-build to create html files
if [[ 0 -eq 1 ]] ; then
    SAVEME=$( pwd )
    cd $BUILDDIR
    ERRORFILE=sphinx-build_rst2html.output
    cat /dev/null > $ERRORFILE
    sphinx-build -b html -c rst rst/ html/ >& $ERRORFILE
    cd $SAVEME
fi

# use sphinx-build to create man files
if [[ 0 -eq 1 ]] ; then
    ERRORFILE=sphinx-build_rst2man.output
    SAVEME=$( pwd )
    cd $BUILDDIR/
    sphinx-build -vvv -b man -c rst  rst/ man/ >& $ERRORFILE
    cd $SAVEME
fi

# TO DO LIST:
#. Decide what the directory structure should be with docs and man
#. Fix the index.rst and make files, which I copied from docs.
#. What to do with the 233 files with nroff .so ? Convert to rst equivalent?
