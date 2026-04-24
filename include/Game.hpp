#ifndef DEF_GAME
#define DEF_GAME

#include <functional>
#include "RegularTask.hpp"
#include "Grid.hpp"

class Game : public RegularTask
{
	public:
		Game(lluint delay);
		Game(Game const& src) = delete;
		~Game();
		
		inline lluint getStepCount() const { return m_stepCount; }
		inline std::mutex& getGridMutex() { return m_gridMutex; }
		inline void setGrid(Grid& grid) { m_grid = &grid; m_stepCount = 0; }
		inline void setOnStepCallback(std::function<void()> cb) { m_onStep = cb; }
		
		void nextState();
	
	private:
		Grid* m_grid;
		lluint m_stepCount;
		std::function<void()> m_onStep;
		std::mutex m_gridMutex;
	
	protected:
		virtual void process() override;
};

#endif // DEF_GAME