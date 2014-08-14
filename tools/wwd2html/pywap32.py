from cffi import FFI
import re

_libwap32_headers = [
    '../../include/wap32/buffer.h',
    '../../include/wap32/errors.h',
    '../../include/wap32/wwd.h'
]

def _preprocess_lshifts(s): # cffi can't parse them
    return re.sub(r'(\d+) << (\d+)', lambda m: str(int(m.group(1)) << int(m.group(2))), s)

_ffi = FFI()

for header in _libwap32_headers:
    with open(header, 'r') as header_file:
        header_content = header_file.read()
        header_content = re.search(r'WAP_BEGIN_DECLS\n(.*?)WAP_END_DECLS\n', header_content, re.DOTALL).group(1)
        header_content = header_content.replace('WAP_API ', '')
        header_content = _preprocess_lshifts(header_content)
        _ffi.cdef(header_content)

_libwap32 = _ffi.dlopen('../../build/Debug/libwap32.dylib')

_exception_map = {
    _libwap32.WAP_ENOMEMORY:      MemoryError,
    _libwap32.WAP_EFILE:          OSError,
    _libwap32.WAP_EINVALIDDATA:   ValueError,
}

def _handle_error(error_code):
    if error_code < 0:
        error_str = _ffi.string(_ffi.cast("wap_error_t", error_code))
        try:
            ExceptionClass = _exception_map[error_code]
            raise ExceptionClass(error_str)
        except KeyError:
            raise Exception(error_str)

def _decode_string(str, encoding, maxlen=-1):
    bytes = _ffi.string(str, maxlen)
    if encoding is None:
        return bytes
    else:
        return bytes.decode(encoding)

def _encode_string(str, encoding):
    if encoding is None:
        return bytes(str)
    else:
        return str.encode(encoding)

def _getattr(super, obj, key):
    try:
        return getattr(obj, key)
    except TypeError:
        return obj[key]

def _setattr(super, obj, key, value):
    try:
        setattr(obj, key, value)
    except TypeError:
        obj[key] = value

def _make_accessors(ctype):
    if ctype.kind == 'array':
        length = ctype.length
        if ctype.item.cname == 'char': # fixed length string
            def get(super, obj, key):
                return _decode_string(_getattr(super, obj, key), super.encoding)
            def set(super, obj, key, value):
                encoding = super.encoding
                buffer = value
                if encoding is None:
                    if not isinstance(value, bytes):
                        raise TypeError
                else:
                    if not isinstance(value, str):
                        raise TypeError
                    buffer = _encode_string(value, super.encoding)
                if len(buffer) > length:
                    raise ValueError
                _setattr(super, obj, key, buffer)
            return (get, set)
        else: # array of primitives
            item_get, item_set = _make_accessors(ctype.item)
            def get(super, obj, key):
                array_cdata = _getattr(super, obj, key)
                return Array(super, array_cdata, item_get, item_set, lambda _: ctype.length, None)
            def set(super, obj, key, value):
                raise TypeError("arrays are not assignable")
            return (get, set)
    else:
        if ctype.cname == 'wap_rect':
            def get(super, obj, key):
                rect_cdata = _getattr(super, obj, key)
                return Rect(rect_cdata)
            def set(super, obj, key, rect):
                tpl = (rect.left, rect.top, rect.right, rect.bottom)
                _setattr(super, obj, key, tpl)
            return (get, set)
        else: # primitive property
            return (_getattr, _setattr)

def _make_ctype_property(name, ctype):
    get, set = _make_accessors(ctype)
    return property(lambda self: get(self._super, self._properties, name),
                    lambda self, value: set(self._super, self._properties, name, value))

def _make_string_property(str_get, str_set):
    def get(self):
        return _decode_string(str_get(self._cdata), self._super.encoding)
    def set(self, value):
        _handle_error(str_set(self._cdata, _encode_string(value, self._super.encoding)))
    return property(get, set)

def _make_array_property(*args):
    return property(lambda self: Array(self._super, self._cdata, *args))

def _add_properties(cls, properties_struct):
    for name, field in properties_struct.fields:
        setattr(cls, name, _make_ctype_property(name, field.type))

