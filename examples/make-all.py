#!/usr/bin/python

import sys
import glob
import re

entrypoints = []
anchors = []

remove_main = re.compile("int main.+\}", re.DOTALL)

for cpp in glob.glob('examples/*.cpp'):
    if cpp != 'examples/all.cpp':
        sys.stdout.write(remove_main.sub("", open(cpp).read()))
        low = cpp.split('/')[-1].split('.')[0] # name without path and extension
        Cap = re.search(r"create([^\s]+)App", open(cpp).read()).groups()[0]
        args = {'low': low, 'Cap': Cap}
        entrypoints.append('''
        addEntryPoint(Wt::Application, create%(Cap)sApp, "/%(low)s");
        ''' % args)
        anchors.append('''
        new WAnchor("%(low)s", "%(Cap)s", root());
        new WBreak(root());
        ''' % args)
sys.stdout.write(open('examples/all.cpp.in').read() %
    {'entrypoints': ''.join(entrypoints), 'anchors': ''.join(anchors)})

