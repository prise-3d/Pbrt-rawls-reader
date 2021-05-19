import os
import argparse
import glob


def main():

    parser = argparse.ArgumentParser(description="Extract specific samples indices")

    parser.add_argument('--folder', type=str, help='folder with all rawls files', required=True)
    parser.add_argument('--index', type=str, help='current rawls image index', required=True)
    parser.add_argument('--nsamples', type=str, help='expected nsamples for image', required=True)
    parser.add_argument('--output', type=str, help='folder with all png files', required=True)

    args = parser.parse_args()

    p_folder = args.folder
    p_output = args.output
    p_index = args.index
    p_samples = args.nsamples

    expected_index = str(p_index)

    while len(expected_index) < 6:
        expected_index = "0" + expected_index

    output_index = ""
    while len(output_index) < 6:
        output_index = "0" + output_index

    images_path = glob.glob(f"{p_folder}/**/**/*{expected_index}.png")

    for img in sorted(images_path):

        # replace expected Samples value
        img_data = img.split('/')[-1].split('-')
        img_data[-2] = "S" + p_samples
        img_data[-1] = output_index + ".png"

        output_path = '-'.join(img_data)

        output_path = os.path.join(p_output, img.split('/')[-2], output_path)
        output_folder, _ = os.path.split(output_path)

        if not os.path.exists(output_folder):
            os.makedirs(output_folder)
        
        if not os.path.exists(output_path):
            os.system(f'cp {img} {output_path}')
        else:
            print(f'{output_path} already exists')
    

if __name__ == "__main__":
    main()