def _add_flags(cls, prefix, flags_enum):
    for name, value in flags_enum.relements.items():
        setattr(cls, name.replace(prefix, ''), value)

class Array:
    def __init__(self, super, cdata, get_element, set_element, get_element_count, set_element_count):
        self._super = super
        self._cdata = cdata
        self._get_element = get_element
        self._set_element = set_element
        self._get_element_count = get_element_count
        self._set_element_count = set_element_count
    def __len__(self):
        return self._get_element_count(self._cdata)
    def __getitem__(self, key):
        return self._get_element(self._super, self._cdata, self._array_index(key))
    def __setitem__(self, key, value):
        if not self._set_element:
            raise TypeError("array is read-only")
        self._set_element(self._super, self._cdata, self._array_index(key), value)
    def __repr__(self):
        return "Array([%s])" % ', '.join([repr(element) for element in self])
    def resize(self, new_size):
        if not self.set_element_count:
            raise TypeError("cannot resize static array")
        if new_size < 0:
            raise ValueError
        _handle_error(self._set_element_count(self._cdata, new_size))
    def _array_index(self, index):
        if not isinstance(index, int):
            raise TypeError
        length = len(self)
        if index < 0:
            index = length + index
        if index >= length:
            raise IndexError
        return index

class Rect:
    def __init__(self, rect_cdata):
        self._super = None
        self._cdata = rect_cdata
        self._properties = rect_cdata
    def __repr__(self):
        return "Rect(left=%r, top=%d, right=%d, bottom=%d)" % (self.left, self.top, self.right, self.bottom)

_add_properties(Rect, _ffi.typeof("wap_rect"))

class Wwd:
    def __init__(self):
        self._cdata = _libwap32.wap_wwd_create()
        if not self._cdata:
            raise MemoryError
        self._super = self
        self._properties = _libwap32.wap_wwd_get_properties(self._cdata)
    def __repr__(self):
        return 'WWD "%s"' % self.level_name
    encoding = 'ascii'
    planes = _make_array_property(lambda super, ptr, idx: Plane(super, _libwap32.wap_wwd_get_plane(ptr, idx)),
                                  None,
                                  _libwap32.wap_wwd_get_plane_count,
                                  _libwap32.wap_wwd_set_plane_count)
    tile_descriptions = _make_array_property(lambda _, ptr, idx: TileDescription(_libwap32.wap_wwd_get_tile_description(ptr, idx)),
                                             None,
                                             _libwap32.wap_wwd_get_tile_description_count,
                                             _libwap32.wap_wwd_set_tile_description_count)
    def read(self, file):
        buffer = file.read()
        _handle_error(_libwap32.wap_wwd_read(wwd._cdata, buffer, len(buffer)))
    def write(self, file):
        buffer = _ffi.new("wap_buffer")
        _libwap32.wap_buffer_init(buffer)
        _handle_error(_libwap32.wap_wwd_write(wwd._cdata, buffer))
        data, size = _libwap32.wap_buffer_data(buffer), _libwap32.wap_buffer_size(buffer)
        file.write(_ffi.buffer(data, size))
        _libwap32.wap_buffer_destroy(buffer)
    @staticmethod
    def open(filename):
        with open(filename, 'rb') as file:
            buffer = file.read()
        wwd = Wwd()
        _handle_error(_libwap32.wap_wwd_read(wwd._cdata, buffer, len(buffer)))
        return wwd
    def save(self, filename):
        with open(filename, 'wb') as file:
            self.write(file)

_add_properties(Wwd, _ffi.typeof("wap_wwd_properties"))
_add_flags(Wwd, 'WAP_WWD_FLAG_', _ffi.typeof("wap_wwd_flags_t"))

def _get_image_set(super, ptr, idx):
    return _decode_string(_libwap32.wap_plane_get_image_set(ptr, idx), super.encoding)

def _set_image_set(super, ptr, idx, value):
    _handle_error(_libwap32.wap_plane_set_image_set(ptr, idx, _encode_string(value, encoding)))

