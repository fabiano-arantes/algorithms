#!/usr/local/bin/python

import sys, os, argparse, shutil, ntpath

PRJ_NAME_TAG = "%prj_name%"
EMPTY_CMAKELISTS_PATH = "empty-project/CMakeLists.txt"
EMPTY_MAIN_PATH = "empty-project/empty.c"

def create_prj(folder, name):
	try: 
		os.makedirs(folder)
	except OSError:
		if not os.path.isdir(folder):
			raise

	f1 = open(EMPTY_CMAKELISTS_PATH, 'r')
	f2 = open(folder + "/" + ntpath.basename(EMPTY_CMAKELISTS_PATH), 'w')
	for line in f1:
	    f2.write(line.replace(PRJ_NAME_TAG, name))
	f1.close()
	f2.close()

	shutil.copy("empty-project/empty.c", folder + "/" + name + ".c")

	with open("CMakeLists.txt", "a") as myfile:
		myfile.write("ADD_SUBDIRECTORY(\"${BASE_DIR}/" + folder + "\")\n")


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description="Create a new project")
    parser.add_argument('folder', help="Source code folder name")
    parser.add_argument('name', help="Project name")

    args = parser.parse_args()
    create_prj(args.folder, args.name)
    
    