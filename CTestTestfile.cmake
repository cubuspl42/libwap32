# CMake generated Testfile for 
# Source directory: D:/Projects/WAP64/libwap32
# Build directory: D:/Projects/WAP64/libwap32
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(wwd "D:/Projects/WAP64/libwap32/tests/test_wwd.exe" "RETAIL01_nocompress.WWD")
set_tests_properties(wwd PROPERTIES  WORKING_DIRECTORY "D:/Projects/WAP64/tests/wwd_files")
subdirs(tests)
