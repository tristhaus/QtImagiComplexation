#
# This file is part of QtImagiComplexation.
#
# QtImagiComplexation is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# QtImagiComplexation is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with QtImagiComplexation.  If not, see <http://www.gnu.org/licenses/>.
#
#

include(../Backend/Backend.pri)

# you may need to change this
GOOGLETEST_DIR = D:\\VSProject\\googletest

include(gtest_dependency.pri)

TEMPLATE = app
CONFIG += console c++11 thread
CONFIG -= app_bundle

HEADERS += \
        tst_basez.h \
        tst_constant.h

SOURCES += \
        main.cpp
