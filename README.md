Dijkstra's shortest path algorithm
==================================

This is my humble C99 implementation of Dijkstra's shortest path algorithm from 2007.

See http://en.wikipedia.org/wiki/Dijkstra's_algorithm

    gcc -std=c99 -Wconversion -Wsign-conversion -Wextra -Werror \
      -pedantic -Wall -Wextra -Wcast-align -Wcast-qual \
      -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op \
      -Wmissing-include-dirs -Wredundant-decls -Wstrict-overflow=5 \
      -Wswitch-default -Wundef -Wno-unused \
      -o dijkstra dijkstra.c
 
 