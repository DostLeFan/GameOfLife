#include "../include/Window.hpp"

#include <fstream>
#include <vector>
#include <stdexcept>
#include "../include/I18n.hpp"
#include "../include/Logger.hpp"

Window::Window() : m_window(sf::VideoMode({1024, 768}), "Game of Life"), m_gui(m_window), m_currentGrid(10, 10, 100, 100, 15, 1.f, true), m_isStarted(false), m_game(50)
{
	setupGui();

	m_fileService.setSaveCallback([this](std::string const& path) { saveGrid(path); });
	m_fileService.setLoadCallback([this](std::string const& path) { loadGrid(path); });

	I18n::getInstance().addListener([this](std::string const&) { updateTranslations(); });
	
	m_game.setGrid(m_currentGrid);
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
				m_window.close();
			}
			
			if(sf::Event::MouseButtonPressed const* mouseBtn = event->getIf<sf::Event::MouseButtonPressed>())
			{
				if(mouseBtn->button == sf::Mouse::Button::Left)
					onLeftClick(mouseBtn->position.x, mouseBtn->position.y);
			}
		}
		
		m_window.clear(sf::Color::White);
		m_gui.draw();
		m_currentGrid.draw(m_window);
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
	
	m_localeCombo->setSelectedItem(i18n.getCurrentLocale());
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
	m_currentGrid = Grid(10, 10, 100, 100, 15, 1.f, true);
	
	if(m_game.isActive())
		m_game.stop();
	
	m_game.setGrid(m_currentGrid);
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

void Window::onLeftClick(int x, int y)
{
	if(y < 40)
		return;
	
	if(m_currentGrid.isOver(x, y))
	{
		try
		{
			Case& c = m_currentGrid.getCaseFromPixel(x, y, 100, 100);
			
			c.setAlive(!c.isAlive());
		}
		catch(std::out_of_range const& err)
		{
			LOG_ERROR("Window : onLeftClick : ERROR : An error occured : {}.", err.what());
		}
	}
}