#!/usr/bin/env python

import leapmidi


def foo():
	print "Ya!"
	print "Number of arguments", leapmidi.numargs()
	return 2


def bar(baz):
	print baz, "is my homeboy"
