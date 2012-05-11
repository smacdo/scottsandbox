cmake_minimum_required(VERSION 2.6)
project(lzmadecode)

add_library( lzmadecode STATIC
             LzmaDec.h
             LzmaDec.c
             Types.h )
