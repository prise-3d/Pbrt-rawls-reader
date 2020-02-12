#! /bin/bash
if [ -z "$1" ]
  then
    echo "No argument supplied"
    echo "Need folder argument"
    exit 1
fi

if [ -z "$2" ]
  then
    echo "No argument supplied"
    echo "Need output extension"
    exit 1
fi

if [ -z "$3" ]
  then
    echo "No argument supplied"
    echo "Need output folder"
    exit 1
fi


main_folder=$1
prefix="p3d_"

ext=$2
output_folder=$3

for folder in $(ls -d -- ${main_folder}*/)
do
  for scene in $(ls -d $folder)
  do
    for file in $(ls $folder)
    do 
        filename=$folder$file
        filename_fixed=${filename//\/\//\/}

        IFS='.' read -ra ADDR <<< "${file}"

        filename_without_ext=${ADDR[0]}
        outfile="${filename_without_ext}.${ext}"

        # check if filename contains 
        if [[ "$file" == ${prefix}* ]]; then
            ./main/rawls_convert --image ${filename_fixed} --outfile ${output_folder}/${outfile}
        fi
    done 
  done
done