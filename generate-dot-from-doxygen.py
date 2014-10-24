#!/usr/bin/python

import xml.etree.ElementTree as ET
import re
import argparse

docxml = 'doc/xml/'

def cleanid(x):
	return x.replace('-', '__')

class Class:
	def __init__(self, id):
		self.id = id
		self.name = ""
		self.inherit = []
		self.refs = []
	def read(self):
		global docxml
		tree = ET.parse(docxml +self.id+ '.xml')
		root = tree.getroot()
		for d in root.iter('compounddef'):
			id = d.get('id')
			if self.id != id:
				pass
			self.name = d.find('compoundname').text
			for ie in d.iter('basecompoundref'):
				id = ie.get('refid')
				if id == None:
					id = ie.text
					print id
				self.inherit.append(id)
			for sec in d.iter('sectiondef'):
				for md in sec.iter('memberdef'):
					kind = md.get('kind')
					if kind == 'variable':
						etype = md.find('type')
						eref = etype.find('ref')
						if eref == None:
							#print self.id, etype.text
							pass
						else:
							self.refs.append(eref.get('refid'))
					elif kind == 'function' or kind == 'enum' or kind == 'typedef' or kind == 'friend':
						pass
					else:
						print 'unknown membderdef kind: ' + kind
	def write(self, f):
		f.write('  {id} [label="{name}"];\n'.format(id=cleanid(self.id), name=self.name) )
		for i in self.inherit:
			f.write('  {id} -> {i} [arrowhead="onormal"];\n'.format(id=cleanid(self.id), i=cleanid(i)) )
		for i in self.refs:
			f.write('  {id} -> {i};\n'.format(id=cleanid(self.id), i=cleanid(i)) )
		f.write('  \n')
		pass
		
def logic():
	global docxml
	tree = ET.parse(docxml + 'index.xml')
	root = tree.getroot()
	classes = []
	for compound in root.findall('compound'):
		kind = compound.get('kind')
		cref = compound.get('refid')
		isclass = False
		if kind == 'class' or kind == 'interface' or kind == 'struct' or kind =='exception':
			c = Class(cref)
			c.read()
			classes.append(c)
		elif kind == 'file':
			pass
		elif kind == 'namespace':
			pass
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
	with open("design.dot", 'w') as f:
		f.write('digraph classes {\n')
		for c in classes:
			c.write(f)
		f.write('}\n')

def main():
	global docxml
	parser = argparse.ArgumentParser(description='Check doxygen XML output for google c++ style errors.')
	parser.add_argument('--xml', help='the path to the doxygen xml folder (default: ' + docxml + ')')
	
	args = parser.parse_args()
	if args.xml is None:
		pass
	else:
		docxml = args.xml
	logic()
main()