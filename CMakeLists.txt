set(REQUIRES_PASIM true)

project(patmos-benchmarks)

cmake_minimum_required(VERSION 2.8)

# we are only using C at the moment
enable_language(C)

# we override add_executable to collect a list of executables in the global property tgt_list_prop
# and use it in setup_all_link_flags().
# IMPORTANT: this overriding of a CMake builtin is fragile, DON'T move it to included scripts!
function(add_executable TARGET)
  set_property(GLOBAL APPEND PROPERTY tgt_list_prop ${TARGET})
  _add_executable(${TARGET} ${ARGN})
endfunction()

# enable testing using ctest
if(ENABLE_TESTING)
  # Target-dependent macros for testing:
  #     macro (run_io name prog in out ref)
  enable_testing()
endif()

# global configuration options
if("${CMAKE_SYSTEM_NAME}" MATCHES "patmos")
  set(IS_BIGENDIAN true)
endif()


set(BUILD_WCET_ANALYSIS false CACHE BOOL "Compile for WCET analysis where applicable (eg. no printfs).")
set(TACLE_BENCH false CACHE BOOL "Use TACLe version of benchmarks when available (transitional).")

# RTEMS based build (EXPERIMENTAL)
if(${TRIPLE} MATCHES "patmos-unknown-rtems")
  # add RTEMS include path
  include_directories(${RTEMS_LIBPATH}/include)

  # add RTEMS default libs (linking with -L .. -l.. does not work)
  add_library(rtemscpu STATIC IMPORTED)
  add_library(rtemsbsp STATIC IMPORTED)
  set_property(TARGET rtemscpu PROPERTY IMPORTED_LOCATION ${RTEMS_LIBPATH}/librtemscpu.a)
  set_property(TARGET rtemsbsp PROPERTY IMPORTED_LOCATION ${RTEMS_LIBPATH}/librtemsbsp.a)
  link_libraries(rtemscpu rtemsbsp)
endif()

set(ENABLE_PAPABENCH true CACHE BOOL "Enable Papabench tests)")
set(ENABLE_ASM_TESTS true CACHE BOOL "Enable custom assembly tests")
set(ENABLE_C_TESTS true CACHE BOOL "Enable custom C tests")
set(ENABLE_PLATIN_TESTS false CACHE BOOL "Enable tests aimed at platin")
set(ENABLE_MIBENCH true CACHE BOOL "Enable MiBench tests)")
set(ENABLE_MEDIABENCH true CACHE BOOL "Enable Mediabench tests)")
set(ENABLE_DEBIE true CACHE BOOL "Enable Debie tests)")
set(ENABLE_HELI true CACHE BOOL "Enable Heli tests)")
set(ENABLE_NONFREE true CACHE BOOL "Enable non-free tests)")
set(ENABLE_TCAS true CACHE BOOL "Enable TCAS tests")
set(ENABLE_WTC14 true CACHE BOOL "Enable WTC14 tests")

# add benchmark sub-directories
if(ENABLE_DEBIE)
  add_subdirectory(Debie1-e)
endif()
if(ENABLE_HELI)
  add_subdirectory(Heli)
endif()
if(ENABLE_MIBENCH)
  add_subdirectory(MiBench)
endif()
add_subdirectory(Malardalen)
if(ENABLE_PAPABENCH)
  add_subdirectory(PapaBench-0.4)
endif()
if(ENABLE_MEDIABENCH)
  add_subdirectory(Mediabench)
endif()
if(ENABLE_ASM_TESTS)
  add_subdirectory(asm-tests)
endif()
if(ENABLE_C_TESTS)
  add_subdirectory(tests)
endif()
if(ENABLE_TCAS)
  add_subdirectory(TCAS)
endif()
if(ENABLE_WTC14)
  add_subdirectory(WTC14-misc)
endif()

# add non-free benchmarks
if(ENABLE_NONFREE)
  if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/nonfree)
    add_subdirectory(nonfree)
  else()
    message("Omitting non-free benchmarks.")
  endif()
endif()



set(ENABLE_CTORTURE true CACHE BOOL "Enable gcc.c-torture suite (long build time)")
set(GCC_C_TORTURE_EXECUTE_PATH ${CMAKE_CURRENT_BINARY_DIR}/gcc.c-torture/execute CACHE FILEPATH 
    "Path to the gcc.c_torture/execute checkout.")

find_package(Subversion)
if(SUBVERSION_FOUND AND ENABLE_CTORTURE)
  # add gcc.torture-execute

  # TODO: Update revision to be checked out every once in a while
  set(GCC_C_TORTURE_REPO svn://gcc.gnu.org/svn/gcc/trunk/gcc/testsuite/gcc.c-torture)
  set(GCC_C_TORTURE_REVISION 223851)

  # we only want the "execute" part of the torture tests
  execute_process(COMMAND ${Subversion_SVN_EXECUTABLE}
	checkout ${GCC_C_TORTURE_REPO}/execute -r ${GCC_C_TORTURE_REVISION}
	${GCC_C_TORTURE_EXECUTE_PATH})

  add_subdirectory(gcc.c-torture)
endif()

# add timing analysis sub-directory
if(A3_EXECUTABLE)
  add_subdirectory(a3)
endif()

# this is how we set the LINK_FLAGS property with Patmos for all our executable targets
if("${CMAKE_SYSTEM_NAME}" MATCHES "patmos")
  setup_all_link_flags()
endif()
