dnl $Id$
dnl config.m4 for extension groonga

PHP_ARG_ENABLE(groonga, whether to enable groonga support,
dnl Make sure that the comment is aligned:
[  --enable-groonga           Enable groonga support])

if test "$PHP_GROONGA" != "no"; then
  # --with-groonga -> check with-path
  SEARCH_PATH="/usr/local /usr"     # you might want to change this
  SEARCH_FOR="/include/groonga/groonga.h"  # you most likely want to change this
  if test -r $PHP_GROONGA/$SEARCH_FOR; then # path given as parameter
    GROONGA_DIR=$PHP_GROONGA
  else # search default path list
    AC_MSG_CHECKING([for groonga files in default path])
    for i in $SEARCH_PATH ; do
      if test -r $i/$SEARCH_FOR; then
        GROONGA_DIR=$i
        AC_MSG_RESULT(found in $i)
      fi
    done
  fi
  
  if test -z "$GROONGA_DIR"; then
    AC_MSG_RESULT([not found])
    AC_MSG_ERROR([Please reinstall the groonga distribution])
  fi

  # --with-groonga -> add include path
  PHP_ADD_INCLUDE($GROONGA_DIR/include/groonga)

  # --with-groonga -> check for lib and symbol presence
  LIBNAME=groonga # you may want to change this
  LIBSYMBOL=grn_init # you most likely want to change this 

  PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  [
    PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $GROONGA_DIR/lib, GROONGA_SHARED_LIBADD)
    AC_DEFINE(HAVE_GROONGALIB,1,[ ])
  ],[
    AC_MSG_ERROR([wrong groonga lib version or lib not found])
  ],[
    -L$GROONGA_DIR/lib -lm
  ])
  
  PHP_SUBST(GROONGA_SHARED_LIBADD)

  PHP_NEW_EXTENSION(groonga, php_groonga.c src/object.c src/command.c, $ext_shared)
fi
