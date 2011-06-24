project(Workbench)
cmake_minimum_required(VERSION 2.6)

init_googletest()

add_definitions("-std=c++0x")

###
### Build projects
###
add_simple_workbench_item(base64)
add_simple_workbench_item(bits)
add_simple_workbench_item(crc)
add_simple_workbench_item(datamanager)
#add_simple_workbench_item(debuglogger)
add_simple_workbench_item(entity)
add_simple_workbench_item(fileutils)
add_simple_workbench_item(lrucache)
add_simple_workbench_item(nullstream)
add_simple_workbench_item(octree)
add_simple_workbench_item(profilerobj)
add_simple_workbench_item(random)
add_simple_workbench_item(shufflebag)
add_simple_workbench_item(statcounter)
add_simple_workbench_item(stringutils)
add_simple_workbench_item(time)
add_simple_workbench_item(volume)
