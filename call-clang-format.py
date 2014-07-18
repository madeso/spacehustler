#!/usr/bin/python
import argparse
import sys
import glob
import platform
from subprocess import call

parser = argparse.ArgumentParser(description="Run clang-format with globbing support and less arguments")
parser.add_argument('files', metavar ='FILE', nargs='+', help='A file, may contain wildcards to modify with clang-format')
args = parser.parse_args()

clangformat = "clang-format"
# on osx assume that the clang-format is installed via alcatraz in eclipse
if platform.mac_ver() != '':
	clangformat = "~/Library/Application\ Support/Developer/Shared/Xcode/Plug-ins/ClangFormat.xcplugin/Contents/Resources/clang-format"

print "clang-format: ", clangformat

error_count = 0

lastcmd = []

try:
	for dir in args.files:
		count = 0
		for filename in glob.glob(dir):
			count += 1
			lastcmd = [clangformat, "-i", "-style=Google", filename]
			retcode = call(lastcmd)
			if retcode <> 0:
				error_count += 1
				sys.stderr.write(filename + ' failed to clang-format\n')
			# clang-format ARGS -i -style=Google ${game_src}
		if count == 0:
			error_count += 1
			sys.stderr.write(dir + ' didnt yield any files\n')
except OSError, e:
	sys.stderr.write("""Failed to call clang-format!
		Error: %s
		Command: %s
		""" % (str(e), str(lastcmd)) )

sys.exit(error_count > 0)