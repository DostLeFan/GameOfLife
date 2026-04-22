#ifndef DEF_I18N
#define DEF_I18N

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <filesystem>

class I18n
{
	public:
		using TranslationMap = std::unordered_map<std::string, std::string>;
		using LocaleChangedCallback = std::function<void(std::string const&)>;

		static I18n& getInstance();

		std::vector<std::string> getAvailableLocales() const;
		inline std::string const& getCurrentLocale() const { return m_current; }
		void setLocale(std::string const& locale);
		inline void setFallback(std::string const& locale) { m_fallback = locale; }

		void load(std::filesystem::path const& localesDir = "locales");
		std::string const& t(std::string const& key) const;
		std::string t(std::string const& key, std::unordered_map<std::string, std::string> const& vars) const;
		inline void addListener(LocaleChangedCallback cb) { m_listeners.push_back(cb); }

	private:
		std::string m_current;
		std::string m_fallback;
		std::unordered_map<std::string, TranslationMap> m_translations;
		std::vector<LocaleChangedCallback> m_listeners;

		I18n() = default;

		void loadFile(std::string const& locale, std::filesystem::path const& path);
		void notifyListeners();
};

#endif // DEF_I18N