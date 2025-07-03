# Huffman Text Compressor/Decompressor with Web Interface 

## **Project Overview**

I made this project to compress large documents using a Huffman coding algorithm for text compression and decompression. It uses a C++ backend that handles the core compression logic, and a frontend built with Python Flask.

The Huffman coding algorithm is a widely used lossless data compression technique. This project shows its practical use for compressing and decompressing various text-based files, including plain text (.txt) and PDF documents (treated as binary streams).

**Video showcasing compression/decompression**

https://youtu.be/q6NUFBnpKHs

## **Features**

* **Lossless Compression:** Efficiently reduces file size without any loss of data.  
* **Decompression:** Restores compressed files back to their original content.  
* **File Type Compatibility:** Handles .txt and .pdf files by treating them as binary streams.  
* **Command-Line** Interface **(C++):** A standalone executable for direct compression/decompression via terminal commands.  
* **Web-Based User Interface (Python Flask):**  
  * Intuitive file upload form.  
  * One-click compression and decompression.  
  * Direct download of processed files.  
  * Responsive design using Tailwind CSS.

## **Project Structure**

The repository is organized into two main components: the C++ backend and the Python Flask frontend.

your-huffman-project-repo/huffman/

├── backend/                \# Contains all C++ source code for the Huffman algorithm  
│   ├── huffman.cpp         \# Core Huffman compression/decompression logic  
│   ├── huffman.h           \# Header for Huffman class  
│   ├── main.cpp            \# Command-line interface entry point for C++ executable  
│   ├── Node.h              \# Definition for Huffman tree nodes  
│   ├── storage.cpp         \# Handles binary file I/O for compressed data  
│   └── storage.h           \# Header for Storage class  
├── frontend/        \# Contains the Python Flask web application  
│   ├── app.py              \# Main Flask application logic  
│   ├── templates/          \# HTML templates for the web interface  
│   │   ├── index.html      \# File upload form  
│   │   └── download.html   \# Processed file download page  
│   └── (uploads/ and processed/ directories are created at runtime)  
├── .gitignore              \# Specifies files and directories to be ignored by Git  
└── README.md               \# This documentation

## **Prerequisites/How to run on your computer**

* **Git:** For cloning the repository.  
  * [Download](https://git-scm.com/downloads) Git  
* **A C++ Compiler (GCC/G++ recommended):**  
  * **Windows:**  
    * **MinGW-w64:** This provides the GCC compiler suite for Windows.  
    * **Installation Steps:**  
      1. **Download:** Go to [MinGW-w64 SourceForge](https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win64/Personal%20Builds/mingw-builds/8.1.0/threads-posix/seh/) (choose x86\_64-8.1.0-release-posix-seh-rt\_v6-rev0.zip or a newer stable version).  
      2. **Extract:** Create a new folder C:\\MinGW (or a similar short path). Extract the *contents* of the downloaded .zip file directly into this C:\\MinGW folder. This should result in C:\\MinGW\\mingw64\\bin containing g++.exe.  
      3. **Add to PATH:** Add the C:\\MinGW\\mingw64\\bin directory to your system's PATH environment variable.  
         * Search "Environment Variables" in Windows search bar.  
         * Click "Edit the system environment variables".  
         * In "System Properties", click "Environment Variables...".  
         * Under "System variables", select Path and click "Edit...".  
         * Click "New" and add C:\\MinGW\\mingw64\\bin.  
         * Click "OK" on all windows to save.  
      4. **Verify:** Open a **NEW** Command Prompt and type g++ \--version. You should see version information.  
  * **Linux (Ubuntu/Debian-based):**  
    sudo apt update  
    sudo apt install build-essential

  * **macOS:**  
    * Install Xcode Command Line Tools:  
      xcode-select \--install

* **Python 3:**  
  * [Download Python](https://www.python.org/downloads/) (Choose the latest Python 3 version for your OS).  
  * **Windows Specific:** During installation, **crucially check the box "Add Python.exe to PATH"** on the very first installer screen.  
  * **Verify:** Open a **NEW** Command Prompt/Terminal and type python \--version (or python3 \--version on Linux/macOS). You should see Python 3.x.x.  
* **Flask:** The Python web framework.  
  * Install using pip:  
    pip install Flask

## **Setup and Running Instructions**

### **1\. Clone the Repository**

Open your terminal or Command Prompt and clone the project repository:

git clone https://github.com/YOUR\_GITHUB\_USERNAME/your-huffman-project-repo.git  
cd your-huffman-project-repo

### **2\. Compile the C++ Backend**

Go to huffman (backend) directory and compile the C++ source files using g++.

cd huffman  
g++ \-o huffman\_app huffman.cpp storage.cpp main.cpp \-std=c++11 \-Wno-narrowing \-g

* **Expected Outcome:** This command will compile your C++ source files and create an executable named huffman\_app.exe (on Windows) or huffman\_app (on Linux/macOS) within the huffman directory.

### **3\. Run the Python Flask Frontend**

Navigate to the huffman\_web\_app (frontend) directory and start the Flask application.

cd ../huffman\_web\_app \# Go up one level (to the repo root), then into huffman\_web\_app  
python app.py

* **Expected Output:** The Flask application will start, and you will see messages in your terminal indicating that the development server is running, typically on http://127.0.0.1:5000/.

### **4\. Access the Website**

Once the Flask server is running, open your web browser and go to this address:

http://127.0.0.1:5000/

You can now upload .txt or .pdf files to compress or decompress them.



