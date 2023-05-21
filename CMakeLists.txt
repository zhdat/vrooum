cmake_minimum_required(VERSION 3.25)
project(vrooum C)

set(CMAKE_C_STANDARD 11)

include_directories(GrandPrix2023_4.0.3/follow_line)
include_directories(GrandPrix2023_4.0.3/Picole_Nationale)
include_directories(GrandPrix2023_4.0.3/test_pilot)
include_directories(GrandPrix2023_4.0.3/test_pilot_copy)
include_directories(GrandPrix2023_4.0.3/test_pilot_JPS)

add_executable(vrooum
        GrandPrix2023_4.0.3/follow_line/follow_line.c
        GrandPrix2023_4.0.3/follow_line/follow_line.h
        GrandPrix2023_4.0.3/follow_line/main.c
        GrandPrix2023_4.0.3/Picole_Nationale/droitAuBut.c
        GrandPrix2023_4.0.3/Picole_Nationale/follow_line.c
        GrandPrix2023_4.0.3/Picole_Nationale/follow_line.h
        GrandPrix2023_4.0.3/test_pilot/droitAuBut.c
        GrandPrix2023_4.0.3/test_pilot/droitAuBut.h
        GrandPrix2023_4.0.3/test_pilot/follow_line.c
        GrandPrix2023_4.0.3/test_pilot/follow_line.h
        GrandPrix2023_4.0.3/test_pilot/test_pilot.c
        GrandPrix2023_4.0.3/test_pilot_copy/droitAuBut.c
        GrandPrix2023_4.0.3/test_pilot_copy/droitAuBut.h
        GrandPrix2023_4.0.3/test_pilot_copy/follow_line.c
        GrandPrix2023_4.0.3/test_pilot_copy/follow_line.h
        GrandPrix2023_4.0.3/test_pilot_copy/test_pilot.c
        GrandPrix2023_4.0.3/test_pilot_JPS/droitAuBut.c
        GrandPrix2023_4.0.3/test_pilot_JPS/droitAuBut.h
        GrandPrix2023_4.0.3/test_pilot_JPS/follow_line.c
        GrandPrix2023_4.0.3/test_pilot_JPS/follow_line.h
        GrandPrix2023_4.0.3/test_pilot_JPS/test_pilot.c PicoleNationale_RavixJantet/src/aStar.c)
