/*
 * The spam-loader test executable
 * Copyright 2018 Itiviti AB, Anton Smyk <Anton.Smyk@itiviti.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "spam-loader-trace.h"
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>

#define DLOPEN_FLAGS RTLD_NOW

static void *load_module(const char *path) {
	void *module = dlopen(path, DLOPEN_FLAGS);
	if (module == NULL) {
		fprintf(stderr, "dlopen() failed: %s\n", dlerror());
		return module;
	}
	return module;
}

static void unload_module(void *handle) {
	int ret = dlclose(handle);
	if (ret) {
		fprintf(stderr, "dlclose() failed: %s\n", dlerror());
		return;
	}
}

static void test_module(void *handle, int arg, const char *fn_name) {
	void (*fn)(unsigned) = dlsym(handle, fn_name);
	if (fn != NULL) {
		fprintf(stderr, "Calling function %s(%d) ... ",
				fn_name, arg);
		fn(arg);
		fprintf(stderr, "OK\n");
	}
}

int main(int argc, char **argv) {
	void *handle_1;
	void *handle_2;

	tracepoint(spam_loader, phase_marker, "Start");

	fprintf(stderr, "Loading version #1 %s... ", argv[1]);
	handle_1 = load_module(argv[1]);
	fprintf(stderr, "OK\n");

	tracepoint(spam_loader, phase_marker, "Loaded first module");

	test_module(handle_1, 1, "spam_foo");
	test_module(handle_1, 10, "spam_bar");

	fprintf(stderr, "Loading version #2 %s... ", argv[2]);
	handle_2 = load_module(argv[2]);
	fprintf(stderr, "OK\n");

	tracepoint(spam_loader, phase_marker, "Loaded second module");

	test_module(handle_1, 2, "spam_foo");
	test_module(handle_1, 20, "spam_bar");
	test_module(handle_2, 3, "spam_foo");
	test_module(handle_2, 30, "spam_bar");

	fprintf(stderr, "Unloading version #1 ...");
	unload_module(handle_1);
	fprintf(stderr, "OK\n");

	tracepoint(spam_loader, phase_marker, "Unloaded first module");

	test_module(handle_2, 4, "spam_foo");
	test_module(handle_2, 40, "spam_bar");

	fprintf(stderr, "Unloading version #2 ...");
	unload_module(handle_2);
	fprintf(stderr, "OK\n");

	tracepoint(spam_loader, phase_marker, "Unloaded second module");

	fprintf(stderr, "Finished.\n");
	tracepoint(spam_loader, phase_marker, "Finish");
}

