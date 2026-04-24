#include "../include/Window.hpp"

#include <fstream>
#include <vector>
#include <stdexcept>
#include "../include/I18n.hpp"
#include "../include/Logger.hpp"

Window::Window() : m_window(sf::VideoMode({1024, 768}), "Game of Life"), m_gui(m_window), m_currentGrid(150, 150, 0, 0, 15, 1.f, true), m_isStarted(false), m_game(100), m_offsetX(50), m_needsRedraw(false)
{
	setupGui();

	m_fileService.setSaveCallback([this](std::string const& path) { saveGrid(path); });
	m_fileService.setLoadCallback([this](std::string const& path) { loadGrid(path); });
	
	I18n::getInstance().addListener([this](std::string const&) { updateTranslations(); });
	
	m_game.setGrid(m_currentGrid);
	m_game.setOnStepCallback([this]() { m_needsRedraw.store(true); });
	
	updateLayout();
}


void Window::run()
{
	while(m_window.isOpen())
	{
		while(std::optional const event = m_window.pollEvent())
		{
			handleShortcuts(*event);
			m_gui.handleEvent(*event);
			
			if (event->is<sf::Event::Closed>())
			{
				//m_autoSaver.stop();
				if(m_game.isActive())
					m_game.stop();
				
				m_window.close();
			}
			else if(sf::Event::Resized const* resized = event->getIf<sf::Event::Resized>())
			{
				sf::View view(
					sf::FloatRect(
						{ 0.f, 0.f },
						{ static_cast<float>(resized->size.x),
						static_cast<float>(resized->size.y) }
					)
				);
				
				m_window.setView(view);
				
				updateLayout();
			}
			
			if(sf::Event::MouseButtonPressed const* mouseBtn = event->getIf<sf::Event::MouseButtonPressed>())
			{
				if(mouseBtn->button == sf::Mouse::Button::Left)
				{
					sf::Vector2f mousePos(mouseBtn->position.x, mouseBtn->position.y);
					sf::Vector2f panelPos = m_scrollPanel->getPosition();
					sf::Vector2f panelSize = m_scrollPanel->getSize();
					float scrollbarSize = m_scrollPanel->getVerticalScrollbar()->getWidth();
					
					bool inPanel = (mousePos.x >= panelPos.x && mousePos.x < panelPos.x + panelSize.x && mousePos.y >= panelPos.y && mousePos.y < panelPos.y + panelSize.y);
					bool onVerticalScrollbar = (mousePos.x >= panelPos.x + panelSize.x - scrollbarSize);
					bool onHorizontalScrollbar = (mousePos.y >= panelPos.y + panelSize.y - scrollbarSize);
					
					if(inPanel && !onVerticalScrollbar && !onHorizontalScrollbar)
						onLeftClick(mouseBtn->position.x, mouseBtn->position.y);
				}
			}
		}
		
		if(m_needsRedraw.exchange(false))
		{
			redrawCanvas();
			updateLayout();
		}
		
		m_window.clear(sf::Color::White);
		m_gui.draw();
		//m_currentGrid.draw(m_window);
		m_window.display();
	}
}


