#include "../include/I18n.hpp"

#include <fstream>
#include <nlohmann/json.hpp>
#include <stdexcept>

I18n& I18n::getInstance()
{
	static I18n instance;
	return instance;
}


std::vector<std::string> I18n::getAvailableLocales() const
{
	std::vector<std::string> locales;

	locales.reserve(m_translations.size());

	for (auto const& [locale, _] : m_translations)
		locales.push_back(locale);

	return locales;
}

void I18n::setLocale(std::string const& locale)
{
	if (m_translations.find(locale) == m_translations.end())
		throw std::runtime_error("ERROR : Locale not loaded : " + locale);

	m_current = locale;

	notifyListeners();
}


void I18n::load(std::filesystem::path const& localesDir)
{
	if (!std::filesystem::is_directory(localesDir))
		throw std::runtime_error("Unable to find directory : " + localesDir.string());

	for (std::filesystem::directory_entry const& entry : std::filesystem::directory_iterator(localesDir))
	{
		std::filesystem::path path = entry.path();

		if (!entry.is_regular_file() || path.extension() != ".json")
			continue;

		std::string const locale = path.stem().string();

		loadFile(locale, path);
	}
}

std::string const& I18n::t(std::string const& key) const
{
	if (auto it = m_translations.find(m_current); it != m_translations.end())
	{
		if (auto k = it->second.find(key); k != it->second.end())
			return k->second;
	}

	// Fallback
	if (!m_fallback.empty() && m_fallback != m_current)
	{
		if (auto it = m_translations.find(m_fallback); it != m_translations.end())
		{
			if (auto k = it->second.find(key); k != it->second.end())
				return k->second;
		}
	}

	return key; // Retourne la clé si rien trouvé
}

std::string I18n::t(std::string const& key, std::unordered_map<std::string, std::string> const& vars) const
{
	std::string result = t(key);

	for (auto const& [k, v] : vars)
	{
		std::string const token = "{" + k + "}";

		for (std::size_t pos = 0; (pos = result.find(token, pos)) != std::string::npos; )
			result.replace(pos, token.size(), v);
	}

	return result;
}


void I18n::loadFile(std::string const& locale, std::filesystem::path const& path)
{
	std::ifstream file(path);

	if (!file)
		throw std::runtime_error("Impossible d'ouvrir : " + path.string());

	auto const json = nlohmann::json::parse(file, nullptr, /*exceptions=*/true);

	if (!json.is_object())
		throw std::runtime_error("Format invalide (objet JSON attendu) : " + path.string());

	TranslationMap map;

	for (auto const& [key, val] : json.items()) {
		if (!val.is_string())
			throw std::runtime_error("Valeur non-string pour la clé \"" + key + "\" dans " + path.string());

		map.emplace(key, val.get<std::string>());
	}

	m_translations.emplace(locale, std::move(map));
}

void I18n::notifyListeners()
{
	for (size_t i = 0; i < m_listeners.size(); ++i)
		m_listeners[i](m_current);
}