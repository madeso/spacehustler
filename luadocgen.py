from __future__ import print_function

#!/usr/bin/python
"""Generates a list of lua markup documents based on c++ code that generates it.
"""

import argparse
import sys
import os

CMD_NONE = 0
CMD_MODULE = 1
CMD_FUNCTION = 2
CMD_DESCRIPTION = 3
CMD_ARGUMENTS = 4
CMD_RETURNS = 5

def firstbold(s):
	index = s.find(' ')
	if index == -1:
		return "*" + s + "*"
	else:
		return "*" + s[0:index] + "*" + s[index:]

class Func:
	def __init__(self, name):
		self.doc = ""
		self.name = name
		self.arg = []
		self.ret = []
	def adddoc(self,line):
		self.doc = self.doc + line
	def addarg(self, arg):
		self.arg.append(arg)
	def addret(self, ret):
		self.ret.append(ret)
	def write(self, target):
		print(self.name, file=target)
		print("-----------", file=target)
		print(self.doc, file=target)
		print("", file=target)
		
		if len(self.arg) > 0:
			print("###Arguments", file=target)
			for a in self.arg:
				print("  - " + firstbold(a), file=target)
			print("", file=target)
		
		if len(self.ret) > 0:
			print("###Returns", file=target)
			for r in self.ret:
				print("  - " + firstbold(r), file=target)
			print("", file=target)
		
		print("", file=target)

class Mod:
	def __init__(self):
		self.funcs = []
	def addfunc(self, func):
		self.funcs.append(func)
	def write(self, target):
		for file in self.funcs:
			file.write(target)

class Mods:
	def __init__(self):
		self.mods = {}
	def mod(self, name):
		if name in self.mods:
			return self.mods[name]
		else:
			m = Mod()
			self.mods[name] = m
			return m
	def write(self, dir):
		for name, mod in self.mods.iteritems():
			filename = os.path.join(dir,name+'.md')
			with open(filename, 'w') as target:
				print(name, file=target)
				print("===========", file=target)
				print("", file=target)
				mod.write(target)

module = ""
function = ""
createnewfunction = True
mods = Mods()
func = None

def addToFunc(func, c, line):
	if c == CMD_DESCRIPTION:
		func.adddoc(line)
	elif c == CMD_ARGUMENTS:
		func.addarg(line)
	elif c == CMD_RETURNS:
		func.addret(line)
	else:
		print("Unknown internal command")

def handleCmd(c, line):
	global module
	global function
	global createnewfunction
	global mods
	global func
	
	if c == CMD_MODULE:
		module = module + line
	elif c == CMD_FUNCTION:
		function = function + line
	else:
		if c == CMD_DESCRIPTION:
			if createnewfunction:
				func = Func(function)
				mods.mod(module).addfunc(func)
			createnewfunction = False
		else:
			createnewfunction = True
		addToFunc(func, c, line)
def main():
	global module
	global function
	global mods
	parser = argparse.ArgumentParser(description='Generate a list of lua markup documents.')
	parser.add_argument('outputdir', help='output directory wher to place the md files')
	parser.add_argument('input', metavar='FILE', type=argparse.FileType('r'), nargs='+',
					   help='c++ file(s) to parse')
	args = parser.parse_args()
	for file in args.input:
		module = ""
		function = ""
		command = CMD_NONE
		for l in file:
			l = l.strip()
			if len(l) > 5:
				if l[0:5] == "// --":
					l = l[5:].strip()
					index = l.find(":")
					cmd = ""
					if index > 0:
						cmd = l[0:index].lower().strip()
						l = l[index+1:].strip()
					if cmd == "module":
						module = ""
						command = CMD_MODULE
					elif cmd == "function":
						function = ""
						command = CMD_FUNCTION
					elif cmd == "description":
						command = CMD_DESCRIPTION
					elif cmd == "arguments":
						command = CMD_ARGUMENTS
					elif cmd == "returns":
						command = CMD_RETURNS
					elif cmd == "":
						pass
					else:
						print("Unknown command " + cmd)
					if l == "":
						pass
					else:
						handleCmd(command, l)
		dir = os.path.abspath(args.outputdir)
		mods.write(dir)

if __name__ == '__main__':
	main()
