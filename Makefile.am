ACLOCAL_AMFLAGS = -I m4

SUBDIRS = src tests examples
dist_doc_DATA = README
CLEANFILES = *~ \#*

doxygen.config: Makefile.in
	echo -e "PROJECT_NAME          = convex-api-py\n \
INPUT		        = src/\n \
FILE_PATTERNS           = *.h *.c\n \
EXAMPLE_PATH		= examples\n \
EXAMPLE_PATTERNS	= *.c\n \
OUTPUT_DIRECTORY        = docs/\n \
OPTIMIZE_OUTPUT_FOR_C   = YES\n \
EXTRACT_ALL             = YES" > doxygen.config

docs: doxygen.config
	doxygen doxygen.config
