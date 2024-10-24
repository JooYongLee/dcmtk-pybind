from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
import sys
import setuptools
import os
import glob
import shutil

__version__ = '0.0.1'

def deep_search_files(search_path, extesions):

    # collect binding sources
    searched_files = []

    for root, dirs, files in os.walk(search_path):

        for file in files:
            if file.lower().endswith(tuple(ext_src_filters)):
                relapath = os.path.relpath(root, file_path)
                searched_files.append(os.path.join(relapath, file))
    return searched_files


def path_to_fname(x):
    return os.path.splitext(os.path.basename(x))[0]


def filtering(targets, filters):
    outs = []
    for name in targets:
        for _check in filters:
            if name.find(_check) >= 0:
                outs.append(name)
    return outs


def collect_files(path_list, ext, only_fname=False):
        # library_dirs.extend(lib_path)
    path_list = [path_list] if isinstance(path_list, str) else path_list
    all_found = []
    for path in path_list:
        found = glob.glob(os.path.join(path, ext))
        all_found.extend(found)
    if only_fname:
        all_found = [path_to_fname(p) for p in all_found]
    return all_found


file_path = os.path.dirname(os.path.realpath(__file__))
solution_path = os.path.join(file_path, '../')
internal_lib_path = os.path.join(file_path, '../x64/Release')
external_lib_path = os.path.join(solution_path, 'libs')

extenion_source_path = os.path.join(file_path, 'src')
ext_src_filters = ['cpp', 'cxx']

public_dependency_libs = {
    'meshDicom': {
        'include': [
            os.path.join(solution_path, "meshDicom/include")
        ],  # meshlibs
        'lib': [
            internal_lib_path
            ],
        'dll': [
            internal_lib_path
        ]
    }

}

private_dependency_libs = {
    'dcmtk': (
        os.path.join(external_lib_path, 'dcmtk_x64-windows/lib'),
        os.path.join(external_lib_path, 'dcmtk_x64-windows/bin')
    )
}

# 
extension_sources = deep_search_files(extenion_source_path, ext_src_filters)
print("all cpp files:\n{}".format(extension_sources))


for k in public_dependency_libs.keys():
    print('extension modules:', k)

# dcmtk_library_path = os.path.join(external_lib_path, "dcmtk_x64-windows/lib")
# dcmtk_libs = glob.glob(dcmtk_library_path + "/*.lib")
# dcmtk_libs = [path_to_fname(path) for path in dcmtk_libs]


include_path_list = []


all_libs = []

all_dlls = []

library_dirs = []

    


for key, val in public_dependency_libs.items():
    include_path_list.extend(val.get('include', []))
    
    lib_path = val.get('lib', [])
    all_libs.extend(collect_files(lib_path, '*.lib', only_fname=True))
    library_dirs.extend(lib_path)

    dll_path = val.get('dll', [])
    all_dlls.extend(collect_files(lib_path, '*.dll', only_fname=False))
    

for key, val in private_dependency_libs.items():
    lib_path, dll_path = val
    all_dlls.extend(collect_files(dll_path, '*.dll', only_fname=False))

print('all dlls', all_dlls)
check = [os.path.exists(p) for p in include_path_list]
check_status = {os.path.exists(p): p for p in include_path_list}
print(include_path_list)
assert all(check), "conatin invalid folder:\n{}".format(check_status)


class get_pybind_include(object):
    """Helper class to determine the pybind11 include path

    The purpose of this class is to postpone importing pybind11
    until it is actually installed, so that the ``get_include()``
    method can be invoked. """

    def __init__(self, user=False):
        self.user = user

    def __str__(self):
        import pybind11
        return pybind11.get_include(self.user)


ext_modules = [
    Extension(
        'meshDicom',
        extension_sources,
        include_dirs=[
            # Path to pybind11 headers
            get_pybind_include(),
            get_pybind_include(user=True),
            *include_path_list
        ],

        libraries=[
            *all_libs
        ],

        library_dirs= library_dirs,
        define_macros = [],
        language='c++'
    ),
]


