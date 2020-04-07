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
build_folder="build"

ext=$2
output_folder=$3


mkdir -p ${output_folder}
for file in $(ls ${main_folder})
do 
    filename=${main_folder}/${file}
    filename_fixed=${filename//\/\//\/}

    IFS='.' read -ra ADDR <<< "${file}"

    filename_without_ext=${ADDR[0]}
    outfile="${filename_without_ext}.${ext}"

    echo ${filename_fixed}
    echo ${output_folder}/${outfile}

    #check if filename contains 
    # if [[ "$file" == ${prefix}* ]]; then
          ./${build_folder}/main/rawls_h_flip --image ${filename_fixed} --outfile ${output_folder}/${outfile}
    # fi
done 