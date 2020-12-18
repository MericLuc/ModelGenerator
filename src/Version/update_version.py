#!/usr/bin/env python
#coding=utf-8
#######################################################################
# @file update_version.py
# @brief Script permettant de mettre à jour le numéro de version 
#  		 en utilisant svnversion
# Utilisation d'un script python plutôt que sh pour la portabilité sous
# windows.
#Ce script est écrit pour python3.
#@author lhm
#######################################################################

import sys
import subprocess
import getopt

file_template = """#ifndef %(name)s_H
#define %(name)s_H
/*! \\file 
 * Ce fichier contient les informations de version du programme.
 * Il est mis à jour automatiquement par un script.
 */
 
/*!
 * Macro définissant le numéro de version courant à partir d'un 
 * numéro de version et de la révision du projet.
 */
#define %(name)s "%(version)s-%(revision)s"

#endif // %(name)s_H
"""

def display_usage():
	print("""Usage: update_version.py [options] file_name
	Options:
	-h, --help: displays this help message
	-s path, --svnversion=path: use path for the svnversion executable
	-n name: name of the version variable
	-V version: the version to set (not including the revision)
	-t file: the file template to use.
	""")

def get_revision(svnversion):
	p = subprocess.Popen([svnversion, '-c'], stdout=subprocess.PIPE)
	p.wait()
	output = p.communicate()[0]
	
	parts = output.decode().split(":")
	version = parts[len(parts)-1].strip()

	return version.encode()
	
def write_version_file(svnversion, template, var_name, version, file_name):
	revision = get_revision(svnversion)
	new_content = template % {"version": version, "revision": revision, "name": var_name} 
	old_content = ""

	try:
		old_file = open(file_name, "r")
		old_content = old_file.read()
	except:
		pass

	if old_content != new_content:
		output = open(file_name, "w")
		output.write(new_content)
		output.close()

if __name__ == '__main__':
	svnversion = "svnversion"
	try:
		opts, args = getopt.getopt(sys.argv[1:], "hs:n:V:t:", [ "help", "svnversion="])
	except (getopt.GetoptError, err):
		print(str(err))
		display_usage()
		sys.exit(1)
	
	version = "1.0"
	var_name = "VERSION"
	template_filename = None
	for o,a in opts:
		if o in ("-h", "--help"):
			display_usage()
			sys.exit()
		elif o in ("-s", "--svnversion"):
			svnversion = a
		elif o == "-n":
			var_name = a
		elif o == "-V":
			version = a
		elif o == "-t":
			template_filename = a
		else:
			display_usage()
			sys.exit(1)
	
	if template_filename != None:
		# Load the file template
		f = open(template_filename, "r")
		template = f.read()
		f.close()
	else:
		template = file_template
	
	for file_name in args:
		write_version_file(svnversion, template, var_name, version, file_name)
	
