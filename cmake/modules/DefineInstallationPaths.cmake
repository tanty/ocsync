if (UNIX)
  # Suffix for Linux
  SET(LIB_SUFFIX
    CACHE STRING "Define suffix of directory name (32/64)"
  )

  SET(EXEC_INSTALL_PREFIX
    "${CMAKE_INSTALL_PREFIX}"
    CACHE PATH  "Base directory for executables and libraries"
  )
  SET(SHARE_INSTALL_PREFIX
    "${CMAKE_INSTALL_PREFIX}/share"
    CACHE PATH "Base directory for files which go to share/"
  )
  SET(DATA_INSTALL_PREFIX
    "${SHARE_INSTALL_PREFIX}/${APPLICATION_SHORT_NAME}"
    CACHE PATH "The parent directory where applications can install their data")

  # The following are directories where stuff will be installed to
  SET(BIN_INSTALL_DIR
    "${CMAKE_INSTALL_BINDIR}"
    CACHE PATH "The ${APPLICATION_SHORT_NAME} binary install dir (default prefix/bin)"
  )
  SET(SBIN_INSTALL_DIR
    "${EXEC_INSTALL_PREFIX}/sbin"
    CACHE PATH "The ${APPLICATION_SHORT_NAME} sbin install dir (default prefix/sbin)"
  )
  SET(LIB_INSTALL_DIR
    "${CMAKE_INSTALL_LIBDIR}"
    CACHE PATH "The subdirectory relative to the install prefix where libraries will be installed (default is prefix/lib)"
  )
  SET(LIBEXEC_INSTALL_DIR
    "${EXEC_INSTALL_PREFIX}/libexec"
    CACHE PATH "The subdirectory relative to the install prefix where libraries will be installed (default is prefix/libexec)"
  )
  SET(PLUGIN_INSTALL_DIR
    "${LIB_INSTALL_DIR}/${APPLICATION_SHORT_NAME}"
    CACHE PATH "The subdirectory relative to the install prefix where plugins will be installed (default is prefix/lib/${APPLICATION_SHORT_NAME})"
  )
  SET(INCLUDE_INSTALL_DIR
    "${CMAKE_INSTALL_PREFIX}/include"
    CACHE PATH "The subdirectory to the header prefix (default prefix/include)"
  )

  SET(DATA_INSTALL_DIR
    "${DATA_INSTALL_PREFIX}"
    CACHE PATH "The parent directory where applications can install their data (default prefix/share/${APPLICATION_SHORT_NAME})"
  )
  SET(HTML_INSTALL_DIR
    "${DATA_INSTALL_PREFIX}/doc/HTML"
    CACHE PATH "The HTML install dir for documentation (default data/doc/html)"
  )
  SET(ICON_INSTALL_DIR
    "${DATA_INSTALL_PREFIX}/icons"
    CACHE PATH "The icon install dir (default data/icons/)"
  )
  SET(SOUND_INSTALL_DIR
    "${DATA_INSTALL_PREFIX}/sounds"
    CACHE PATH "The install dir for sound files (default data/sounds)"
  )

  SET(LOCALE_INSTALL_DIR
    "${SHARE_INSTALL_PREFIX}/locale"
    CACHE PATH "The install dir for translations (default prefix/share/locale)"
  )

  SET(XDG_APPS_DIR
    "${SHARE_INSTALL_PREFIX}/applications/"
    CACHE PATH "The XDG apps dir"
  )
  SET(XDG_DIRECTORY_DIR
    "${SHARE_INSTALL_PREFIX}/desktop-directories"
    CACHE PATH "The XDG directory"
  )

  IF(NOT "${EXEC_INSTALL_PREFIX}" STREQUAL "/usr")
    SET(SYSCONFDIR_INSTALL_PREFIX "${EXEC_INSTALL_PREFIX}")
  endif()

  SET(SYSCONF_INSTALL_DIR
    "${SYSCONFDIR_INSTALL_PREFIX}/etc"
    CACHE PATH "The ${APPLICATION_SHORT_NAME} sysconfig install dir (default prefix/etc)"
  )
  SET(MAN_INSTALL_DIR
    "${SHARE_INSTALL_PREFIX}/man"
    CACHE PATH "The ${APPLICATION_SHORT_NAME} man install dir (default prefix/man)"
  )
  SET(INFO_INSTALL_DIR
    "${SHARE_INSTALL_PREFIX}/info"
    CACHE PATH "The ${APPLICATION_SHORT_NAME} info install dir (default prefix/info)"
  )
endif (UNIX)

if (WIN32)
  # Same same
  set(SHARE_INSTALL_PREFIX "share" CACHE PATH "-")
  set(BIN_INSTALL_DIR "." CACHE PATH "-")
  set(SBIN_INSTALL_DIR "." CACHE PATH "-")
  set(LIB_INSTALL_DIR "lib" CACHE PATH "-")
  set(INCLUDE_INSTALL_DIR "include" CACHE PATH "-")
  set(PLUGIN_INSTALL_DIR "plugins" CACHE PATH "-")
  set(HTML_INSTALL_DIR "doc/HTML" CACHE PATH "-")
  set(ICON_INSTALL_DIR "." CACHE PATH "-")
  set(SOUND_INSTALL_DIR "." CACHE PATH "-")
  set(LOCALE_INSTALL_DIR "lang" CACHE PATH "-")
  set(SYSCONF_INSTALL_DIR "config" CACHE PATH "-")
  set(MAN_INSTALL_DIR "man" CACHE PATH "-")
endif (WIN32)