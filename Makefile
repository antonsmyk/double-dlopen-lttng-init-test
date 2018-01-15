# The libspam test dynamic library and spam-loader test executable
# Copyright 2017 Itiviti AB, Anton Smyk <Anton.Smyk@itiviti.com>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the "Software"), to deal in
# the Software without restriction, including without limitation the rights to
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
# of the Software, and to permit persons to whom the Software is furnished to do
# so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

CPPFLAGS := -I. -D_GNU_SOURCE
CFLAGS := -std=gnu99 -pedantic -Wall -Wextra -pthread -g
LDFLAGS := -pthread

.PHONY: all
all: libspam.so libspam-clone.so spam-loader

libspam.so: LDFLAGS += -shared
libspam.so: LIBS += -lrt
libspam.so: libspam-foo.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)

libspam-foo.o: CFLAGS += -fPIC
libspam-foo.o: libspam-foo.c

libspam-clone.so: libspam.so
	cp -f libspam.so libspam-clone.so

spam-loader: LDFLAGS += -ldl
spam-loader: spam-loader.o

.PHONY: clean
clean:
	rm -f spam-loader
	rm -f libspam.so
	rm -f libspam-clone.so
	rm -f *.o

