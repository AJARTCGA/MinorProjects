#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <stdio.h>
#include <string.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define CELL_SIZE 10
#define EMPTY 0
#define CONDUCTOR 1
#define HEAD 2
#define TAIL 3
#define FULLSCREEN false

const int CELLS_HOR = SCREEN_WIDTH / CELL_SIZE;
const int CELLS_VER = SCREEN_HEIGHT / CELL_SIZE;

void drawGrid(sf::RenderWindow * win);
void drawCells(sf::RenderWindow * win, int cells[][CELLS_HOR]);
void updateCells(int cells[][CELLS_HOR]);
void removeCurrent(int cells[][CELLS_HOR]);

int main()
{
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Wire World!", (FULLSCREEN) ? sf::Style::Fullscreen : 0);
    sf::RectangleShape rect(sf::Vector2f(CELL_SIZE,CELL_SIZE));
    window.setKeyRepeatEnabled(false);
    int cells[CELLS_VER][CELLS_HOR] = {{0}};
    int count = 0;
    int count2 = 0;
    bool paused = true;
    bool showGrid = true;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if(event.type == sf::Event::KeyPressed){
            	if(event.key.code == sf::Keyboard::Space)
                    paused = !paused;
                else if(event.key.code == sf::Keyboard::Escape)
                    window.close();
                else if(event.key.code == sf::Keyboard::Return && paused)
		    updateCells(cells);
		else if(event.key.code == sf::Keyboard::C)
		    memset(cells, 0, CELLS_VER * CELLS_HOR * sizeof(int));
		else if(event.key.code == sf::Keyboard::R)
		    removeCurrent(cells);
		else if(event.key.code == sf::Keyboard::G)
      		    showGrid = !showGrid;
            }
        }
	sf::Vector2i mousePos = sf::Mouse::getPosition(window);
	sf::Vector2f cursorPos(0,0);
        cursorPos.x = mousePos.x - (mousePos.x % CELL_SIZE);
        cursorPos.y = mousePos.y - (mousePos.y % CELL_SIZE);
        rect.setPosition(cursorPos);
        if(mousePos.y >= 0 && mousePos.y < SCREEN_HEIGHT && mousePos.x >= 0 && mousePos.x <= SCREEN_WIDTH){
	    if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
	       cells[(int)cursorPos.y / CELL_SIZE][(int)cursorPos.x / CELL_SIZE] = CONDUCTOR;
	    }
            else if(sf::Mouse::isButtonPressed(sf::Mouse::Right)){
	       cells[(int)cursorPos.y / CELL_SIZE][(int)cursorPos.x / CELL_SIZE] = EMPTY;
            }
            else if(sf::Mouse::isButtonPressed(sf::Mouse::Middle)){
	       cells[(int)cursorPos.y / CELL_SIZE][(int)cursorPos.x / CELL_SIZE] = HEAD;
            }
	}
        if(!paused)
          updateCells(cells);
        window.clear();
        if(showGrid)
          drawGrid(&window);
	drawCells(&window, cells);
	window.draw(rect);
        window.display();
    }
    std::cout << "Goodbye!" << std::endl;
    return 0;
}

void removeCurrent(int cells[][CELLS_HOR]){
    for(int i = 0; i < CELLS_HOR; i++){
        for(int j = 0; j < CELLS_VER; j++){
            if(cells[j][i]) //EMPTY = 0, so the cell is a conductor, head, or tail
		cells[j][i] = CONDUCTOR;
	}
    }
}

void updateCells(int cells[][CELLS_HOR]){
    int numNeighbors = 0;
    int newCells[CELLS_VER][CELLS_HOR] = {{0}};
    for(int i = 0; i < CELLS_HOR; i++){
        for(int j = 0; j < CELLS_VER; j++){
            switch(cells[j][i]){
                case EMPTY:
                    newCells[j][i] = EMPTY;
                    break;
                case CONDUCTOR:
                    numNeighbors = 0;
                    if(i != 0){
                        if(cells[j][i - 1] == HEAD)
		  	    numNeighbors++;
                        if(j != 0 && cells[j - 1][i - 1] == HEAD)
                            numNeighbors++;
                        if(j != CELLS_VER - 1 && cells[j + 1][i - 1] == HEAD)
		            numNeighbors++;
                    }
                    if(i != CELLS_HOR - 1){
                        if(cells[j][i + 1] == HEAD)
                            numNeighbors++;
                        if(j != 0 && cells[j - 1][i + 1] == HEAD)
                            numNeighbors++;
                        if(j != CELLS_VER - 1 && cells[j + 1][i + 1] == HEAD)
		            numNeighbors++;
                    }
                    if(j != 0 && cells[j - 1][i] == HEAD)
			numNeighbors++;
                    if(j != CELLS_VER - 1 && cells[j + 1][i] == HEAD)
			numNeighbors++;
                    if(numNeighbors == 1 || numNeighbors == 2)
                        newCells[j][i] = HEAD;
		    else
                        newCells[j][i] = CONDUCTOR;
                    break;
                case HEAD:
                    newCells[j][i] = TAIL;
                    break;
                case TAIL:
                    newCells[j][i] = CONDUCTOR;
                    break;
            }
        }
    }
    memcpy(cells, newCells, CELLS_VER * CELLS_HOR * sizeof(int));
}

void drawCells(sf::RenderWindow * win, int cells[][CELLS_HOR]){
    sf::RectangleShape cellRect(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
    for(int i = 0; i < CELLS_HOR; i++){
        for(int j = 0; j < CELLS_VER; j++){
            cellRect.setPosition(sf::Vector2f(CELL_SIZE * i, CELL_SIZE * j));
	    switch(cells[j][i]){
                case EMPTY:
                  break;
                case CONDUCTOR:
                  cellRect.setFillColor(sf::Color(200,200,0));
                  win->draw(cellRect);
		  break;
		case HEAD:
		  cellRect.setFillColor(sf::Color(200,0,0));
	 	  win->draw(cellRect);
                  break;
		case TAIL:
		  cellRect.setFillColor(sf::Color(0,0,200));
	 	  win->draw(cellRect);
                  break;
            }
        }
    }
}

void drawGrid(sf::RenderWindow * win){
  sf::Color gray(150,150,150,255);
  for(int i = 0; i <= SCREEN_WIDTH; i+=CELL_SIZE)
  {
    sf::Vertex line[] =
    {
        sf::Vertex(sf::Vector2f(i, 0), gray),
        sf::Vertex(sf::Vector2f(i, SCREEN_HEIGHT), gray)
    };
    win->draw(line, 2, sf::Lines);
  }
  for(int i = 0; i <= SCREEN_HEIGHT; i+=CELL_SIZE)
  {
    sf::Vertex line[] =
    {
        sf::Vertex(sf::Vector2f(0, i), gray),
        sf::Vertex(sf::Vector2f(SCREEN_WIDTH, i), gray)
    };
    win->draw(line, 2, sf::Lines);
  }
}
