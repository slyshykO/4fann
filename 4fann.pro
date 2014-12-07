#-------------------------------------------------
#
#
#
#-------------------------------------------------

TEMPLATE = subdirs

SUBDIRS += ./src.pro \
           ./3rdparty/fanndouble/fanndouble.pro

src.pro.depends = fanndouble.pro

#http://blog.rburchell.com/2013/10/every-time-you-configordered-kitten-dies.html
#TEMPLATE = subdirs
#SUBDIRS = src plugins tests docs
#plugins.depends = src
#tests.depends = src plugins
