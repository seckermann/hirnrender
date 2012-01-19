from distutils.core import setup, Extension

modul = Extension("isisReader", sources=["isisReader.cpp"], include_dirs=["/home/raid1/hheinz/VTKBuild/include/vtk-5.9/", "/usr/include/isis/"], define_macros = [('_ENABLE_LOG','1'),('_ENABLE_DEBUG','1')])
setup(name = "PyIsisReader", version = "0.1", description = "Simple parser for Isis images for use with vtk", ext_modules = [modul])

# , libraries = ["libisis_core.so"],  library_dirs = ["/usr/lib/"]
