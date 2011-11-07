############################################################################
# LGPL License                                                             #
#                                                                          #
# This file is part of the Machine Learning Framework.                     #
# Copyright (c) 2010, Philipp Kraus, <philipp.kraus@flashpixx.de>          #
# This program is free software: you can redistribute it and/or modify     #
# it under the terms of the GNU Lesser General Public License as           #
# published by the Free Software Foundation, either version 3 of the       #
# License, or (at your option) any later version.                          #
#                                                                          #
# This program is distributed in the hope that it will be useful,          #
# but WITHOUT ANY WARRANTY; without even the implied warranty of           #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            #
# GNU Lesser General Public License for more details.                      #
#                                                                          #
# You should have received a copy of the GNU Lesser General Public License #
# along with this program. If not, see <http://www.gnu.org/licenses/>.     #
############################################################################

# -*- coding: utf-8 -*-
import os
Import("*")

flags = {}


if os.environ.has_key("CPPPATH") :
    flags["CPPPATH"] = os.environ["CPPPATH"].split(os.pathsep)
elif os.environ.has_key("CPATH") :
    flags["CPPPATH"] = os.environ["CPATH"].split(os.pathsep)
    
if os.environ.has_key("LIBRARY_PATH") :
    flags["LIBPATH"] = os.environ["DYLD_LIBRARY_PATH"].split(os.pathsep)
elif os.environ.has_key("LD_LIBRARY_PATH") :
    flags["LIBPATH"] = os.environ["LIBRARY_PATH"].split(os.pathsep)
    

    
flags["LIBS"]        = ["boost_system", "boost_thread", "boost_iostreams", "boost_regex"]
flags["CXXFLAGS"]    = ["-pipe", "-Wall", "-Wextra", "-D BOOST_FILESYSTEM_NO_DEPRECATED", "-D BOOST_NUMERIC_BINDINGS_BLAS_CBLAS"]
flags["LINKKFLAGS"]  = ["-pthread"]



if not("javac" in COMMAND_LINE_TARGETS) :
    flags["LIBS"].extend(["boost_program_options", "boost_exception", "boost_filesystem"])
else :
    flags["LINKKFLAGS"].append("-Wl,--rpath=\\$$ORIGIN")
    
    
if env["atlaslink"] == "multi" :
    flags["LIBS"].extend(["lapack", "ptcblas", "ptf77blas", "atlas"])
else :
    flags["LIBS"].append(["lapack", "cblas", "f77blas", "atlas"])

if env["withdebug"] :
    flags["CXXFLAGS"].append("-g")
else :
    flags["CXXFLAGS"].extend(["-D NDEBUG", "-D BOOST_UBLAS_NDEBUG"])

if env["withmpi"] :
    flags["CXXFLAGS"] = "mpic++"
    flags["CXXFLAGS"].append("-D MACHINELEARNING_MPI")
    flags["LIBS"].extend( ["boost_mpi", "boost_serialization"] )

if env["withrandomdevice"] :
    flags["CXXFLAGS"].append("-D MACHINELEARNING_RANDOMDEVICE")
    flags["LIBS"].append("boost_random");

if env["withmultilanguage"] :
    flags["CXXFLAGS"].append("-D MACHINELEARNING_MULTILANGUAGE")
    flags["LIBS"].append("intl");

if env["withsources"] :
    flags["CXXFLAGS"].extend(["-D MACHINELEARNING_SOURCES", "-D MACHINELEARNING_SOURCES_TWITTER"])
    flags["LIBS"].extend( ["xml2", "json"] )

if env["withfiles"] :
    flags["CXXFLAGS"].extend(["-D MACHINELEARNING_FILES", "-D MACHINELEARNING_FILES_HDF"])
    flags["LIBS"].extend( ["hdf5_cpp", "hdf5"] )

if env["withsymbolicmath"] :
    flags["CXXFLAGS"].append("-D MACHINELEARNING_SYMBOLICMATH")
    flags["LIBS"].append("ginac")

if env["withoptimize"] :
    flags["CXXFLAGS"].extend(["-O2", "-Os", "-s", "-mfpmath=sse", "-finline-functions", "-mtune="+env["cputype"]])

if env["withlogger"] :
    flags["CXXFLAGS"].append("-D MACHINELEARNING_LOGGER")



env.MergeFlags(flags)