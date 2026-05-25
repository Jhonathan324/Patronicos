cmake_minimum_required(VERSION 3.16)

project(Patronicos C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# --- LÓGICA DE DETECÇÃO DO ALLEGRO ---

if(WIN32)
    # No Windows, utiliza estrutura de pasta local
    message(STATUS "Configurando para Windows (Pasta Local)")
    set(ALLEGRO_DIR ${CMAKE_SOURCE_DIR}/allegro)
    set(ALLEGRO_INCLUDE_DIR ${ALLEGRO_DIR}/include)
    set(ALLEGRO_LIB_DIR ${ALLEGRO_DIR}/lib)
    
    # Define as bibliotecas específicas (monolith)
    set(ALLEGRO_LIBRARIES 
        $<$<CONFIG:Debug>:${ALLEGRO_LIB_DIR}/liballegro_monolith-debug.dll.a>
        $<$<NOT:$<CONFIG:Debug>>:${ALLEGRO_LIB_DIR}/liballegro_monolith.dll.a>
    )
else()
    # No Linux (Ubuntu), busca o Allegro instalado no sistema via APT
    # Você precisa instalar os pacotes com o comando sudo apt install liballegro5-dev

    message(STATUS "Configurando para Linux (Pacotes do Sistema)")
    
    # No Linux, o Allegro costuma ser dividido em módulos. 
    # O comando abaixo busca o componente principal e os mais comuns.
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(ALLEGRO REQUIRED 
        allegro-5 
        allegro_main-5 
        allegro_image-5 
        allegro_font-5 
        allegro_ttf-5 
        allegro_audio-5 
        allegro_acodec-5 
        allegro_primitives-5
    )
    
    set(ALLEGRO_INCLUDE_DIR ${ALLEGRO_INCLUDE_DIRS})
    set(ALLEGRO_LIBRARIES ${ALLEGRO_LIBRARIES})
endif()

# --- ALVO DO PROJETO ---

add_executable(Patronicos src/main.c)

target_include_directories(Patronicos PRIVATE ${ALLEGRO_INCLUDE_DIR})

target_link_libraries(Patronicos PRIVATE ${ALLEGRO_LIBRARIES})