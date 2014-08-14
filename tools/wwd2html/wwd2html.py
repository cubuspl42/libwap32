import argparse
import mako
from mako.template import Template
from os import path
import pywap32
import sys

class _Layer:
    pass

class _Tile:
    pass

class _Object:
    pass

def render(wwd, image_sets):
    tiles_dir = wwd.image_dir.replace('\\', '_')
    if tiles_dir.startswith('_'):
        tiles_dir = tiles_dir[1:]
    wwd_image_sets = [image_set.replace('\\', '_') for image_set in wwd.image_sets]

    layers = []
    objects = []

    for plane in wwd.planes:
        if plane.movement_x_percent == 100 and plane.movement_x_percent == 100:
            layer = _Layer()
            layer.image_set_id = tiles_dir + '_' + plane.image_sets[0]
            layer.tiles = []
            layer.tile_width, layer.tile_height = plane.tile_width, plane.tile_height
            layer.z_index = plane.z_coord
            layer.fill_color = plane.fill_color
            w, h = plane.size
            for x in range(w):
                for y in range(h):
                    tile_index = plane.get_tile((x, y))
                    tile = _Tile()
                    tile.x = x
                    tile.y = y
                    if tile_index == 0xFFFFFFFF:
                        continue
                    elif tile_index == 0xEEEEEEEE:
                        tile.filled = True
                    else:
                        tile.filled = False
                        tile.index = tile_index
                    layer.tiles.append(tile)
            layers.append(layer)
        for obj in plane.objects:
            object = _Object()
            for i, prefix in enumerate(wwd.prefixes):
                if prefix and obj.image_set.startswith(prefix):
                    object.image_set_id = obj.image_set.replace(prefix, wwd_image_sets[i])
                    break
            else:
                object.image_set_id = tiles_dir + '_' + obj.image_set
            object.x, object.y = obj.x, obj.y
            object.z_index = obj.z
            object.i = obj.i
            object.mirrored = obj.draw_flags & pywap32.Object.DRAW_FLAG_MIRROR
            object.inverted = obj.draw_flags & pywap32.Object.DRAW_FLAG_INVERT
            objects.append(object)
    template = Template(filename='index.mako')
    return template.render(layers=layers, objects=objects, image_sets=image_sets)

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-o', metavar='<file>', help='Write output to <file>')
    parser.add_argument('input', help='WWD file to process')
    args = parser.parse_args()

    out_filename = args.o or path.splitext(args.input)[0] + ".html"
    with open(out_filename, 'w') as out_file:
        wwd = pywap32.Wwd.open(args.input)
        with open('image_sets.dict') as image_sets_dict:
            image_sets = eval(image_sets_dict.read())
        try:
            rendered = render(wwd, image_sets)
        except:
            rendered = mako.exceptions.html_error_template().render().decode()
        out_file.write(rendered)
