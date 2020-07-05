
set(STOCKCHECK_SOURCE
   evaluator.cpp

)
set(STOCKCHECK_HEADERS
   evaluator.cpp
   idatabase.h
   stockcheck.h
   stockentity.h
)

add_library(stockcheck ${STOCKCHECK_HEADERS} ${STOCKCHECK_SOURCES} )