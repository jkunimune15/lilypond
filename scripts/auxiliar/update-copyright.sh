#!/bin/bash

echo "Update copyright dates in LilyPond sources"
echo
if [ $# -eq 0 ]
  then
    targetyear=$(date +"%Y")
    echo "No target year supplied. Using current year $targetyear instead."
  else
    #TODO: sanity check?
    targetyear="$1"
fi

# Change to repository's root
cd $(git rev-parse --show-cdup)

#TODO: is there a more efficient way of finding
# all files modified in a given year?
# It might be helpful to reduce the file list to files containing
# the copyright notice in the first place?
for f in $(find . -type f | grep -v "\.git")
do
  # We have to filter out the commit changing copyright to 2016 (in 2017!)
  commit=$(git rev-list -n 1 HEAD $f | grep -v "fc765145")
  if [ "$commit" != "" ]
  then
    date=$(git log -1 --date=short --pretty='format:%cd' "$commit")
    year=${date:0:4}
    if [ "$year" = "$targetyear" ]
    then
      echo "Check/update $f: $commit, $date"
      sed -i "s/\(Copyright.*[0-9]*--\)\([0-9]*\)/\1$targetyear/g" "$f"
    fi
  fi
done
