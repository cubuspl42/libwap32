from cffi import FFI
import re

_libwap32_headers = ['../../include/wap32/wwd.h', '../../include/wap32/errors.h']
_ffi = FFI()

def _sub_lshifts(s):
    return re.sub(r'(\d+) << (\d+)', lambda m: str(int(m.group(1)) << int(m.group(2))), s)

for header in _libwap32_headers:
    with open(header, 'r') as header_file:
        header_content = header_file.read()
        header_content = re.search(r'WAP32_BEGIN_DECLS\n(.*?)WAP32_END_DECLS\n', header_content, re.DOTALL).group(1)
        header_content = header_content.replace('WAP32_API ', '')
        header_content = _sub_lshifts(header_content)
        _ffi.cdef(header_content)

_libwap32 = _ffi.dlopen('../../build/Debug/libwap32.dylib')

_exception_map = {
    _libwap32.WAP32_ENOMEMORY:      MemoryError,
    _libwap32.WAP32_EFILE:          OSError,
    _libwap32.WAP32_EINVALIDDATA:   ValueError,
}

def _handle_error(error_code):
    if error_code < 0:
        error_str = _ffi.string(_ffi.cast("Wap32Error", error_code))
        try:
            ExceptionClass = _exception_map[error_code]
            raise ExceptionClass(error_str)
        except KeyError:
            raise Exception(error_str)

def _string(*args):
    return _ffi.string(*args).decode('utf-8')

def _unpack_cdata(cdata):
    try:
        ctype = _ffi.typeof(cdata)
    except TypeError:
        return cdata
    if ctype.kind == 'pointer' and ctype.item.cname == 'char':
        return _string(cdata)
    elif ctype.kind == 'array':
        length = ctype.length
        if ctype.item.cname == 'char':
            return _string(cdata, length)
        else:
            array = []
            for i in range(length):
                array.append(_unpack_cdata(cdata[i]))
            return array
    else:
        return None

def _unpack_cdata_struct(obj, cdata_ptr):
    ctype = _ffi.typeof(cdata_ptr[0])
    for field_name, field in ctype.fields:
        field_value = getattr(cdata_ptr, field_name)
        setattr(obj, field_name, _unpack_cdata(field_value))

def _unpack_property(prefix, name, cdata_ptr):
    getter = getattr(_libwap32, 'wap32_' + prefix + '_get_' + name)
    cdata = getter(cdata_ptr)
    return _unpack_cdata(cdata)

class Wwd:
    @staticmethod
    def open(filename):
        wwd = Wwd()
        wwd_ptr_out = _ffi.new("Wap32Wwd *[1]")
        _handle_error(_libwap32.wap32_wwd_open(wwd_ptr_out, filename.encode('utf-8')))
        wwd_ptr = wwd_ptr_out[0]
        assert wwd_ptr
        _unpack_cdata_struct(wwd, _libwap32.wap32_wwd_get_properties(wwd_ptr))
        
        wwd.planes = []
        for i in range(_libwap32.wap32_wwd_get_plane_count(wwd_ptr)):
            plane = Plane()
            wwd.planes.append(plane)
            
            plane_ptr = _libwap32.wap32_wwd_get_plane(wwd_ptr, i)
            _unpack_cdata_struct(plane, _libwap32.wap32_plane_get_properties(plane_ptr))
            
            plane.tiles = []
            for j in range(_libwap32.wap32_plane_get_tile_count(plane_ptr)):
                tile_id = _libwap32.wap32_plane_get_tile(plane_ptr, j)
                plane.tiles.append(tile_id)
            
            plane.image_sets = []
            for j in range(_libwap32.wap32_plane_get_image_set_count(plane_ptr)):
                image_set = _unpack_cdata(_libwap32.wap32_plane_get_image_set(plane_ptr, j))
                plane.image_sets.append(image_set)
            
            plane.objects = []
            for j in range(_libwap32.wap32_plane_get_object_count(plane_ptr)):
                obj = Object()
                plane.objects.append(obj)

                obj_ptr = _libwap32.wap32_plane_get_object(plane_ptr, j)
                _unpack_cdata_struct(obj, _libwap32.wap32_object_get_properties(obj_ptr))
                obj.name = _unpack_property('object', 'name', obj_ptr)
                obj.logic = _unpack_property('object', 'logic', obj_ptr)
                obj.image_set = _unpack_property('object', 'image_set', obj_ptr)
                obj.animation = _unpack_property('object', 'animation', obj_ptr)
    
        wwd.tile_descriptions = []
        for i in range(_libwap32.wap32_wwd_get_tile_description_count(wwd_ptr)):
            desc = TileDescription()
            wwd.tile_descriptions.append(desc)
        
            desc_ptr = _libwap32.wap32_wwd_get_tile_description(wwd_ptr, i)
            _unpack_cdata_struct(desc, desc_ptr)
        
        return wwd
    def __repr__(self):
        return 'WWD "%s"' % self.level_name

class Plane:
    pass

class Object:
    pass

class TileDescription:
    pass

if __name__ == "__main__":
    wwd = Wwd.open('RETAIL01.WWD')
    action = wwd.planes[1]
    print(wwd.planes[1].objects[1234].logic)
