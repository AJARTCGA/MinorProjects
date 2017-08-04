#!/bin/bash
g++ -c main.cpp
g++ main.o -o WireWorld -lsfml-graphics -lsfml-system -lsfml-window
