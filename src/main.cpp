#include "../include/Include.hpp"

#include <stdexcept>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>

int main()
{
	I18n& i18n = I18n::getInstance();
	i18n.load();
	i18n.setLocale("en");
	i18n.setFallback("en");
	
	Logger::init();
	
	LOG_INFO("Game of Life launched.");
	
	try
	{
		Window window;
		
		window.run();
	}
	catch(std::exception const& err)
	{
		std::cerr << err.what() << std::endl;
	}
	catch(...)
	{
		std::cerr << "Unknown error." << std::endl;
	}
	
	return 0;
}