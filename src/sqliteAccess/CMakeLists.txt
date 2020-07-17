
add_library(sqliteAccess sqlite3.h sqliteAccess.h sqlite3.c sqliteAccess.cpp )

target_link_libraries(sqliteAccess
    stdc++fs
    )
