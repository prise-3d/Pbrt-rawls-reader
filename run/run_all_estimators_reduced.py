import os
import argparse
import time

def main():

    estimators = ["mean", "median", "var", "std", "skewness", "kurtosis", "mode"]

    parser = argparse.ArgumentParser("Run estimators reconstruction")
    parser.add_argument('--folder', type=str, help='folder with rawls scene data', required=True)
    parser.add_argument('--nfiles', type=int, help='expected number of rawls files', required=True)
    parser.add_argument('--tiles', type=str, help='tiles size: 100,100', default="100,100")
    parser.add_argument('--output', type=str, help='output folder', required=True)

    args = parser.parse_args()

    p_folder = args.folder
    p_nfiles = args.nfiles
    x_tile, y_tile = list(map(int, args.tiles.split(',')))
    p_output = args.output

    scenes = sorted(os.listdir(p_folder))

    for scene in scenes:
        scene_path = os.path.join(p_folder, scene)
        nelements = len(os.listdir(scene_path))

        if nelements == p_nfiles:

            # check all scene folder exists
            checked_folder = []

            for est in estimators:
                output_folder = os.path.join(p_output, est, scene)

                if not os.path.exists(output_folder):
                    os.makedirs(output_folder)

                outfilename = os.path.join(output_folder, scene + '.rawls')

                checked_folder.append(os.path.exists(outfilename))

            if not all(checked_folder):
                print(f'Extraction of estimators for {scene} scene')
                t1 = time.time()
                print('./build/main/extract_stats_images_all_reduced --folder {0} --bwidth {1} --bheight {2} --nfiles {3} --output {4}'.format(scene_path, x_tile, y_tile, p_nfiles, p_output))
                t2 = time.time()
                delta = (t2 - t1)
                print(f'Extraction for {scene}, took {delta}')
            else:
                print(f'Estimators for {scene}, already generated')



if __name__ == "__main__":
    main()