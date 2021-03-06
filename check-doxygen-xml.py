#!/usr/bin/python
import xml.etree.ElementTree as ET
import re
import argparse

reFileName = re.compile('^[a-z][a-z-_0-9]*\\.((cc)|(h))$')
messageFileName = 'Filenames should be all lowercase and can include underscores (_) or dashes (-). Follow the convention that your project uses. If there is no consistent local pattern to follow, prefer "_".'

reNamespace = re.compile('^([a-z][a-z_]*::)*[a-z][a-z_]*[0-9]*$')
messageNamespace = 'Namespace names are all lower-case, and based on project names and possibly their directory structure: google_awesome_project.'
# anonymous namespace apperently starts with @and a number
reAnonNamespace = re.compile('^([a-z][a-z_]*::)*@[0-9]+$')

reTypeName = re.compile('^([^:]+::)*[A-Z][a-zA-Z]*[0-9]*$')
# the namespace check here includes invalid namespaces too, that is catched by the more extensive check designed
# for namespaces this is only here to allow namespaces in names as doxgen export the fully qualified name
messageTypeName = 'Type names start with a capital letter and have a capital letter for each new word, with no underscores: MyExcitingClass, MyExcitingEnum.'

reConst = re.compile('^k[A-Z][a-zA-Z0-9]*$')
messageConst = 'Use a k followed by mixed case: kDaysInAWeek.'

reMacro = re.compile('^[A-Z][A-Z_0-9]*$')
messageMacro = 'If they are absolutely needed, then they should be named with all capitals and underscores. Like this: MY_MACRO_THAT_SCARES_SMALL_CHILDREN.'

reDataMember = re.compile('^[a-z][a-z_0-9]*$')
messageDataMember = 'Data members (also called instance variables or member variables) are lowercase with optional underscores like regular variable names, but always end with a trailing underscore.'

reFunction = re.compile('^[A-Z][a-zA-Z0-9]*$')
messageFunction = 'Regular functions have mixed case; accessors and mutators match the name of the variable: MyExcitingFunction(), MyExcitingMethod(), my_exciting_member_variable(), set_my_exciting_member_variable().'

messageEnumValue = 'Enumerators should be named either like constants or like macros: either kEnumName or ENUM_NAME.'

class ErrFile:
	def __init__(self, file, line):
		self.file = file
		self.line = line
	def message(self):
		return str(self.file) + '(' + str(self.line) + ')'
NOFILE = ErrFile('nofilefound', -1)

errorstoignore = []
ignoreNoFiles = False
docxml = 'doc/xml/'

def geterrorfile(cref, memref):
	global docxml
	tree = ET.parse(docxml +cref+ '.xml')
	root = tree.getroot()
	parent_map = dict((c, p) for p in tree.getiterator() for c in p)
	if memref is None:
		for d in root.findall('compounddef'):
			if d.get('id') == cref:
				location = d.find('location')
				if location != None:
					file = location.get('file')
					line = location.get('line')
					return ErrFile(file, line)
		return NOFILE
	for d in root.iter('memberdef'):
		id = d.get('id')
		if id == memref:
			location = d.find('location')
			if location != None:
				file = location.get('file')
				line = location.get('line')
				return ErrFile(file, line)
	for d in root.iter('enumvalue'):
		id = d.get('id')
		if id == memref:
			location = parent_map[d].find('location')
			if location != None:
				file = location.get('file')
				line = location.get('line')
				return ErrFile(file, line)
	return NOFILE

errorcount = 0

errormessages = set([])

def GetFunctionArgs(cref, memref):
	global docxml
	tree = ET.parse(docxml +cref+ '.xml')
	root = tree.getroot()
	for d in root.iter('memberdef'):
		id = d.get('id')
		if id == memref:
			args = d.find('argsstring')
			if args != None:
				return args.text
	return ''
def StrOrEmpty(e):
	if e is None:
		return ''
	else:
		return e
def GetFunctionReturn(cref, memref):
	global docxml
	tree = ET.parse(docxml +cref+ '.xml')
	root = tree.getroot()
	for d in root.iter('memberdef'):
		id = d.get('id')
		if id == memref:
			type = d.find('type')
			if type is not None:
				ref = type.find('ref')
				if ref is not None:
					return StrOrEmpty(ref.text) + StrOrEmpty(type.text)
				else:
					return StrOrEmpty(type.text)
	return ''

def getLine(file):
	if file.file is None:
		return ''
	if file.line is None:
		return ''
	with open(file.file) as f:
		linenumber = 1
		fline = int(file.line)
		for line in f:
			if linenumber == fline:
				return line.strip()
			linenumber = linenumber + 1
	return ''

def hasSourceErrorIgnore(file):
	line = getLine(file)
	return 'no-doxygen-namechecks' in line
	
def errprint(file, name, id, error, desc):
	global errorcount
	global errorstoignore
	global ignoreNoFiles
	global errormessages
	
	if id in errorstoignore:
		return
	if file == NOFILE and ignoreNoFiles:
		return
	error = file.message() + ": Error N" + str(id) + ": " + error + ": [" + name + "]"
	if hasSourceErrorIgnore(file):
		return
	# if the error meeage already has been displayed, don't display it again
	if error in errormessages:
		pass
	else:
		print error
		errormessages.add(error)
		errorcount += 1

