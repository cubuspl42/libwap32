<%
	def get_image(image_set_id, index):
		try:
			return image_sets[image_set_id][index]
		except KeyError:
			return ("SADFACE", (32, 32), (0, 0))

	used_images = set()
	for obj in objects:
		used_images.add((obj.image_set_id, obj.i))
	for layer in layers:
		for tile in layer.tiles:
			if not tile.filled:
				used_images.add((layer.image_set_id, tile.index))
%>\
<style>
div {
	position: absolute;
	left: 0px;
	top: 0px;
}
% for image_set_id, index in used_images:
<%
	rez_path, size, offset = get_image(image_set_id, index)
	width, height = size
	offset_x, offset_y = offset
%>
.${image_set_id}_${index} {
	background-image: url(CLAW/${rez_path}.png);
	width: ${width}px;
	height: ${height}px;
	margin-left: ${offset_x}px;
	margin-top: ${offset_y}px;
}
% endfor
</style>

% for layer in layers:
<div style="z-index: ${layer.z_index};">
% for tile in layer.tiles:
    <div \
% if not tile.filled:
class="${layer.image_set_id}_${tile.index}" \
% endif
style="\
% if tile.filled:
background-color: #${hex(layer.fill_color)}; \
% endif
left:${tile.x*layer.tile_width}px; top:${tile.y*layer.tile_height}px"></div>
% endfor
</div>
% endfor

% for obj in objects:
<%
	
%>\
<div style="left:${obj.x}px; top:${obj.y}px; z-index:${obj.z_index}; transform:translate(-50%, -50%)\
% if obj.mirrored:
 scaleX(-1)\
% endif
% if obj.inverted:
 scaleY(-1)\
% endif
" class="${obj.image_set_id}_${obj.i}"></div>
% endfor