void Window::setupGui()
{
	// Tool bar (at the top).
	auto toolbar = tgui::Panel::create({{"100%", 40}});
	toolbar->setSize(1024, 40);
	toolbar->setPosition({0, 0});
	toolbar->getRenderer()->setBackgroundColor(sf::Color(50, 50, 50));
	toolbar->setWidgetName("Toolbar");
	
	I18n& i18n = I18n::getInstance();
	
	// Button "New".
	m_newBtn = tgui::Button::create(i18n.t("toolbar.new"));
	m_newBtn->setPosition({10, 5});
	m_newBtn->onPress([this] { onNewGrid(); });
	toolbar->add(m_newBtn);
	
	// Button "Load".
	m_loadBtn = tgui::Button::create(i18n.t("toolbar.load"));
	m_loadBtn->setPosition({100, 5});
	m_loadBtn->onPress([this] { onLoadGrid(); });
	toolbar->add(m_loadBtn);
	
	// Button "Save".
	m_saveBtn = tgui::Button::create(i18n.t("toolbar.save"));
	m_saveBtn->setPosition({190, 5});
	m_saveBtn->onPress([this] { onSaveGrid(); });
	toolbar->add(m_saveBtn);
	
	// Button "Save As".
	m_saveAsBtn = tgui::Button::create(i18n.t("toolbar.saveAs"));
	m_saveAsBtn->setPosition({ 300, 5 });
	m_saveAsBtn->onPress([this] { onSaveAsGrid(); });
	toolbar->add(m_saveAsBtn);
	
	// Button "Next step".
	m_nextStepBtn = tgui::Button::create(i18n.t("toolbar.nextStep"));
	m_nextStepBtn->setPosition({ 450, 5 });
	m_nextStepBtn->onPress([this] { onNextStep(); });
	toolbar->add(m_nextStepBtn);
	
	// Button "Start" and "Stop".
	m_startStopBtn = tgui::Button::create(i18n.t("toolbar.start"));
	m_startStopBtn->setPosition({ 550, 5 });
	m_startStopBtn->onPress([this] { onStartStop(); });
	toolbar->add(m_startStopBtn);
	
	// Locales combo.
	std::vector<std::string> locales = i18n.getAvailableLocales();
	
	m_localeCombo = tgui::ComboBox::create();
	m_localeCombo->setPosition({ 680, 5 });
	m_localeCombo->setSize({ 120, 30 });
	
	for(std::string& loc : locales)
		m_localeCombo->addItem(loc);
	
	m_localeCombo->setSelectedItem(i18n.getCurrentLocale());
	m_localeCombo->onItemSelect([this](tgui::String const& value) { onLocaleChanged(value.toStdString()); });
	toolbar->add(m_localeCombo);
	
	m_gui.add(toolbar);
	
	// Step label.
	m_stepLabel = tgui::Label::create(i18n.t("toolbar.step") + " : " + std::to_string(m_game.getStepCount()));
	m_stepLabel->setPosition({ 820, 5 });
	m_stepLabel->getRenderer()->setTextColor(tgui::Color::White);
	toolbar->add(m_stepLabel);
	
	m_scrollPanel = tgui::ScrollablePanel::create();
	m_scrollPanel->setWidgetName("ScrollPanel");
	m_scrollPanel->getRenderer()->setBackgroundColor(sf::Color(200, 200, 200));
	m_gui.add(m_scrollPanel);
	
	m_gridCanvas = tgui::CanvasSFML::create();
	m_scrollPanel->add(m_gridCanvas);
	redrawCanvas();
}

void Window::updateLayout()
{
	sf::Vector2u winSize = m_window.getSize();
	float winWidth = static_cast<float>(winSize.x);
	float winHeight = static_cast<float>(winSize.y);
	
	float toolbarHeight = winHeight * 0.06f;
	float leftSidebarWidth = winWidth * 0.15f;
	float rightSidebarWidth = winWidth * 0.15f;
	
	auto toolbar = m_gui.get<tgui::Panel>("Toolbar");
	
	if(toolbar)
		toolbar->setSize({winWidth, toolbarHeight});
	
	float panelX = leftSidebarWidth;
	float panelY = toolbarHeight;
	float panelWidth = winWidth - leftSidebarWidth - rightSidebarWidth;
	float panelHeight = winHeight - toolbarHeight;
	
	if(m_scrollPanel)
	{
		m_scrollPanel->setPosition({panelX, panelY});
		m_scrollPanel->setSize({panelWidth, panelHeight});
	}
	
	float caseStep = m_currentGrid.getCaseSize() + m_currentGrid.getSpacing() * 2;
	float gridRealWidth = m_currentGrid.getColumn() * caseStep;
	float gridRealHeight = m_currentGrid.getLine() * caseStep;
	
	if(m_gridCanvas)
	{
		m_gridCanvas->setSize({gridRealWidth, gridRealHeight});
		
		float offsetX = m_offsetX;
		float offsetY = 0.f;
		
		if(gridRealWidth < panelWidth)
			offsetX = (panelWidth - gridRealWidth) / 2.f + m_offsetX;
		
		if(gridRealHeight < panelHeight)
			offsetY = (panelHeight - gridRealHeight) / 2.f;
		
		m_gridCanvas->setPosition({offsetX, offsetY});
	}
	
	redrawCanvas();
}

void Window::updateTranslations()
{
	I18n& i18n = I18n::getInstance();
	
	m_newBtn->setText(i18n.t("toolbar.new"));
	m_loadBtn->setText(i18n.t("toolbar.load"));
	m_saveBtn->setText(i18n.t("toolbar.save"));
	m_saveAsBtn->setText(i18n.t("toolbar.saveAs"));
	m_nextStepBtn->setText(i18n.t("toolbar.nextStep"));
	m_startStopBtn->setText(i18n.t((m_isStarted ? "toolbar.stop" : "toolbar.start")));
	m_stepLabel->setText(i18n.t("toolbar.step") + " : " + std::to_string(m_game.getStepCount()));
	
	m_localeCombo->setSelectedItem(i18n.getCurrentLocale());
}

