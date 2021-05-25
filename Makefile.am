ACLOCAL_AMFLAGS = -I m4

SUBDIRS = src tests
dist_doc_DATA = README
CLEANFILES = *~ \#*

doxygen.config: Makefile.in
	echo -e "PROJECT_NAME          = convex-api-py\n \
USE_MDFILE_AS_MAINPAGE  = README.md\n \
INPUT		        = src/\n \
FILE_PATTERNS           = *.h *.c\n \
OUTPUT_DIRECTORY        = docs/\n \
OPTIMIZE_OUTPUT_FOR_C   = YES\n \
EXTRACT_ALL             = YES" > doxygen.config

docs: doxygen.config
	doxygen doxygen.config
