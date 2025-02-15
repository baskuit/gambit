#
# This file is part of Gambit
# Copyright (c) 1994-2022, The Gambit Project (http://www.gambit-project.org)
#
# FILE: src/python/setup.py
# Setuptools configuration file for Gambit Python extension
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
#

import glob
import platform
import setuptools
import Cython.Build

# By compiling this separately as a C library, we avoid problems
# with passing C++-specific flags when building the extension
lrslib = ('lrslib', {'sources': glob.glob("solvers/lrs/*.c")})

cppgambit = (
    'cppgambit',
    {
        'sources': (
            glob.glob("core/*.cc") +
            glob.glob("games/*.cc") +
            glob.glob("games/agg/*.cc") +
            glob.glob("solvers/*/*.cc") +
            ["tools/lp/nfglp.cc",
             "tools/lp/efglp.cc",
             "tools/logit/path.cc",
             "tools/logit/nfglogit.cc",
             "tools/logit/efglogit.cc"]
         ),
         'include_dirs': ["."],
         'cflags': (
             ["-std=c++11"] if platform.system() == "Darwin" else []
         )
    }
)
    

libgambit = setuptools.Extension(
    "pygambit.lib.libgambit",
    sources=["pygambit/lib/libgambit.pyx"],
    language="c++",
    include_dirs=["."],
    extra_compile_args=(
        ["-std=c++11"] if platform.system() == "Darwin" else []
    )
)


def readme():
    with open("README.rst") as f:
        return f.read()
    

setuptools.setup(
    name="pygambit",
    version="16.1.0a1",
    description="Software tools for game theory",
    long_description=readme(),
    classifiers=[
        "Development Status :: 4 - Beta",
        "Intended Audience :: Science/Research",
        "License :: OSI Approved :: GNU General Public License v2 or later (GPLv2+)",
        "Programming Language :: Python :: 3.7",
        "Programming Language :: Python :: 3.8",
        "Programming Language :: Python :: 3.9",
        "Programming Language :: Python :: 3.10",
        "Programming Language :: Python :: Implementation :: CPython",
        "Topic :: Scientific/Engineering :: Mathematics"
    ],
    keywords="game theory Nash equilibrium",
    license="GPL2+",
    author="Theodore Turocy",
    author_email="ted.turocy@gmail.com",
    url="http://www.gambit-project.org",
    project_urls={
        'Documentation': 'https://gambitproject.readthedocs.io/',
        'Source': 'https://github.com/gambitproject/gambit',
        'Tracker': 'https://github.com/gambitproject/gambit/issues',
    },
    python_requires=">=3.7",
    install_requires=[
        'lxml',  # used for reading/writing GTE files
        'numpy',
        'scipy',
    ],
    libraries=[cppgambit, lrslib],
    packages=['pygambit', 'pygambit.games', 'pygambit.lib'],
    ext_modules=Cython.Build.cythonize(libgambit)
)
