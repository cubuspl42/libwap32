from pprint import pprint
import os
from os import path

image_set_map = {}
rez = 'CLAW'
for root, dirs, files in os.walk(rez):
    for ignored in ('INTERFACE', 'STATES'):
        if ignored in dirs:
            dirs.remove(ignored)
    if not len(dirs) and len(files):
        key = root.replace('/', '_').replace(rez + '_', '')
        value = path.join(root, files[0])
        print(key, value)
        image_set_map[key] = value

file = open('image_set_map.dict', 'w')
pprint(image_set_map, file)