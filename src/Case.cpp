#include "../include/Case.hpp"

Case::Case(unsigned int width, unsigned int height, int x, int y, bool isAlive) : m_width(width), m_height(height), m_x(x), m_y(y), m_isAlive(isAlive) {}

Case::Case(Case const& src) : m_width(src.m_width), m_height(src.m_height), m_x(src.m_x), m_y(src.m_y), m_isAlive(src.m_isAlive) {}


Case& Case::operator=(Case const& src)
{
	m_width = src.m_width;
	m_height = src.m_height;
	m_x = src.m_x;
	m_y = src.m_y;
	m_isAlive = src.m_isAlive;
	
	return *this;
}


void Case::draw(sf::RenderWindow& win) const
{
	float border = 1.f;
	
	sf::RectangleShape borderRect(sf::Vector2f(m_width + border * 2, m_height + border * 2));
	borderRect.setPosition({ m_x - border, m_y - border });
	borderRect.setFillColor(sf::Color::Black);
	
	win.draw(borderRect);
	
	sf::RectangleShape rect({ static_cast<float>(m_width), static_cast<float>(m_height) });
	rect.setPosition({ static_cast<float>(m_x), static_cast<float>(m_y) });
	rect.setFillColor(m_isAlive ? sf::Color::Black : sf::Color::White);
	
	win.draw(rect);
}


std::ostream& operator<<(std::ostream& os, Case const& src)
{
	src.write(os);
	
	return os;
}

std::istream& operator>>(std::istream& is, Case& src)
{
	src.read(is);
	
	return is;
}


void Case::write(std::ostream& os) const
{
	os << m_width << " " << m_height << " " << m_x << " " << m_y << " " << m_isAlive;
}

void Case::read(std::istream& is)
{
	is >> m_width;
	is >> m_height;
	is >> m_x;
	is >> m_y;
	is >> m_isAlive;
}

bool Case::isEqual(Case const& src) const
{
	bool widthEqual = (m_width == src.m_width);
	bool heightEqual = (m_height == src.m_height);
	bool xEqual = (m_x == src.m_x);
	bool yEqual = (m_y == src.m_y);
	bool isAliveEqual = (m_isAlive == src.m_isAlive);
	
	return widthEqual && heightEqual && xEqual && yEqual && isAliveEqual;
}