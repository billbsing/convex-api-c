AC_INIT([convex-api-c library], 0.01)
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_PROG_CC
AC_CONFIG_SRCDIR([src/])
AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_FILES([
 Makefile
 src/Makefile
 tests/Makefile
 examples/Makefile
])
AC_CONFIG_MACRO_DIRS([m4])

# Checks for libraries.


PKG_CHECK_MODULES([libcrypto], [libssl],,
    [AC_MSG_ERROR([No pkg-config for libssl])])

AC_CHECK_LIB([crypto], [EVP_PKEY_keygen],,[AC_MSG_ERROR([libcrypto ssl required!])])
AC_CHECK_LIB([crypto], [EVP_PKEY_keygen_init],,[AC_MSG_ERROR([libcrypto ssl required!])])

AC_CHECK_LIB([ssl], [BIO_s_mem],,[AC_MSG_ERROR([libcrypto ssl required!])])
AC_CHECK_LIB([ssl], [BIO_new],,[AC_MSG_ERROR([libcrypto ssl required!])])
AC_CHECK_LIB([ssl], [BIO_puts],,[AC_MSG_ERROR([libcrypto ssl required!])])
AC_CHECK_LIB([ssl], [BIO_free_all],,[AC_MSG_ERROR([libcrypto ssl required!])])
# AC_CHECK_LIB([ssl], [BIO_get_mem_data],,[AC_MSG_ERROR([libcrypto ssl required!])])


# AC_CHECK_LIB([crypto], [EVP_aes_256_cfb],,[AC_MSG_ERROR([libcrypto ssl required!])])

# AC_CHECK_LIB([crypto], [EVP_DigestInit_ex],,[AC_MSG_ERROR([libcrypto ssl required!])])
# AC_CHECK_LIB([crypto], [EVP_DigestFinal_ex],,[AC_MSG_ERROR([libcrypto ssl required!])])
# AC_CHECK_LIB([crypto], [EVP_DigestSignInit],,[AC_MSG_ERROR([libcrypto ssl required!])])
# AC_CHECK_LIB([crypto], [EVP_DigestSign],,[AC_MSG_ERROR([libcrypto ssl required!])])
# AC_CHECK_LIB([crypto], [EVP_DigestUpdate],,[AC_MSG_ERROR([libcrypto ssl required!])])


# AC_CHECK_LIB([crypto], [EVP_MD_CTX_new],,[AC_MSG_ERROR([libcrypto ssl required!])])
# AC_CHECK_LIB([crypto], [EVP_MD_CTX_free],,[AC_MSG_ERROR([libcrypto ssl required!])])
# AC_CHECK_LIB([crypto], [EVP_PKEY_CTX_free],,[AC_MSG_ERROR([libcrypto ssl required!])])
# AC_CHECK_LIB([crypto], [EVP_PKEY_CTX_new_id],,[AC_MSG_ERROR([libcrypto ssl required!])])
# AC_CHECK_LIB([crypto], [EVP_PKEY_free],,[AC_MSG_ERROR([libcrypto ssl required!])])
# AC_CHECK_LIB([crypto], [EVP_PKEY_get_raw_public_key],,[AC_MSG_ERROR([libcrypto ssl required!])])
# AC_CHECK_LIB([crypto], [EVP_PKEY_keygen],,[AC_MSG_ERROR([libcrypto ssl required!])])
# AC_CHECK_LIB([crypto], [EVP_PKEY_keygen_init],,[AC_MSG_ERROR([libcrypto ssl required!])])
# AC_CHECK_LIB([crypto], [EVP_PKEY_size],,[AC_MSG_ERROR([libcrypto ssl required!])])
# AC_CHECK_LIB([crypto], [EVP_sha3_256],,[AC_MSG_ERROR([libcrypto ssl required!])])


AC_CHECK_LIB([crypto], [PEM_read_bio_PrivateKey],,[AC_MSG_ERROR([libcrypto ssl required!])])
AC_CHECK_LIB([crypto], [PEM_read_PrivateKey],,[AC_MSG_ERROR([libcrypto ssl required!])])
AC_CHECK_LIB([crypto], [PEM_write_PKCS8PrivateKey],,[AC_MSG_ERROR([libcrypto ssl required!])])
AC_CHECK_LIB([crypto], [PEM_write_bio_PKCS8PrivateKey],,[AC_MSG_ERROR([libcrypto ssl required!])])


AC_SUBST(SSL_CFLAGS)
AC_SUBST(SSL_LIBS)


PKG_CHECK_MODULES([curl], [libcurl],,
    [AC_MSG_ERROR([No pkg-config for libcurl])])

AC_CHECK_LIB([curl], [curl_easy_init],,[AC_MSG_ERROR([libcurl required!])])
AC_CHECK_LIB([curl], [curl_easy_escape],,[AC_MSG_ERROR([libcurl required!])])
AC_CHECK_LIB([curl], [curl_easy_setopt],,[AC_MSG_ERROR([libcurl required!])])
AC_CHECK_LIB([curl], [curl_easy_cleanup],,[AC_MSG_ERROR([libcurl required!])])
AC_CHECK_LIB([curl], [curl_free],,[AC_MSG_ERROR([libcurl required!])])

AC_SUBST(CURL_CFLAGS)
AC_SUBST(CURL_LIBS)

JSON_LIBS=
# PKG_CHECK_MODULES(JSON, json-c,, [AC_MSG_WARN("json-c not found")])

# if test "$JSON_LIBS" == ""; then
# PKG_CHECK_MODULES(JSON, json,
# 		  [AC_DEFINE([HAVE_JSON_0],[1],[Use JSON0])],
# 		  [AC_MSG_ERROR("json0 missing")])
# fi

# AC_SUBST(JSON_CFLAGS)
# AC_SUBST(JSON_LIBS)

# Checks for header files.
AC_CHECK_HEADERS([stddef.h stdint.h stdio.h stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T
AC_TYPE_INT64_T
AC_TYPE_UINT32_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([malloc, realloc, calloc strchr strcpy strncat sprintf])


AM_PROG_AR
LT_INIT
AC_OUTPUT

