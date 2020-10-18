import os
import argparse

def main():

    estimators = ["median", "variance", "std", "skewness", "kurtosis", "mode"]

    parser = argparse.ArgumentParser("Run estimators reconstruction")
    parser.add_argument('--folder', type=str, help='folder with rawls scene data', required=True)
    parser.add_argument('--nfiles', type=int, help='expected number of rawls files', required=True)
    parser.add_argument('--est', type=str, help='estimator to use', choices=estimators, required=True)
    parser.add_argument('--tiles', type=str, help='tiles size: 100,100', default="100,100")
    parser.add_argument('--output', type=str, help='output folder', required=True)

    args = parser.parse_args()

    p_folder = args.folder
    p_nfiles = args.nfiles
    p_est = args.estimators
    x_tile, y_tile = list(map(int, args.tiles.split(',')))
    p_output = args.output

    scenes = sorted(os.listdir(p_folder))

    for scene in scenes:
        scene_path = os.path.join(p_folder, scene)
        nelements = len(os.listdir(scene_path))

        if nelements == p_nfiles:
            print('Extraction of {0} estimator for {1} scene'.format(p_est, scene))

            output_folder = os.path.join(p_output, p_est, scene)

            if not os.path.exists(output_folder):
                os.makedirs(output_folder)

            outfilename = os.path.join(output_folder, scene + '_10000.rawls')

            if not os.path.exists(outfilename):
                os.system('./build/main/extract_stats_images --folder {0} --bwidth {1} --bheight {2} --outfile {3} --estimator {4}'.format(scene_path, x_tile, y_tile, p_est, outfilename))
            else:
                print('Already generated')



if __name__ == "__main__":
    main()