#ifndef DEF_GRID
#define DEF_GRID

#include <vector>
#include "Case.hpp"

class Grid
{
	public:
		Grid(unsigned int line, unsigned int col, int x, int y, unsigned int caseSize, float spacing = 1.f, bool toroidal = true);
		Grid(Grid const& src);
		
		Grid& operator=(Grid const& src);
		Case& operator()(size_t line, size_t column);
		Case const& operator()(size_t line, size_t column) const;
		
		inline unsigned int getLine() const { return m_line; }
		inline unsigned int getColumn() const { return m_column; }
		inline int getX() const { return m_x; }
		inline int getY() const { return m_y; }
		inline unsigned int getCaseSize() const { return m_caseSize; }
		inline float getSpacing() const { return m_spacing; }
		std::vector<Case> getNeighbors(size_t i, size_t j) const;
		size_t getCaseIndexFromPixel(int x, int y, int offsetX = 0, int offsetY = 0);
		inline Case& getCaseFromPixel(int x, int y, int offsetX = 0, int offsetY = 0) { return m_cases[getCaseIndexFromPixel(x, y, offsetX, offsetY)]; }
		inline void setLine(unsigned int line) { m_line = line; recreateCases(); }
		inline void setColumn(unsigned int column) { m_column = column; recreateCases(); }
		inline void setX(int x) { m_x = x; recreateCases(); }
		inline void setY(int y) { m_y = y; recreateCases(); }
		inline void setXY(int x, int y) { setX(x); setY(y); }
		inline void setCaseSize(unsigned int caseSize) { m_caseSize = caseSize; recreateCases(); }
		inline void setSpacing(float spacing) { m_spacing = spacing; }
		inline void setToroidal(bool toroidal) { m_toroidal = toroidal; }
		
		virtual void draw(sf::RenderWindow& win) const;
		inline bool isToroidal() const { return m_toroidal; }
		bool isOver(int x, int y) const;
		
		inline friend bool operator==(Grid const& lhs, Grid const& rhs) { return lhs.isEqual(rhs); }
		inline friend bool operator!=(Grid const& lhs, Grid const& rhs) { return !lhs.isEqual(rhs); }
		friend std::ostream& operator<<(std::ostream& os, Grid const& src);
		friend std::istream& operator>>(std::istream& is, Grid& src);
	
	private:
		unsigned int m_line;
		unsigned int m_column;
		int m_x;
		int m_y;
		unsigned int m_caseSize;
		float m_spacing; // Spacing between cases for grid drawing.
		bool m_toroidal;
		std::vector<Case> m_cases;
		
		void recreateCases();
	
	protected:
		virtual void write(std::ostream& os) const;
		virtual void read(std::istream& is);
		virtual bool isEqual(Grid const& src) const;
};

#endif // DEF_GRID