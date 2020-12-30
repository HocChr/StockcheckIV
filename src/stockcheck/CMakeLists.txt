
set(STOCKCHECK_SOURCES
   evaluator.cpp

)
set(STOCKCHECK_HEADERS
   idatabase.h
   stockcheck.h
   stockentity.h
)

add_library(stockcheck ${STOCKCHECK_HEADERS} ${STOCKCHECK_SOURCES} )
