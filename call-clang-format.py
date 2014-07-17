import argparse
import sys
import glob
from subprocess import call

parser = argparse.ArgumentParser(description="Run clang-format with globbing support and less arguments")
parser.add_argument('files', metavar ='FILE', nargs='+', help='A file, may contain wildcards to modify with clang-format')
args = parser.parse_args()

error_count = 0

for dir in args.files:
	count = 0
	for filename in glob.glob(dir):
		count += 1
		retcode = call(["clang-format", "-i", "-style=Google", filename])
		if retcode <> 0:
			error_count += 1
			sys.stderr.write(filename + ' failed to clang-format\n')
		# clang-format ARGS -i -style=Google ${game_src}
	if count == 0:
		error_count += 1
		sys.stderr.write(dir + ' didnt yield any files\n')

sys.exit(error_count > 0)