# As of Python 3.6, CCompiler has a `has_flag` method.
# cf http://bugs.python.org/issue26689
def has_flag(compiler, flagname):
    """Return a boolean indicating whether a flag name is supported on
    the specified compiler.
    """
    import tempfile
    with tempfile.NamedTemporaryFile('w', suffix='.cpp') as f:
        f.write('int main (int argc, char **argv) { return 0; }')
        try:
            compiler.compile([f.name], extra_postargs=[flagname])
        except setuptools.distutils.errors.CompileError:
            return False
    return True


def cpp_flag(compiler):
    """Return the -std=c++[11/14/17] compiler flag.

    The newer version is prefered over c++11 (when it is available).
    """
    flags = ['-std=c++17', '-std=c++14', '-std=c++11']

    for flag in flags:
        if has_flag(compiler, flag): return flag

    raise RuntimeError('Unsupported compiler -- at least C++11 support '
                       'is needed!')


class BuildExt(build_ext):
    """A custom build extension for adding compiler-specific options."""
    c_opts = {
        'msvc': ['/EHsc', '/openmp'],
        'unix': [],
    }
    l_opts = {
        'msvc': [],
        'unix': [],
    }

    if sys.platform == 'darwin':
        darwin_opts = ['-stdlib=libc++', '-mmacosx-version-min=10.7']
        c_opts['unix'] += darwin_opts
        l_opts['unix'] += darwin_opts

    def run(self):
        super().run()

        self.move_pyd_file()

    def move_pyd_file(self):
        ext_name = self.get_ext_filename(self.extensions[0].name)  # .pyd 파일 이름
        # 빌드된 .pyd 파일의 전체 경로
        build_lib = self.build_lib  # 빌드된 확장 모듈이 있는 디렉토리
        built_pyd_path = os.path.join(build_lib, ext_name)
        target_pyd_path = os.path.join(build_lib, 'meshlibs')
        self.copy_file(built_pyd_path, target_pyd_path)  # 파일을 이동
        os.remove(built_pyd_path)


    def build_extensions(self):
        ct = self.compiler.compiler_type
        opts = self.c_opts.get(ct, [])
        link_opts = self.l_opts.get(ct, [])
        if ct == 'unix':
            opts.append('-DVERSION_INFO="%s"' % self.distribution.get_version())
            opts.append(cpp_flag(self.compiler))
            if has_flag(self.compiler, '-fvisibility=hidden'):
                opts.append('-fvisibility=hidden')
        elif ct == 'msvc':
            opts.append('/DVERSION_INFO=\\"%s\\"' % self.distribution.get_version())
        for ext in self.extensions:
            ext.extra_compile_args = opts
            ext.extra_link_args = link_opts

        build_lib = self.build_lib  # 빌드된 확장 모듈이 있는 디렉토리
        print('build_lib:', build_lib)
        print('build_lib:', all_dlls)
        dest_path = os.path.join(build_lib, 'meshlibs') #, os.path.basename(dll))
        os.makedirs(dest_path, exist_ok=True)
        for dll in all_dlls:
            print(f"Copying {dll} to {dest_path}")
            self.copy_file(dll, dest_path)

        build_ext.build_extensions(self)


setup(
    name='meshlibs',
    version=__version__,
    author='jooyongLee',
    author_email='jooyong.lee@medit.com',
    url='',
    description='meshlibs python userdefined project using pybind11',
    long_description='',
    ext_modules=ext_modules,
    install_requires=['pybind11>=2.3'],
    setup_requires=['pybind11>=2.3'],
    cmdclass={'build_ext': BuildExt},
    # packages=['meshlibs']
    packages=['meshlibs'],
    zip_safe=False,
)

# # to_copy_dlls = [os.path.join(meshlibs_path, 'dll', fname + '.dll') for fname in ext_dep_meshlibs]
# for p in to_copy_dlls:
#     assert os.path.exists(p), 'cannot find dependency dll:{}'.format(p)

# # all_dll_copys = meshlibs_dlls
# print('==============dependency dll copy...==============')
# to_save_dirs_dll = os.path.join(file_path, 'dlls')
# os.makedirs(to_save_dirs_dll, exist_ok=True)

# for src_file in to_copy_dlls:
#     fname = os.path.basename(src_file)
#     tar_file = os.path.join(to_save_dirs_dll, fname)
#     shutil.copy(src_file, tar_file)
#     print('{}-->{}'.format(src_file, tar_file))
# print('==============complete==============')



