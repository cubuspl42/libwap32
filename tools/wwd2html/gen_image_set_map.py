import os
from os import path
from pprint import pprint
import struct
import sys

image_set_map = {}
unpacked_rez_path = sys.argv[1]
for root, dirs, files in os.walk(unpacked_rez_path):
    if not dirs and files:
        first_file = files[0]
        if first_file.endswith(".PID"):
            offset = None
            file_path = path.join(root, first_file)
            with open(file_path, 'rb') as pid_file:
                pid_file.read(16)
                offset = struct.unpack("<ii", pid_file.read(8))
            key = path.relpath(root, unpacked_rez_path).replace(os.sep, '_')
            rel_file_path = path.relpath(file_path, unpacked_rez_path)
            value = (rel_file_path.replace(os.sep, '/').replace('.PID', ''), offset)
            print(key, value)
            image_set_map[key] = value

file = open('image_set_map.dict', 'w')
pprint(image_set_map, file)