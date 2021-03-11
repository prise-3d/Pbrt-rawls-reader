import os
import argparse
import glob

def main():

    parser = argparse.ArgumentParser(description="Convert rawls file into png")

    parser.add_argument('--folder', type=str, help='folder with all rawls files', required=True)
    parser.add_argument('--output', type=str, help='folder with all png files', required=True)

    args = parser.parse_args()

    p_folder = args.folder
    p_output = args.output

    images_path = glob.glob(f"{p_folder}/**/**/*.rawls")

    for img in sorted(images_path):

        output_path = img.replace('.rawls', '.png')
        output_path = output_path.replace(p_folder, p_output)

        output_folder, _ = os.path.split(output_path)

        if not os.path.exists(output_folder):
            os.makedirs(output_folder)
        
        if not os.path.exists(output_path):
            os.system(f'./build/main/rawls_convert --image {img} --outfile {output_path}')
        else:
            print(f'{output_path} already exists')
    

if __name__ == "__main__":
    main()