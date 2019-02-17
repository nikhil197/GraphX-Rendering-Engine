#include "pch.h"
#include "FileOpenDialog.h"

#include <Windows.h>
#include <ShObjIdl.h>
#include <KnownFolders.h>
#include <shtypes.h>

namespace engine
{
	const COMDLG_FILTERSPEC FileOpenDialog::TextureFileTypes[] = {
		{L"PNG (*.png)", L"*.png"},
		{L"JPEG (*.jpg)", L"*.jpg"},
		{L"TGA (*.tga)", L"*.tga"}
	};

	const COMDLG_FILTERSPEC FileOpenDialog::ModelFileTypes[] = {
		{L"FBX (*.fbx)", L"*.fbx"},
		{L"OBJ (*.obj)", L"*.obj"}
	};

	FileOpenDialog::FileOpenDialog(ResourceType RSType)
		: m_FileDialog(nullptr), m_KnownFolderManager(nullptr), m_KnownFolder(nullptr), m_ShellItem(nullptr), m_ResultItem(nullptr)
	{
		m_Result = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, (void**)(&m_FileDialog));
		if (SUCCEEDED(m_Result))
		{
			// Add a default folder
			m_Result = CoCreateInstance(CLSID_KnownFolderManager, NULL, CLSCTX_ALL, IID_IKnownFolderManager, (void**)(&m_KnownFolderManager));
			if (SUCCEEDED(m_Result))
			{
				m_Result = m_KnownFolderManager->GetFolder(FOLDERID_PublicDocuments, &m_KnownFolder);
				if (SUCCEEDED(m_Result))
				{
					m_Result = m_KnownFolder->GetShellItem(0, IID_PPV_ARGS(&m_ShellItem));
					if (SUCCEEDED(m_Result))
					{
						m_FileDialog->AddPlace(m_ShellItem, FDAP_BOTTOM);
					}
				}
			}

			// Restrict the dialog to show only file system items
			DWORD OptionFlags;
			m_Result = m_FileDialog->GetOptions(&OptionFlags);
			if (SUCCEEDED(m_Result))
			{
				m_Result = m_FileDialog->SetOptions(OptionFlags | FOS_FORCEFILESYSTEM);
			}

			// Set the allowed File types
			if (RSType == ResourceType::TEXTURES)
			{
				m_Result = m_FileDialog->SetFileTypes(ARRAYSIZE(TextureFileTypes), TextureFileTypes);
				if (SUCCEEDED(m_Result))
				{
					// Set the default extention
					m_FileDialog->SetDefaultExtension(L"PNG");
				}
			}
			else if (RSType == ResourceType::MODELS)
			{
				m_Result = m_FileDialog->SetFileTypes(ARRAYSIZE(ModelFileTypes), ModelFileTypes);
				if (SUCCEEDED(m_Result))
				{
					// Set the default extension
					m_FileDialog->SetDefaultExtension(L"FBX");
				}
			}
		}
	}

	void FileOpenDialog::Show()
	{
		if (m_FileDialog != nullptr)
		{
			// Show the dialog box
			m_Result = m_FileDialog->Show(NULL);
			if (SUCCEEDED(m_Result))
			{
				// Get the result
				m_Result = m_FileDialog->GetResult(&m_ResultItem);
				if (SUCCEEDED(m_Result))
				{
					// Relative file path
					wchar_t* FilePath = nullptr;
					m_Result = m_ResultItem->GetDisplayName(SIGDN_NORMALDISPLAY, &FilePath);
					if (SUCCEEDED(m_Result))
					{
						m_RelativePath = FilePath;
					}

					// Absolute file path
					FilePath = nullptr;
					m_Result = m_ResultItem->GetDisplayName(SIGDN_FILESYSPATH, &FilePath);
					if (SUCCEEDED(m_Result))
					{
						m_SystemPath = FilePath;
					}
				}
			}
		}
	}

	FileOpenDialog::~FileOpenDialog()
	{
		if (m_FileDialog != nullptr)
		{
			if (m_KnownFolderManager != nullptr)
			{
				if (m_KnownFolder != nullptr)
				{
					if (m_ShellItem != nullptr)
					{
						m_ShellItem->Release();
					}

					m_KnownFolder->Release();
				}

				m_KnownFolderManager->Release();
			}

			if (m_ResultItem != nullptr)
			{
				m_ResultItem->Release();
			}

			m_FileDialog->Release();
		}
	}

}