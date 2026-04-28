#ifndef DEF_WINDOW
#define DEF_WINDOW

#include <string>
#include <utility>
#include "Grid.hpp"
#include "Game.hpp"
#include "FileService.hpp"
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/Backend/Renderer/SFML-Graphics/CanvasSFML.hpp>

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
		tgui::Label::Ptr m_stepLabel;
		FileService m_fileService;
		tgui::ScrollablePanel::Ptr m_scrollPanel;
		tgui::CanvasSFML::Ptr m_gridCanvas;
		sf::Vector2f m_gridCanvasSize;
		float m_offsetX;
		std::atomic<bool> m_needsRedraw;
		bool m_isDragging;
		bool m_dragSetAlive;
		
		void setupGui();
		void updateLayout();
		void updateTranslations();
		void redrawCanvas();
		void saveGrid(std::string const& gridName);
		void loadGrid(std::string const& gridName);
		void newGrid();
		inline void saveUnsavedGrid(std::string const& randomName) { saveGrid("unsaved_" + randomName + ".bin"); }
		
		bool isMouseOnGrid(sf::Vector2f const& mousePos) const;
		std::pair<int, int> getCellFromMouse(sf::Vector2f const& mousePos);
		
		void handleShortcuts(sf::Event const& event);
		
		void onNewGrid();
		void onSaveGrid();
		void onSaveAsGrid();
		void onLoadGrid();
		void onNextStep();
		void onStartStop();
		void onLocaleChanged(std::string const& locale);
		void onLeftButtonPressed(sf::Vector2f mousePos);
		void onLeftButtonReleased(sf::Vector2f mousePos);
		void onMouseMoved(sf::Vector2f mousePos);
};

#endif // DEF_WINDOW