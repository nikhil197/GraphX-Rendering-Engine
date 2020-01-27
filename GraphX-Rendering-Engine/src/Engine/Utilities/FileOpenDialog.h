#pragma once

struct IFileOpenDialog;
struct IKnownFolderManager;
struct IKnownFolder;
struct IShellItem;

typedef struct _COMDLG_FILTERSPEC COMDLG_FILTERSPEC;

namespace GraphX
{
	/* The type of resource to be opened */
	enum ResourceType
	{
		TEXTURES = 0,
		MODELS
	};

	/* Dialog box to select a file from the windows explorer */
	class FileOpenDialog
	{
	public:
		/* RSType is the type of resources the dialog will be used to open */
		FileOpenDialog(ResourceType RSType);

		/* Display the dialog box to the user */
		void Show();

		/* Returns the relative path of the selected file */
		const std::wstring& GetRelativePath() const { return m_RelativePath; }

		/* Returns the system path of the selected file */
		const std::wstring& GetAbsolutePath() const { return m_SystemPath; }

		~FileOpenDialog();

	private:
		/* File types allowed for textures */
		static const COMDLG_FILTERSPEC TextureFileTypes[];

		/* File types allowed for models */
		static const COMDLG_FILTERSPEC ModelFileTypes[];

	private:
		/* Dialog box object */
		IFileOpenDialog* m_FileDialog;

		/* To open a known folder when the dialog is opened */
		IKnownFolderManager* m_KnownFolderManager;

		/* Known folder to open */
		IKnownFolder* m_KnownFolder;

		/* Shell Item to represent the location */
		IShellItem* m_ShellItem;

		/* Result obtained after user clicks open */
		IShellItem* m_ResultItem;

		/* Result of the Win API operation */
		HRESULT m_Result;

		/* Path of the file relative to the parent folder */
		std::wstring m_RelativePath;

		/* System file Path */
		std::wstring m_SystemPath;
	};
}