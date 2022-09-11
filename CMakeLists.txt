cmake_minimum_required(VERSION 3.15)
# set the project name
project(test)
#ADD_SUBDIRECTORY(bplustree)
# add the executable

add_executable(test blinktree/Demo.cpp blinktree/BPlusTree.cpp blinktree/BPlusTree.h)