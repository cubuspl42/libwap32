import os
from os import path
from pprint import pprint
import re
import struct
import sys

assert len(sys.argv) == 2

image_sets = {}
unpacked_rez_path = sys.argv[1]
for root, dirs, files in os.walk(unpacked_rez_path):
    if not dirs and files:
        pids = [pid_filename for pid_filename in files if pid_filename.endswith(".PID")]
        if pids:
            key = path.relpath(root, unpacked_rez_path).replace(os.sep, '_') # example: LEVEL3_IMAGES_CRATE
            image_set = dict()
            image_sets[key] = image_set
            for i, pid_filename in enumerate(pids):
                try:
                    index = int(re.search(r'\d+', pid_filename).group(0))
                except:
                    if i != 0:
                        continue
                    index = None
                pid_path = path.join(root, pid_filename)
                rel_path = path.relpath(pid_path, unpacked_rez_path)
                with open(pid_path, 'rb') as pid_file:
                    pid_file.read(8)
                    size = struct.unpack("<ii", pid_file.read(8))
                    offset = struct.unpack("<ii", pid_file.read(8))
                cut_rel_path = rel_path.replace(os.sep, '/').replace('.PID', '')
                value = (cut_rel_path, size, offset)
                print(key, index, value)
                if index is not None:
                    image_set[index] = value
                if i == 0:
                    image_set[-1] = value

file = open('image_sets.dict', 'w')
pprint(image_sets, file)