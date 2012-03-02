# wt-classes, utility classes used by Wt applications
# Copyright (C) 2011 Boris Nagaev
#
# See the LICENSE file for terms of use.

from distutils.core import setup
import os.path as p
import os

version_file = p.join(os.pardir, p.dirname(__file__), 'VERSION')

setup(name='locales-test',
    version=open(version_file).read().strip(),
    description='Test of xml-based localizations used by Wt apps',
    author='Boris Nagaev',
    author_email='bnagaev@gmail.com',
    url='https://bitbucket.org/starius/wt-classes/',
    license='GNU GPL v2',
    scripts=['locales-test'],
)