class Plane:
    def _check_map_bounds(self, xy):
        size = self.size
        x, y = xy
        if x < 0 or x >= size[0] or y < 0 or y >= size[1]:
            raise IndexError
    def __init__(self, wwd, plane_cdata):
        self._super = wwd
        self._cdata = plane_cdata
        self._properties = _libwap32.wap_plane_get_properties(self._cdata)
    def get_tile(self, xy):
        self._check_map_bounds(xy)
        return _libwap32.wap_plane_get_tile(self._cdata, xy[0], xy[1])
    def set_tile(self, xy, tile):
        self._check_map_bounds(xy)
        return _libwap32.wap_plane_set_tile(self._cdata, xy[0], xy[1], tile)
    @property
    def size(self):
        dimensions = _ffi.new("uint32_t[2]")
        _libwap32.wap_plane_get_map_dimensions(self._cdata, dimensions, dimensions+1)
        return (dimensions[0], dimensions[1])
    @size.setter
    def size(self, wh):
        _handle_error(_libwap32.wap_plane_set_map_dimensions(self._cdata, wh[0], wh[1]))
    objects = _make_array_property(lambda super, ptr, idx: Object(super, _libwap32.wap_plane_get_object(ptr, idx)),
                                  None,
                                  _libwap32.wap_plane_get_object_count,
                                  _libwap32.wap_plane_set_object_count)
    image_sets = _make_array_property(_get_image_set,
                                      _set_image_set,
                                      _libwap32.wap_plane_get_image_set_count,
                                      _libwap32.wap_plane_set_image_set_count)

_add_properties(Plane, _ffi.typeof("wap_plane_properties"))
_add_flags(Wwd, 'WAP_PLANE_FLAG_', _ffi.typeof("wap_plane_flags_t"))

class Object:
    def __init__(self, wwd, obj_cdata):
        self._super = wwd
        self._cdata = obj_cdata
        self._properties = _libwap32.wap_object_get_properties(self._cdata)
    name = _make_string_property(_libwap32.wap_object_get_name, _libwap32.wap_object_set_name)
    logic = _make_string_property(_libwap32.wap_object_get_logic, _libwap32.wap_object_set_logic)
    image_set = _make_string_property(_libwap32.wap_object_get_image_set, _libwap32.wap_object_set_image_set)
    animation = _make_string_property(_libwap32.wap_object_get_animation, _libwap32.wap_object_set_animation)

_add_properties(Object, _ffi.typeof("wap_object_properties"))
_add_flags(Object, 'WAP_OBJECT_', _ffi.typeof("wap_object_add_flags_t"))
_add_flags(Object, 'WAP_OBJECT_', _ffi.typeof("wap_object_draw_flags_t"))
_add_flags(Object, 'WAP_OBJECT_', _ffi.typeof("wap_object_dynamic_flags_t"))
_add_flags(Object, 'WAP_OBJECT_', _ffi.typeof("wap_object_user_flags_t"))
_add_flags(Object, 'WAP_OBJECT_', _ffi.typeof("wap_object_type_flags_t"))

class TileDescription:
    def __init__(self, desc_cdata):
        self._super = None
        self._cdata = desc_cdata
        self._properties = desc_cdata
    def _access_double_tile_attrs(self):
        if self.type != _libwap32.WAP_TILE_TYPE_DOUBLE:
            raise AttributeError("rect and outside_attrib are accesible iff type == WAP_TILE_TYPE_DOUBLE")
    _double_tile_attrs = ('outside_attrib', 'rect')

_add_flags(TileDescription, 'WAP_TILE_', _ffi.typeof("wap_tile_type_flags_t"))
_add_flags(TileDescription, 'WAP_TILE_', _ffi.typeof("wap_tile_attribute_flags_t"))

for name, field in _ffi.typeof("wap_tile_description").fields:
    _name = name
    if name in TileDescription._double_tile_attrs:
        _name = '_' + name
    setattr(TileDescription, _name, _make_ctype_property(name, field.type))

for name in TileDescription._double_tile_attrs:
    def get(self):
        self._access_double_tile_attrs()
        return getattr(self, '_' + name)
    def set(self, value):
        self._access_double_tile_attrs()
        setattr(self, '_' + name, value)
    setattr(TileDescription, name, property(get, set))
