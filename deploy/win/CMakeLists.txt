project(WinDeploy LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(QT_ROOT_DIR ${QT_DIR}/../../..)
set(QT_QML_DIR ${QT_ROOT_DIR}/qml)
set(DEPLOY_DIR ${BIN_DIR}/../delivery)
set(COPY_DEPS_APP copy_deps)

find_package(QT NAMES Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)

add_executable(${COPY_DEPS_APP} copy_deps.cpp)
target_link_librarieS(${COPY_DEPS_APP} Qt5::Core)

add_custom_target(deploy
    COMMAND ${CMAKE_COMMAND} -E env
    "PATH=$ENV{PATH}"
    windeployqt --release --no-translations --no-virtualkeyboard --force
    --qmldir ${QT_QML_DIR}
    --dir ${DEPLOY_DIR}
    ${BIN_DIR}/${LFS_TARGET}.exe
    COMMENT "Using windowsqtdeploy for Windows"
    DEPENDS ${LFS_TARGET} ${COPY_DEPS_APP}
)

set(DEPENDENCIES_FILE ${BIN_DIR}/dependencies.txt)

add_custom_command(TARGET deploy POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E echo libcrypto*.dll         >> ${DEPENDENCIES_FILE}
                   COMMAND ${CMAKE_COMMAND} -E echo libssl*.dll            >> ${DEPENDENCIES_FILE}
                   COMMAND ${CMAKE_COMMAND} -E echo ${BIN_DIR}/${LFS_TARGET}.exe >> ${DEPENDENCIES_FILE}
                   COMMAND ${CMAKE_COMMAND} -E env
                   "PATH=$ENV{PATH};${BIN_DIR}"
                   ${BIN_DIR}/${COPY_DEPS_APP} ${DEPENDENCIES_FILE} ${DEPLOY_DIR}
                   COMMAND ${CMAKE_COMMAND} -E echo Delivery can be found at ${DEPLOY_DIR}
                   COMMENT "Copying model and binaries ..."
)
