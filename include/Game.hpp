#ifndef DEF_GAME
#define DEF_GAME

#include "RegularTask.hpp"
#include "Grid.hpp"

class Game : public RegularTask
{
	public:
		Game(lluint delay);
		Game(Game const& src) = delete;
		~Game();
		
		inline void setGrid(Grid& grid) { m_grid = &grid; }
		
		void nextState();
	
	private:
		Grid* m_grid;
	
	protected:
		virtual void process() override;
};

#endif // DEF_GAME