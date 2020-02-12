#! /bin/bash

if [ -z "$1" ]
  then
    echo "No argument supplied"
    echo "Need data folder"
    exit 1
fi

if [ -z "$2" ]
  then
    echo "No argument supplied"
    echo "Need output folder"
    exit 1
fi

if [ -z "$3" ]
  then
    echo "No argument supplied"
    echo "Need step output images expected (step of samples)"
    exit 1
fi

prefix="p3d_"
build_folder="build"

folder_path=$1
output_folder=$2
step=$3

nb_elements=$(ls -l ${folder_path} | grep ${prefix} | wc -l)

IFS='/' read -ra ADDR <<< "${folder_path}"
folder=${ADDR[-1]}

mkdir -p $output_folder

counter=0
startindex=0
while [ $startindex -lt $nb_elements ]
do
  startindex=$(($startindex + $step))

  outfile="${folder}_${step}_${counter}.png"
  echo $outfile

  ./${build_folder}/main/rawls_merge_mean --folder ${folder_path} --random 1 --samples ${step} --outfile $output_folder/$outfile

  counter=$(( $counter + 1 ))
done
  
      
