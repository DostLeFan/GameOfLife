#ifndef DEF_FILESERVICE
#define DEF_FILESERVICE

#define TINYFD_WIN32_ONLY
#include <string>
#include <functional>

class FileService
{
	public:
		using SaveCallback = std::function<void(std::string const&)>;
		using LoadCallback = std::function<void(std::string const&)>;
		using CancelCallback = std::function<void(std::string const&)>;

		inline std::string const& getCurrentFile() const { return m_currentFile; }
		inline std::vector<std::string> const& getRecentFiles() const { return m_recentFiles; }

		inline void setSaveCallback(SaveCallback cb) { m_saveCallback = cb; }
		inline void setLoadCallback(LoadCallback cb) { m_loadCallback = cb; }
		inline void setCancelCallback(CancelCallback cb) { m_cancelCallback = cb; }

		inline bool isDirty() const { return m_dirty; }
		inline void markDirty(bool dirty) { m_dirty = dirty; }

		void newFile();
		void open();
		void save();
		void saveAs();

	private:
		std::string m_currentFile;
		std::string m_lastDirectory;
		std::vector<std::string> m_recentFiles;
		bool m_dirty;
		SaveCallback m_saveCallback;
		LoadCallback m_loadCallback;
		CancelCallback m_cancelCallback;

		bool confirmDiscardChanges();
		std::string openDialog();
		std::string saveDialog();
		void addRecentFile(std::string const& path);
};

#endif // DEF_FILESERVICE