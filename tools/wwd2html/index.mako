<%
tiles_dir = wwd.image_dir.replace('\\', '/')
image_sets = [image_set.replace('\\', '_') for image_set in wwd.image_sets]
image_set_map = eval(open('image_set_map.dict').read())
%>\
<html>
<style>
div {
	position: absolute;
}

.plane {
	width: ${plane.tiles_wide*plane.tile_width}px;
}

.plane .tile {
	width: ${plane.tile_width}px;
	height: ${plane.tile_height}px;
}

.object {
	width: auto;
	height: auto;
}

.object img {
	-webkit-transform: translate(-50%, -50%);
	   -moz-transform: translate(-50%, -50%);
        -ms-transform: translate(-50%, -50%);
	     -o-transform: translate(-50%, -50%);
	        transform: translate(-50%, -50%);
}

% for tile in sorted(set(plane.tiles)):
.tile_${tile} { background-image: url(CLAW/${tiles_dir}/ACTION/${'%03d' % tile}.png) }
% endfor

</style>
<body>
<div class="plane">
<div class="tiles">
% for y in range(plane.tiles_high):
% for x in range(plane.tiles_wide):
<%
tile = plane.tiles[y*plane.tiles_wide+x]
tw = plane.tile_width
th = plane.tile_height
%>\
% if tile != 0xFFFFFFFF:
<div style="left:${x*tw}px; top:${y*th}px;" class="tile tile_${tile}"></div>
% endif
% endfor
% endfor
</div>
<div class="objects">
% for obj in plane.objects:
<%
full_image_set = None
for i, prefix in enumerate(wwd.prefixes):
	if obj.image_set.startswith(prefix):
		full_image_set = obj.image_set.replace(prefix, image_sets[i])
		break
%>\
<div style="left:${obj.x}px; top:${obj.y}px;" class="object">
<img src="${image_set_map[full_image_set]}">
</div>
% endfor
</div>
</div>
</body>
</html>