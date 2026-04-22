#include "../include/FileService.hpp"

#include <algorithm>
#include <tinyfiledialogs.h>
#include "../include/I18n.hpp"

void FileService::newFile()
{
	if (!confirmDiscardChanges())
		return;

	m_currentFile.clear();
	m_dirty = false;
}

void FileService::open()
{
	if (!confirmDiscardChanges())
		return;

	std::string path = openDialog();

	if (path.empty())
	{
		if (m_cancelCallback)
			m_cancelCallback("Cancel requested");

		return;
	}

	if (m_loadCallback)
		m_loadCallback(path);

	m_currentFile = path;
	m_dirty = false;

	addRecentFile(path);
}

void FileService::save()
{
	if (m_currentFile.empty())
	{
		saveAs();

		return;
	}

	if (m_saveCallback)
		m_saveCallback(m_currentFile);

	m_dirty = false;
}

void FileService::saveAs()
{
	std::string path = saveDialog();

	if (path.empty())
	{
		if (m_cancelCallback)
			m_cancelCallback("Cancel requested");

		return;
	}

	if (m_saveCallback)
		m_saveCallback(path);

	m_currentFile = path;
	m_dirty = false;

	addRecentFile(path);
}


bool FileService::confirmDiscardChanges()
{
	if (!m_dirty)
		return true;

	I18n& i18n = I18n::getInstance();

	int result = tinyfd_messageBox(
		i18n.t("warnbox.confirmDiscardChange.title").c_str(),
		i18n.t("warnbox.confirmDiscardChange.message").c_str(),
		"yesno",
		"warning",
		0
	);

	return result == 1;
}

std::string FileService::openDialog()
{
	char const* filters[] = { "*.bin" };

	I18n& i18n = I18n::getInstance();

	char const* result = tinyfd_openFileDialog(
		i18n.t("filedialog.openDialog.title").c_str(),
		m_lastDirectory.empty() ? "" : m_lastDirectory.c_str(),
		1,
		filters,
		i18n.t("filedialog.openDialog.singleFilterDescription").c_str(),
		0
	);

	if (!result)
		return "";

	std::string path = result;
	m_lastDirectory = path.substr(0, path.find_last_of("/\\"));

	return path;
}

std::string FileService::saveDialog()
{
	char const* filters[] = { "*.bin" };

	I18n& i18n = I18n::getInstance();

	char const* result = tinyfd_saveFileDialog(
		i18n.t("filedialog.saveDialog.title").c_str(),
		m_lastDirectory.empty() ? "map.bin" : m_lastDirectory.c_str(),
		1,
		filters,
		i18n.t("filedialog.saveDialog.singleFilterDescription").c_str()
	);

	if (!result)
		return "";

	std::string path = result;

	if (path.find(".bin") == std::string::npos)
		path += ".bin";

	m_lastDirectory = path.substr(0, path.find_last_of("/\\"));

	return path;
}

void FileService::addRecentFile(std::string const& path)
{
	m_recentFiles.erase(
		std::remove(
			m_recentFiles.begin(),
			m_recentFiles.end(),
			path
		),
		m_recentFiles.end()
	);

	m_recentFiles.insert(m_recentFiles.begin(), path);

	if (m_recentFiles.size() > 10)
		m_recentFiles.pop_back();
}