void Window::redrawCanvas()
{
	if(m_gridCanvas)
	{
		std::lock_guard<std::mutex> lock(m_game.getGridMutex());
		m_gridCanvas->clear(sf::Color::White);
		m_currentGrid.draw(m_gridCanvas->getRenderTexture());
		m_gridCanvas->display();
		m_stepLabel->setText(I18n::getInstance().t("toolbar.step") + " : " + std::to_string(m_game.getStepCount()));
	}
}

void Window::saveGrid(std::string const& gridName)
{
	std::ofstream ofs(gridName);
	
	if(!ofs.is_open())
	{
		LOG_ERROR("Window : saveGrid : ERROR : An error occured : Cannot open file \"{}\".", gridName);
		
		return;
	}
	
	ofs << m_currentGrid;
	
	ofs.close();
	
	LOG_INFO("Grid saved into \"{}\".", gridName);
}

void Window::loadGrid(std::string const& gridName)
{
	std::ifstream ifs(gridName);
	
	if(!ifs.is_open())
	{
		LOG_ERROR("Window : loadGrid : ERROR : An error occured : Cannot open file \"{}\".", gridName);
		
		return;
	}
	
	ifs >> m_currentGrid;
	
	if(m_game.isActive())
		m_game.stop();
	
	m_game.setGrid(m_currentGrid);
	
	LOG_INFO("Grid loaded from \"{}\"", gridName);
}

void Window::newGrid()
{
	m_currentGrid = Grid(150, 150, 0, 0, 15, 1.f, true);
	
	if(m_game.isActive())
		m_game.stop();
	
	m_game.setGrid(m_currentGrid);
	
	updateLayout();
	redrawCanvas();
}


void Window::handleShortcuts(sf::Event const& event)
{
	if (!event.is<sf::Event::KeyPressed>())
		return;

	sf::Event::KeyPressed const* key = event.getIf<sf::Event::KeyPressed>();

	if (!key)
		return;

	if (key->code == sf::Keyboard::Key::S)
	{
		if (key->control && key->alt)
			m_fileService.saveAs();
		else if (key->control)
			m_fileService.save();
	}
	else if (key->control && key->code == sf::Keyboard::Key::O)
		m_fileService.open();
	else if (key->control && key->code == sf::Keyboard::Key::N)
		onNewGrid();
}


void Window::onNewGrid()
{
	m_fileService.newFile();
	newGrid();
}

void Window::onSaveGrid()
{
	m_fileService.save();
}

void Window::onSaveAsGrid()
{
	m_fileService.saveAs();
}

void Window::onLoadGrid()
{
	m_fileService.open();
}

void Window::onNextStep()
{
	m_game.nextState();
	
	updateLayout();
}

void Window::onStartStop()
{
	if(m_isStarted)
		m_game.stop();
	else
		m_game.start();
	
	m_isStarted = !m_isStarted;
	m_startStopBtn->setText(I18n::getInstance().t((m_isStarted ? "toolbar.stop" : "toolbar.start")));
}

void Window::onLocaleChanged(std::string const& locale)
{
	I18n::getInstance().setLocale(locale);
}

void Window::onLeftClick(float x, float y)
{
	sf::Vector2f mousePos(x, y);
	sf::Vector2f panelPos = m_scrollPanel->getPosition();
	sf::Vector2f canvasPos = m_gridCanvas->getPosition();
	sf::Vector2f scrollOffset(
		m_scrollPanel->getHorizontalScrollbar()->getValue(),
		m_scrollPanel->getVerticalScrollbar()->getValue()
	);
	sf::Vector2f localPos = mousePos - panelPos - canvasPos + scrollOffset;
	
	if(localPos.x >= 0 && localPos.x < m_gridCanvas->getSize().x && localPos.y >= 0 && localPos.y < m_gridCanvas->getSize().y)
	{
		float step = m_currentGrid.getCaseSize() + m_currentGrid.getSpacing() * 2;
		int col = static_cast<int>(localPos.x / step);
		int row = static_cast<int>(localPos.y / step);
		
		if(col >= 0 && col < static_cast<int>(m_currentGrid.getColumn()) && row >= 0 && row < static_cast<int>(m_currentGrid.getLine()))
		{
			{
				std::lock_guard<std::mutex> lock(m_game.getGridMutex());
				Case& c = m_currentGrid(row, col);
				c.setAlive(!c.isAlive());
			}
			
			redrawCanvas();
		}
	}
}