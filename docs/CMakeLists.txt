find_package(Doxygen REQUIRED dot OPTIONAL_COMPONENTS mscgen dia)

if(DOXYGEN_FOUND)
  # set input and output files
  set(DOXYFILE_IN ${CMAKE_CURRENT_LIST_DIR}/Doxyfile.in)
  set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile.out)

  # request to configure the file
  configure_file(${DOXYFILE_IN} ${DOXYGEN_OUT} @ONLY)
  message("Doxygen build started")

  # Note: do not put "ALL" - this builds docs together with application EVERY
  # TIME!
  add_custom_target(
    docs
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API documentation with Doxygen"
    VERBATIM)
else(DOXYGEN_FOUND)
  message("Doxygen needs to be installed to generate the doxygen documentation")
endif(DOXYGEN_FOUND)
