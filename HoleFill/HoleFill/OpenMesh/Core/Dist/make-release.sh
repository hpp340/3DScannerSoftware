#!/bin/sh
#
#

if [ -z $1 ]; then
   echo "Usage: make-release <version>"
   echo "   Exports given version to current directory."
   echo "   After exporting the html documentation is generated."
   echo "   After checking if everything exists the source packages"
   echo "   are created."
   echo ""
   echo "Example"
   echo "   make-release 0-11-0"
   exit
fi

# -------------------------------------------------------------- settings ----

VER=$1
DIR=OpenMesh-$VER
SYM=OpenMesh-$VER
SRC=OpenMesh_`echo $VER|tr '-' '.'`

# ----------------------------------------------------------------------------

if [ -d $DIR ]; then
  echo "Warning! Release $VER already exists!"
  read -n 1 -p "Shall I remove it first? [y/N] "
  echo
  if ! [ -z $REPLY ] && [ $REPLY == "y" ]; then
     echo "  removing $DIR first..."
     rm -rf $DIR
     rm -f $SRC.tar.* $SRC.zip
     mkdir $DIR
  else
    echo "Cannot proceed! Release making aborted!"
    exit 1
  fi
else
  mkdir $DIR
fi

# cd to new directory
pushd $DIR > /dev/null 2>&1
if [ $? != 0 ]; then
  echo "Cannot proceed! Missing directory '$DIR'!"
  exit 1
fi

# -------------------------------------------------------- export version ----

echo "Exporting version $SYM"
cvs export -r $SYM OM_DIST > /dev/null 2>&1


# ----------------------------------------------------------------- check ----
if ! [ -d OpenMesh ]; then
   echo "ERROR: Missing subdirectory OpenMesh."
   echo "(Does the label '$SYM' exist?)"
   popd > /dev/null
   rm -rf $DIR
   exit;
fi

# ------------------------------------------------ generate documentation ----

echo "  generate documentation"
pushd OpenMesh/Doc > /dev/null
acgmake
popd > /dev/null


# ------------------------------------------------- create source packages ----
echo "  creating source packages"
echo "    $SRC.tar.gz"
tar c OpenMesh/ | gzip -9 > ../$SRC.tar.gz
echo "    $SRC.tar.bz2"
tar c OpenMesh/ | bzip2 -9 > ../$SRC.tar.bz2
echo "    $SRC.zip"
zip -r ../$SRC.zip OpenMesh/ > /dev/null

# -----------------------------------------------------------------------------
popd > /dev/null
echo "Done"
