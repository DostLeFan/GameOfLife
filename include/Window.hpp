#ifndef DEF_WINDOW
#define DEF_WINDOW

#include <string>
#include "Grid.hpp"
#include "Game.hpp"
#include "FileService.hpp"
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>

class Window
{
	public:
		Window();
		
		void run();
	
	private:
		sf::RenderWindow m_window;
		tgui::Gui m_gui;
		Grid m_currentGrid;
		bool m_isStarted;
		Game m_game;
		tgui::Button::Ptr m_newBtn;
		tgui::Button::Ptr m_loadBtn;
		tgui::Button::Ptr m_saveBtn;
		tgui::Button::Ptr m_saveAsBtn;
		tgui::Button::Ptr m_nextStepBtn;
		tgui::Button::Ptr m_startStopBtn;
		tgui::ComboBox::Ptr m_localeCombo;
		FileService m_fileService;
		
		void setupGui();
		void updateTranslations();
		void saveGrid(std::string const& gridName);
		void loadGrid(std::string const& gridName);
		void newGrid();
		inline void saveUnsavedGrid(std::string const& randomName) { saveGrid("unsaved_" + randomName + ".bin"); }
		
		void handleShortcuts(sf::Event const& event);
		
		void onNewGrid();
		void onSaveGrid();
		void onSaveAsGrid();
		void onLoadGrid();
		void onNextStep();
		void onStartStop();
		void onLocaleChanged(std::string const& locale);
		void onLeftClick(int x, int y);
};

#endif // DEF_WINDOW