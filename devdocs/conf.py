# Configuration file for the Sphinx documentation builder.
#
# This file contains specialized commands to generated source code API docuemtation
# with Doxygen in a manner that the resulting HTML files can be published on Read-the-Docs

# -- Before Sphinx runs (RAB) -----------------------------------------------
# This is custom code to generate API docuementation using Doxygen
# and make it part of the overall documentation set.
#
# The output location specifed in Doxyfile is ./API/
# This is important. The html_extra_path command at the end of this file
# depends on the Doxygen output being in ./API/html
import subprocess
subprocess.call('doxygen', shell=True)


# -- Project information -----------------------------------------------------

project = 'BEToolbox Extensions Developer Documentation'

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

# The files in this location are copied into the RTD output location
# This is the location of the Doxygen files. This configuration setting will
# replace the RTD generated files with the Doxygen files
html_extra_path = ['./API/html']

