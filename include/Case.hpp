#ifndef DEF_CASE
#define DEF_CASE

#include <ostream>
#include <istream>
#include <SFML/Graphics.hpp>

class Case
{
	public:
		Case(unsigned int width = 0, unsigned int height = 0, int x = 0, int y = 0, bool isAlive = false);
		Case(Case const& src);
		
		Case& operator=(Case const& src);
		
		inline unsigned int getWidth() const { return m_width; }
		inline unsigned int getHeight() const { return m_height; }
		inline int getX() const { return m_x; }
		inline int getY() const { return m_y; }
		inline void setWidth(unsigned int width) { m_width = width; }
		inline void setHeight(unsigned int height) { m_height = height; }
		inline void setX(int x) { m_x = x; }
		inline void setY(int y) { m_y = y; }
		inline void setXY(int x, int y) { setX(x); setY(y); }
		inline void setAlive(bool isAlive) { m_isAlive = isAlive; }
		
		virtual void draw(sf::RenderWindow& win) const;
		inline bool isAlive() const { return m_isAlive; }
		
		inline friend bool operator==(Case const& lhs, Case const& rhs) { return lhs.isEqual(rhs); }
		inline friend bool operator!=(Case const& lhs, Case const& rhs) { return !lhs.isEqual(rhs); }
		friend std::ostream& operator<<(std::ostream& os, Case const& src);
		friend std::istream& operator>>(std::istream& is, Case& src);
	
	private:
		unsigned int m_width;
		unsigned int m_height;
		int m_x;
		int m_y;
		bool m_isAlive;
	
	protected:
		virtual void write(std::ostream& os) const;
		virtual void read(std::istream& is);
		virtual bool isEqual(Case const& src) const;
};

#define NULL_CASE Case(0, 0, -1, -1, false)

#endif // DEF_CASE