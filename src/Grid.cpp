#include "../include/Grid.hpp"

#include <string>
#include <stdexcept>

Grid::Grid(unsigned int line, unsigned int col, int x, int y, unsigned int caseSize, float spacing, bool toroidal) : m_line(line), m_column(col), m_x(x), m_y(y), m_caseSize(caseSize), m_spacing(spacing), m_toroidal(toroidal)
{
	recreateCases();
}

Grid::Grid(Grid const& src) : m_line(src.m_line), m_column(src.m_column), m_x(src.m_x), m_y(src.m_y), m_caseSize(src.m_caseSize), m_spacing(src.m_spacing), m_toroidal(src.m_toroidal), m_cases(src.m_cases) {}


Grid& Grid::operator=(Grid const& src)
{
	m_line = src.m_line;
	m_column = src.m_column;
	m_x = src.m_x;
	m_y = src.m_y;
	m_caseSize = src.m_caseSize;
	m_spacing = src.m_spacing;
	m_toroidal = src.m_toroidal;
	m_cases = src.m_cases;
	
	return *this;
}

Case& Grid::operator()(size_t line, size_t column)
{
	if(line >= m_line || column >= m_column)
		throw std::out_of_range("Grid::operator() out of range");
	
	return m_cases[(line * m_column + column)];
}

Case const& Grid::operator()(size_t line, size_t column) const
{
	if(line >= m_line || column >= m_column)
		throw std::out_of_range("Grid::operator() out of range");
	
	return m_cases[(line * m_column + column)];
}


std::vector<Case> Grid::getNeighbors(size_t i, size_t j) const
{
	std::vector<Case> neighbors;
	
	for(int di=-1;di<=1;++di)
	{
		for(int dj=-1;dj<=1;++dj)
		{
			if(di == 0 && dj == 0)
				continue;
			
			int ni = static_cast<int>(i) + di;
			int nj = static_cast<int>(j) + dj;
			
			if(m_toroidal)
			{
				ni = (ni + m_line) % m_line;
				nj = (nj + m_column) % m_column;
				
				neighbors.push_back((*this)(ni, nj));
			}
			else
			{
				if(ni >= 0 && ni < static_cast<int>(m_line) && nj >= 0 && nj < static_cast<int>(m_column))
					neighbors.push_back((*this)(ni, nj));
			}
		}
	}
	
	return neighbors;
}

size_t Grid::getCaseIndexFromPixel(int x, int y, int offsetX, int offsetY)
{
	float step = m_caseSize + m_spacing * 2;
	int col = (x - offsetX) / step;
	int row = (y - offsetY) / step;
	
	if(col < 0 || col >= static_cast<int>(m_column) || row < 0 || row >= static_cast<int>(m_line))
		throw std::out_of_range("Grid::getCaseIndexFromPixel() out of range");
	
	return (row * m_column + col);
}


void Grid::draw(sf::RenderWindow& win) const
{
	//float spacing = 2.f;
	
	for(unsigned int i=0;i<(m_line*m_column);++i)
	{
		/*unsigned int line = i / m_column;
		unsigned int column = i % m_column;
		
		Case const& c = m_cases[i];
		
		float stepX = c.getWidth() + spacing * 2;
		float stepY = c.getHeight() + spacing * 2;
		
		float x = column * stepX;
		float y = line * stepY;
		
		Case temp = c;
		temp.setXY(x, y);
		temp.draw(target, states);*/
		m_cases[i].draw(win);
	}
}

bool Grid::isOver(int x, int y) const
{
	float width = m_column * (m_caseSize + m_spacing * 2);
	float height = m_line * (m_caseSize + m_spacing * 2);
	
	return (x >= m_x && x <= m_x + width && y >= m_y && y <= m_y + height);
}


std::ostream& operator<<(std::ostream& os, Grid const& src)
{
	src.write(os);
	
	return os;
}

std::istream& operator>>(std::istream& is, Grid& src)
{
	src.read(is);
	
	return is;
}


void Grid::recreateCases()
{
	m_cases.clear();
	m_cases = std::vector<Case>(m_line*m_column);
	
	for(unsigned int i=0;i<(m_line*m_column);++i)
	{
		unsigned int line = i / m_column;
		unsigned int column = i % m_column;
		
		float stepX = m_caseSize + m_spacing * 2;
		float stepY = m_caseSize + m_spacing * 2;
		
		float x = column * stepX + m_x;
		float y = line * stepY + m_y;
		
		m_cases[i] = Case(m_caseSize, m_caseSize, x, y, false);
	}
}


void Grid::write(std::ostream& os) const
{
	os << m_line << " " << m_column << " " << m_x << " " << m_y << " " << m_caseSize << " " << m_spacing << " " << m_toroidal << "\n";
	
	for(unsigned int i=0;i<(m_line*m_column);++i)
		os << m_cases[i] << ((i < ((m_line*m_column) - 1)) ? "\n" : "");
}

void Grid::read(std::istream& is)
{
	m_cases.clear();
	
	is >> m_line;
	is >> m_column;
	is >> m_x;
	is >> m_y;
	is >> m_caseSize;
	is >> m_spacing;
	is >> m_toroidal;
	
	for(unsigned int i=0;i<(m_line*m_column);++i)
	{
		std::string buff;
		
		std::getline(is, buff);
		
		Case c;
		
		is >> c;
		
		m_cases.push_back(c);
	}
}

bool Grid::isEqual(Grid const& src) const
{
	bool lineEqual = (m_line == src.m_line);
	bool columnEqual = (m_column == src.m_column);
	bool toroidalEqual = (m_toroidal == src.m_toroidal);
	bool casesEqual = true;
	
	for(size_t i=0;i<m_cases.size();++i)
		casesEqual = ((casesEqual) && (m_cases[i] == src.m_cases[i]));
	
	return lineEqual && columnEqual && toroidalEqual && casesEqual;
}