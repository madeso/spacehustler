#!/usr/bin/python

# Todo:
# - generate a alphabetical list of all non type related function
# - sort the function list
# - validate the types

"""Generates a list of lua markup documents based on c++ code that generates it.
"""

from __future__ import print_function
import argparse
import sys
import os

CMD_NONE = 0
CMD_MODULE = 1
CMD_FUNCTION = 2
CMD_DESCRIPTION = 3
CMD_ARGUMENTS = 4
CMD_RETURNS = 5

def firstbold(mods, s):
	index = s.find(' ')
	if index == -1:
		return "**" + s + "**"
	else:
		type = s[0:index]
		doc = s[index:]
		if mods.hastype(type):
			return "[" + type + "](type_" + type + ".md)"+ doc
		else:
			return "**" + type + "**" + doc

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
	def write(self, mods, modulename, truemodule, target):
		print("", file=target)
		name = self.name
		if self.isOperator:
			name = "Operator " + name
		else:
			name = name + "()"
		if truemodule:
			print(modulename + "." + name, file=target)
		else:
			print(name, file=target)
		print("-----------", file=target)
		print("", file=target)
		print(self.doc, file=target)
		print("", file=target)
		
		if len(self.arg) > 0:
			print("###Arguments:", file=target)
			for a in self.arg:
				print("  - " + firstbold(mods, a), file=target)
			print("", file=target)
		
		if len(self.ret) > 0:
			print("####Returns:", file=target)
			for r in self.ret:
				print("  - " + firstbold(mods, r), file=target)
			print("", file=target)
		
		print("", file=target)

class Mod:
	def __init__(self):
		self.funcs = []
		self.truemodule = True
	def addfunc(self, func):
		self.funcs.append(func)
	def write(self, mods, modulename, target):
		for file in self.funcs:
			file.write(mods, modulename, self.truemodule, target)

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
	def hastype(self, name):
		return name in self.mods
	def write(self, dir):
		for name, mod in self.mods.iteritems():
			prefix = "module"
			if mod.truemodule == False:
				prefix = "type"
			filename = os.path.join(dir, prefix+"_" + name+'.md')
			with open(filename, 'w') as target:
				if mod.truemodule:
					print("Module: " + name, file=target)
				else:
					print("Type: " + name, file=target)
				print("===========", file=target)
				print("", file=target)
				print("[Index](index.md)", file=target)
				print("", file=target)
				mod.write(self, name, target)
		modulesfilename = os.path.join(dir, "modules.md")
		with open(modulesfilename, 'w') as modulesfile:
			print("All functions", file=modulesfile)
			print("===========", file=modulesfile)
			print("", file=modulesfile)
			print("[Index](index.md)", file=modulesfile)
			for name, mod in self.mods.iteritems():
				if mod.truemodule:
					mod.write(self, name, modulesfile)
		indexfilename = os.path.join(dir, "index.md")
		with open(indexfilename, 'w') as indexfile:
			print(os.path.basename(os.getcwd()) + " documentation", file=indexfile)
			print("========", file=indexfile)
			print("", file=indexfile)
			print("Modules:", file=indexfile)
			print("--------", file=indexfile)
			print("[All functions](modules.md)", file=indexfile)
			print("", file=indexfile)
			for name, mod in self.mods.iteritems():
				if mod.truemodule:
					print("* [" + name + " module](module_" + name + ".md)", file=indexfile)
			print("", file=indexfile)
			print("Type:", file=indexfile)
			print("--------", file=indexfile)
			print("", file=indexfile)
			for name, mod in self.mods.iteritems():
				if mod.truemodule==False:
					print("* [" + name + " module](type_" + name + ".md)", file=indexfile)

module = ""
function = ""
createnewfunction = True
mods = Mods()
func = None
truemodule = True
isoperator = False

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
	global truemodule
	global isoperator
	
	if c == CMD_MODULE:
		module = module + line
	elif c == CMD_FUNCTION:
		if len(line) > 0:
			function = function + line
			createnewfunction = True
	else:
		if c == CMD_DESCRIPTION:
			if createnewfunction:
				func = Func(function)
				func.isOperator = isoperator
				m = mods.mod(module)
				m.truemodule = truemodule
				m.addfunc(func)
			createnewfunction = False
		else:
			createnewfunction = True
		addToFunc(func, c, line)
def main():
	global module
	global function
	global mods
	global truemodule
	global isoperator
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
						truemodule = True
						command = CMD_MODULE
					elif cmd == "type":
						module = ""
						truemodule = False
						command = CMD_MODULE
					elif cmd == "function":
						function = ""
						isoperator = False
						command = CMD_FUNCTION
					elif cmd == "operator":
						function = ""
						isoperator = True
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
		for file in os.listdir(dir):
			path = os.path.join(dir, file)
			try:
				if os.path.isfile(path):
					os.unlink(path)
			except Exception, e:
				print(e)
		mods.write(dir)

if __name__ == '__main__':
	main()
