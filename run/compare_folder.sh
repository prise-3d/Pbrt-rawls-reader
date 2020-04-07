if [ -z "$1" ]
  then
    echo "No argument supplied"
    echo "Folder with images to compare"
    exit 1
fi

if [ -z "$2" ]
  then
    echo "No argument supplied"
    echo "Expected metric choice"
    exit 1
fi

if [ -z "$3" ]
  then
    echo "No argument supplied"
    echo "Output markdown filename"
    exit 1
fi

folder_path=$1
metric=$2
markdown_file=$3


header_line="# | "
table_line="---|"
for image in $(ls ${folder_path}/)
do
    IFS='.' read -ra ADDR <<< "${image}"
    image_name=${ADDR[0]}

    IFS='_' read -ra ADDR <<< "${image_name}"
    image_index=${ADDR[-1]}

    header_line="${header_line} ${image_index} |"
    table_line="${table_line}---|"
done

echo $folder_path > $markdown_file
echo ""
echo ${header_line} >> $markdown_file
echo ${table_line} >> $markdown_file

for image_i in $(ls ${folder_path}/)
do
    IFS='.' read -ra ADDR <<< "${image_i}"
    image_i_name=${ADDR[0]}

    IFS='_' read -ra ADDR <<< "${image_i_name}"
    image_i_index=${ADDR[-1]}


    line="${image_i_index} | "
    
    for image_j in $(ls ${folder_path}/)
    do
        echo "Comparisons between ${image_i} and ${image_j}"
        estimated_error=$(python utils/compare_images.py --img1 ${folder_path}/${image_i} --img2 ${folder_path}/${image_j} --metric ${metric})
        line="${line} ${estimated_error} | "
    done

    echo ${line} >> $markdown_file
done