CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

SET(BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

SET(EXECUTABLE_OUTPUT_PATH ${BASE_DIR})

SET(UTILCCI_DIR "${CMAKE_CURRENT_LIST_DIR}/utilcc")
INCLUDE_DIRECTORIES(${UTILCCI_DIR})

ADD_SUBDIRECTORY("${UTILCCI_DIR}")
ADD_SUBDIRECTORY("${BASE_DIR}/1-arrays-and-strings/1.8-isrotation")
ADD_SUBDIRECTORY("${BASE_DIR}/2-linked-lists/2.1-remove-duplicates-unsorted")
ADD_SUBDIRECTORY("${BASE_DIR}/2-linked-lists/2.2-kth-to-last-element")
ADD_SUBDIRECTORY("${BASE_DIR}/2-linked-lists/2.3-delete-middle-element")
ADD_SUBDIRECTORY("${BASE_DIR}/2-linked-lists/2.4-list-partition-around-x")
ADD_SUBDIRECTORY("${BASE_DIR}/2-linked-lists/2.5-sum-two-numbers")
	
