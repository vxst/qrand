# Copyright (C) 2023 Chunqing Shan
# 
# qrand is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# qrand is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
# 
# You should have received a copy of the GNU Lesser General Public License
# along with qrand. If not, see <http://www.gnu.org/licenses/>.

CXX = g++
CXXFLAGS = -Wall -O3 -std=c++11 -march=native
LDFLAGS = -ltestu01 -ltestu01probdist -ltestu01mylib

example: example.cpp include/qrand.h
	$(CXX) $(CXXFLAGS) -o example example.cpp

test_u01: test_u01.cpp include/qrand.h
	$(CXX) $(CXXFLAGS) -o test_u01 test_u01.cpp $(LDFLAGS)

.PHONY: clean test

clean:
	rm -f test_u01 example

test: test_u01
	./test_u01
