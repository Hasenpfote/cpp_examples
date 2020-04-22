﻿
#message("CMAKE_CXX_FLAGS: " ${CMAKE_CXX_FLAGS})
#message("CMAKE_CXX_FLAGS_DEBUG: " ${CMAKE_CXX_FLAGS_DEBUG})
#message("CMAKE_CXX_FLAGS_RELEASE: " ${CMAKE_CXX_FLAGS_RELEASE})
#message("CMAKE_CXX_FLAGS_MINSIZEREL: " ${CMAKE_CXX_FLAGS_MINSIZEREL})
#message("CMAKE_CXX_FLAGS_RELWITHDEBINFO: " ${CMAKE_CXX_FLAGS_RELWITHDEBINFO})

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CLANG 1)
endif()

if(WIN32)
    if(MSVC)
        if(CMAKE_CONFIGURATION_TYPES)
            set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Multi config types" FORCE)
        endif()
        ###
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3")
        ### Force to always compile with W3
        #[[
        if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
            string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
        else()
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
        endif()
        ]]
        add_definitions(-D_UNICODE -DUNICODE)
        add_definitions(-DPRODUCT_NAME=$(ProjectName))
    endif()
elseif(UNIX)
    if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Release CACHE STRING "Force a Release build." FORCE)
    endif()
    # for gcc or g++.
    if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
        #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wcast-align")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wcast-qual")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wctor-dtor-privacy")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wdisabled-optimization")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wformat=2")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Winit-self")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wlogical-op")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wmissing-declarations")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wmissing-include-dirs")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wnoexcept")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wold-style-cast")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Woverloaded-virtual")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wredundant-decls")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wshadow")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wsign-conversion")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wsign-promo")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wstrict-null-sentinel")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wstrict-overflow=5")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wswitch-default")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wundef")
        #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused")

        set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3 -pg")
        set(CMAKE_CXX_FLAGS_RELEASE "-O2 -s -DNDEBUG -march=native")
        set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -s -DNDEBUG -march=native")
        set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-Og -g3 -pg")
    # for clang.
    elseif(CLANG)
        #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weverything")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-c++98-compat")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-c++98-compat-pedantic")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-covered-switch-default")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-disabled-macro-expansion")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-global-constructors")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-inconsistent-missing-destructor-override")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-padded")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-macros")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-member-function")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-weak-vtables")

        # -s option not working with clang.
        set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3 -pg")
        set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG -march=native")
        set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG -march=native")
        set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-Og -g3 -pg")
    endif()
endif()