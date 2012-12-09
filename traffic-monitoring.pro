TEMPLATE = subdirs

gui_proj.subdir = gui
lib_proj.subdir = lib

gui_proj.depends = lib_proj

SUBDIRS  = lib_proj \
           gui_proj
