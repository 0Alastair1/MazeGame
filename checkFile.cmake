if(EXISTS "${FROM}")
    FILE(COPY "${FROM}" DESTINATION "${TO}")
endif()