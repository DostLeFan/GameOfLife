#include "../include/Game.hpp"

Game::Game(lluint delay) : RegularTask(delay), m_grid(nullptr) {}

Game::~Game()
{
	m_grid = nullptr;
}


void Game::nextState()
{
	if(m_grid == nullptr)
		return;
	
	std::vector<std::vector<bool> > states;
	
	for(unsigned int x=0;x<m_grid->getLine();++x)
	{
		states.push_back(std::vector<bool>(m_grid->getColumn()));
		
		for(unsigned int y=0;y<m_grid->getColumn();++y)
		{
			std::vector<Case> neighbors = m_grid->getNeighbors(x, y);
			Case& currentCase = (*m_grid)(x, y);
			
			int countNeighborsAlive = 0;
			
			for(size_t i=0;i<neighbors.size();++i)
				countNeighborsAlive += (neighbors[i].isAlive() ? 1 : 0);
			
			if(countNeighborsAlive == 3)
				states[x][y] = true;
			else if(countNeighborsAlive == 2)
				states[x][y] = currentCase.isAlive();
			else
				states[x][y] = false;
		}
	}
	
	for(unsigned int x=0;x<states.size();++x)
	{
		for(unsigned int y=0;y<states[x].size();++y)
		{
			Case& currentCase = (*m_grid)(x, y);
			
			currentCase.setAlive(states[x][y]);
		}
	}
}


void Game::process()
{
	nextState();
}