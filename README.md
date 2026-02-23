# How to use
- Choose or drag&drop the files you want to compress
- Select quality 
- Press compress - the new compress files will be placed alongside the old ones with the postfix "_compressed.pdf"

## How do build portable

To make the application fully portable, you need to bundle the compiled .exe with all the Qt DLLs, compiler runtime libraries, and your Ghostscript folder into one single directory. Qt provides a built-in tool called windeployqt specifically for this.

Here is the step-by-step process to package your portable app:

1. Build in Release Mode
Open your project in Qt Creator (or your chosen IDE) and switch your build configuration from Debug to Release. Build the project. This generates an optimized, smaller executable without debug symbols.

2. Create a Deployment Folder
Create a new, empty folder anywhere on your computer (e.g., on your Desktop, named PDFCompressorPortable).

Locate the Release build folder that Qt generated.

Copy your compiled executable (e.g., PDFCompressor.exe) from the Release folder into your new PDFCompressorPortable folder.

3. Run the windeployqt Tool
This tool analyzes your executable and automatically copies all the necessary Qt frameworks, plugins (like platforms/qwindows.dll), and C++ runtime libraries into your folder.

Open the Qt Command Prompt (search for "Qt" in your Windows Start menu—it usually says something like Qt 6.x.x (MinGW/MSVC)).

Navigate to your deployment folder in the command prompt:

DOS
cd path\to\your\Desktop\PDFCompressorPortable
Run the windeployqt command on your executable. Adding the --compiler-runtime flag ensures it grabs the necessary C++ libraries so it works on PCs that don't have Visual Studio installed:

DOS
windeployqt --compiler-runtime PDFCompressor.exe
4. Add Ghostscript
Because your application relies on Ghostscript to process the PDFs, you need to include it in the portable folder structure exactly as your code expects.

Inside your PDFCompressorPortable folder, create a new folder named ghostscript.

Place the gswin64c.exe (and any DLLs Ghostscript requires, like gsdll64.dll) inside that ghostscript folder.