# If you are naming something that is analogous to an existing C or C++ entity then you can follow the existing naming convention scheme.
# uint typedef
def IsBaseType(name):
	name = name.rstrip('0123456789')
	if name.endswith('int'):
		return True
	if name.endswith('float'):
		return True
	if name.endswith('double'):
		return True
	return False
	
def logic():
	global docxml
	global errorcount
	allowedfunctions = ['main']
	errorcount = 0
	tree = ET.parse(docxml + 'index.xml')
	root = tree.getroot()
	for compound in root.findall('compound'):
		name = compound.find('name').text
		kind = compound.get('kind')
		cref = compound.get('refid')
		isclass = False
		if kind == 'class' or kind == 'interface' or kind == 'struct' or kind =='exception':
			isclass = True
			if reTypeName.match(name) == None:
				file = geterrorfile(cref, None)
				errprint(file, name, 4201, "invalid type name for " + kind, messageTypeName)
		elif kind == 'file':
			if reFileName.match(name) == None:
				if name.endswith('.md') == False:
					# ignore markdown files
					file = geterrorfile(cref, None)
					errprint(file, name, 4202, "invalid file name", messageFileName)
		elif kind == 'namespace':
			if reNamespace.match(name) == None:
				if reAnonNamespace.match(name) == None:
					file = geterrorfile(cref, None)
					errprint(file, name, 4203, "invalid namespace name", messageNamespace)
		elif kind == 'union':
			pass
		elif kind == 'protocol':
			pass
		elif kind == 'category':
			pass
		elif kind == 'group':
			pass
		elif kind == 'page':
			pass
		elif kind == 'example':
			pass
		elif kind == 'dir':
			pass
		else:
			print 'unknown compund kind: ' + kind
		
		memfunctions = []
		if isclass:
			for member in compound.findall('member'):
				memberkind = member.get('kind')
				membername = member.find('name').text
				if memberkind == 'variable':
					name = membername
					# remove trailing _ if it exists
					if name.endswith('_'):
						name = name[:len(name)-1]
					memfunctions.append(name)
					memfunctions.append('set_' + name)
		for member in compound.findall('member'):
			memberkind = member.get('kind')
			membername = member.find('name').text
			memref = member.get('refid')
			
			if memberkind == 'define':
				if reMacro.match(membername) == None:
					file = geterrorfile(cref, memref)
					errprint(file, membername, 4204, "invalid macro name", messageMacro)
			elif memberkind == 'property':
				pass
			elif memberkind == 'event':
				pass
			elif memberkind == 'variable':
				if isclass:
					endswithunderscore = membername.endswith('_')
					isstruct = kind == 'struct'
					if reDataMember.match(membername) == None or isstruct == endswithunderscore:
						file = geterrorfile(cref, memref)
						errprint(file, membername, 4205, "invalid data member name", messageDataMember)
			elif memberkind == 'typedef' or memberkind == 'enum':
				if reTypeName.match(membername) == None:
					if IsBaseType(membername) == False:
						file = geterrorfile(cref, memref)
						errprint(file, membername, 4201, "invalid type name for " + memberkind, messageTypeName)
			elif memberkind == 'enumvalue':
				if reConst.match(membername) == None and reMacro.match(membername) == None:
					file = geterrorfile(cref, memref)
					errprint(file, membername, 4206, "invalid enumvalue", messageEnumValue)
			elif memberkind == 'function':
				if reFunction.match(membername) == None:
					# not a valid function, check for acessor/mutator match instead
					if membername in memfunctions:
						pass
					else:
						funcargs = GetFunctionArgs(cref, memref)
						funcret = GetFunctionReturn(cref, memref)
						invalidname = True
						# allow various c++ functions too!
						if membername.startswith('operator'):
							invalidname = False
						if membername.endswith('operator'):
							invalidname = False
						if membername.startswith('~'):
							invalidname = False
						if membername in allowedfunctions:
							invalidname = False
						if funcargs.endswith('override'):
							#ignore invalid functions marked as override as they implement external interfaces
							# they might also implement internal interfaces but they are already checked 
							invalidname = False
						if funcret == '':
							# if the function doesn't return anything, it probably isn't a function but
							# something that looks like a function, say a macro call
							invalidname = False
						if invalidname:
							file = geterrorfile(cref, memref)
							errprint(file, membername, 4207, "invalid functionname", messageFunction)
			elif memberkind == 'signal':
				pass
			elif memberkind == 'prototype':
				pass
			elif memberkind == 'friend':
				pass
			elif memberkind == 'dcop':
				pass
			elif memberkind == 'slot':
				pass
			else:
				print 'Unknown member kind ' + memberkind
	print "Errors found: ", errorcount

def main():
	global errorstoignore
	global ignoreNoFiles
	global docxml
	parser = argparse.ArgumentParser(description='Check doxygen XML output for google c++ style errors.')
	parser.add_argument('--ignore', metavar='ERROR', type=int, nargs='+', help='errors to ignore')
	parser.add_argument('--xml', help='the path to the doxygen xml folder (default: ' + docxml + ')')
	parser.add_argument('--ignoremissing', action='store_const', const=True, default=False, help='Ignore errors where there is no file origin (default: ' + str(ignoreNoFiles) +')')
	
	args = parser.parse_args()
	if args.ignore is None:
		pass
	else:
		errorstoignore = args.ignore
	if args.xml is None:
		pass
	else:
		docxml = args.xml
	ignoreNoFiles = args.ignoremissing
	logic()
main()