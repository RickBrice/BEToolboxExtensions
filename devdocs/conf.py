# Configuration file for the Sphinx documentation builder.
#
# This file only contains a selection of the most common options. For a full
# list see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Before Sphinx runs (RAB) -----------------------------------------------
# This is custom code to generate API docuementation using Doxygen
# and make it part of the overall documentation set.
#
# Before Sphinx runs, generate API documentation with Doxygen
import subprocess, os

# def configureDoxyfile(input_dir,output_dir):
    # with open('Doxyfile','r') as file :
       # filedata = file.read()
	  
    # filedata = filedata.replace('@DOXYGEN_INPUT_DIR@', input_dir)
    # filedata = filedata.replace('@DOXYGEN_OUTPUT_DIR@', output_dir)
    	  
    # with open('Doxyfile','w') as file :
        # file.write(filedata)

# # Check if we are running on Read the Docs' servers
# read_the_docs_build = os.environ.get('READTHEDOCS', None) == 'True'

# input_dir = '../'
# output_dir = '_build'

# if read_the_docs_build:
   # input_dir = '../'
   # output_dir = 'build'
   
   
# configureDoxyfile(input_dir,output_dir)
subprocess.call('doxygen', shell=True)


# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
# import os
# import sys
# sys.path.insert(0, os.path.abspath('.'))


# -- Project information -----------------------------------------------------

project = 'BEToolbox Extensions Developer Documentation'
copyright = '2020, Richard Brice, PE'
author = 'Richard Brice, PE'


# -- General configuration ---------------------------------------------------

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [
]

# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']


# -- Options for HTML output -------------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#
html_theme = 'alabaster'

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
#html_static_path = ['_static']

html_extra_path = ['./API/html']

