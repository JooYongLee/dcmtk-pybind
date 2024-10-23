import sys
import os

file_path = os.path.dirname(os.path.abspath(__file__))
ext_dlls = [
    os.path.join(file_path, 'dlls')
]


"""
Add a path to the DLL search path.

This search path is used when resolving dependencies for imported extension modules (the module itself is resolved through sys.path), and also by ctypes.

Remove the directory by calling close() on the returned object or using it in a with statement.

See the Microsoft documentation for more information about how DLLs are loaded.

Raises an auditing event os.add_dll_directory with argument path.

Availability: Windows.
"""
# https://stackoverflow.com/questions/214852/python-module-dlls
# https://docs.python.org/3/library/os.html#os.add_dll_directory
if sys.version_info.minor < 8:
    for p in ext_dlls:
        assert os.path.exists(p), p
        os.environ["PATH"] += os.pathsep + p + ";"
else:
    for p in ext_dlls:
        assert os.path.exists(p), p
        os.add_dll_directory(p)
