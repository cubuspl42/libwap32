import argparse
from mako.template import Template
import pywap32
import sys

parser = argparse.ArgumentParser()
parser.add_argument('-o', metavar='<file>', help='Write output to <file> instead of stdout')
parser.add_argument('input', help='WWD file to process')
args = parser.parse_args()

wwd = pywap32.Wwd.open(args.input)
action = wwd.planes[1]

template = Template(filename='index.mako')
file = args.o and open(args.o, 'w') or sys.stdout
file.write(template.render(wwd=wwd, plane=action))
file is sys.stdout or file.close()