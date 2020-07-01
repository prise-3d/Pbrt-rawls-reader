import os, sys
import argparse
import subprocess

def write_progress(progress):
    '''
    Display progress information as progress bar
    '''
    barWidth = 150

    output_str = "["
    pos = barWidth * progress
    
    for i in range(barWidth):
        if i < pos:
           output_str = output_str + "="
        elif i == pos:
           output_str = output_str + ">"
        else:
            output_str = output_str + " "

    output_str = output_str + "] " + str(int(progress * 100.0)) + " %\r"
    print(output_str)
    sys.stdout.write("\033[F")


def main():

    parser = argparse.ArgumentParser(description="Update whole scene folder .rawls files to new .rawls version format")

    parser.add_argument('--folder', type=str, help='folder with all .rawls scenes sub folder', required=True)
    parser.add_argument('--output', type=str, help='output expected folder (can be the same)', required=True)

    args = parser.parse_args()

    p_folder   = args.folder
    p_output   = args.output

    # check if executable file is available
    executable_filepath = './build/main/rawls_update'

    if not os.path.exists(executable_filepath):
        print("Executable '{0}' does not exist or is not accessible as expected.".format(executable_filepath))

    scenes = sorted(os.listdir(p_folder))

    for scene in scenes:

        scene_path = os.path.join(p_folder, scene)

        # create output scene path if does not exist
        output_scene_path = os.path.join(p_output, scene)

        if not os.path.exists(output_scene_path):
            os.makedirs(output_scene_path)

        print('Convert .rawls for: {0}'.format(scene))

        images = sorted(os.listdir(scene_path))
        n_images = len(images)

        for i, img in enumerate(images):

            img_path = os.path.join(scene_path, img)
            out_img_path = os.path.join(output_scene_path, img)

            # create and launch command
            subprocess.call([executable_filepath, '--image', img_path, '--outfile', out_img_path])

            write_progress((i + 1 ) / float(n_images))

        print('\n')

if __name__ == "__main__":
    main()