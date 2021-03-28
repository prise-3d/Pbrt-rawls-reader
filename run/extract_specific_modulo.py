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

        # replace expected Samples value
        img_data = img.split('-')
        index_img = int(img_data[-1].split('.')[0])

        if (index_img % p_modulo) - 1 == 0:

            output_path = img.replace(p_folder, p_output)
            output_folder, _ = os.path.split(output_path)

            if not os.path.exists(output_folder):
                os.makedirs(output_folder)
            
            if not os.path.exists(output_path):
                os.system(f'cp {img} {output_path}')
            else:
                print(f'{output_path} already exists')
        else:
            print(f'Not considered: {img}')
    

if __name__ == "__main__":
    main()