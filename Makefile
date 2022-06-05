################################################################################
# krakfot                                                                      #
#                                                                              #
#    Copyright 2022 Tomas Kindahl                                              #
#                                                                              #
# Licensed under the Apache License, Version 2.0 (the "License");              #
# you may not use this file except in compliance with the License.             #
# You may obtain a copy of the License at                                      #
#                                                                              #
#     http://www.apache.org/licenses/LICENSE-2.0                               #
#                                                                              #
# Unless required by applicable law or agreed to in writing, software          #
# distributed under the License is distributed on an "AS IS" BASIS,            #
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.     #
# See the License for the specific language governing permissions and          #
# limitations under the License.                                               #
################################################################################

LIBS	= -lGL -lGLU -lglut
INC		= bezier.o

krakfot: krakfot.c $(INC)
	gcc -o $@ $< $(LIBS) $(INC)

bezier.o: bezier.c bezier.h
	gcc -c $<

scan.o: scan.c scan.h
	gcc -c $<

parse.o: parse.c parse.h
	gcc -c $<

test/charscan:
	cd test ; $(MAKE) $@

test/test_bezier: test_bezier.c $(INC)
	cd test ; $(MAKE) $@
