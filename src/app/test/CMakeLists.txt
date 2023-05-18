project(test)

#message(${SETUP_RESOURCE_PATH}/setting/env.ini)

#cmrc_add_resource_library(reflex-rc
#    ALIAS reflex::rc
#    NAMESPACE reflex
#    WHENCE ${SETUP_RESOURCE_PATH}/setting
#    PREFIX setting
#    ${SETUP_RESOURCE_PATH}/setting/env.ini)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} uzmq)









