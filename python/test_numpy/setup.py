import os
import sys
from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
import pybind11

# Pybind11 빌드 확장 정의
class get_pybind_include(object):
    """Helper class to determine the pybind11 include path"""

    def __str__(self):
        import pybind11
        return pybind11.get_include()

# 확장 모듈 정의
ext_modules = [
    Extension(
        'example',  # 모듈 이름
        ['ext.cpp'],  # C++ 소스 파일
        include_dirs=[
            # pybind11 및 NumPy 헤더 파일 경로
            get_pybind_include(),
            pybind11.get_include(user=True)
        ],
        language='c++',
        extra_compile_args=['-std=c++17'],  # C++17 표준 사용
    ),
]

# setup 함수 정의
setup(
    name='example',
    version='0.1',
    author='Your Name',
    author_email='your_email@example.com',
    description='Simple Pybind11 example with NumPy array',
    ext_modules=ext_modules,
    install_requires=['pybind11>=2.6.0', 'numpy'],  # pybind11 및 numpy 의존성
    zip_safe=False,
)