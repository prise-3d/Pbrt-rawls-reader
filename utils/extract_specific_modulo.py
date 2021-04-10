import os
import argparse
import glob


def main():

    parser = argparse.ArgumentParser(description="Extract specific samples indices")

    parser.add_argument('--folder', type=str, help='folder with all rawls files', required=True)
    parser.add_argument('--modulo', type=int, help='every image to take', required=True)
    parser.add_argument('--output', type=str, help='folder with all png files', required=True)

    args = parser.parse_args()

    p_folder = args.folder
    p_output = args.output
    p_modulo = args.modulo

    images_path = glob.glob(f"{p_folder}/**/**/*.rawls")

    for img in sorted(images_path):

        img_index = int(img.split('-').split('.')[0])

        if ((img_index + 1) % p_modulo) == 0:

            output_path = img.replace(p_folder, p_output)
            output_folder, _ = os.path.split(output_path)

            if not os.path.exists(output_folder):
                os.makedirs(output_folder)
            
            if not os.path.exists(output_path):
                os.system(f'cp {img} {output_path}')
                print(f'Copy of {img}')
            else:
                print(f'{output_path} already exists')
    

if __name__ == "__main__":
    main()