# app.py
# This Flask application provides a web interface for the Huffman compressor/decompressor.

import os
import subprocess
from flask import Flask, request, render_template, send_file, redirect, url_for

app = Flask(__name__)

# Configuration for file uploads
UPLOAD_FOLDER = 'uploads'  # Directory to store uploaded files
PROCESSED_FOLDER = 'processed' # Directory to store processed files
# Define the path to your compiled C++ executable relative to this script.
# Assumes 'huffman_app.exe' will be compiled and located in the 'backend' directory.
HUFFMAN_EXECUTABLE = '../backend/huffman_app.exe' # Path from frontend to backend

# Ensure upload and processed folders exist
os.makedirs(UPLOAD_FOLDER, exist_ok=True)
os.makedirs(PROCESSED_FOLDER, exist_ok=True)

@app.route('/', methods=['GET'])
def index():
    """Renders the main upload form page."""
    return render_template('index.html')

@app.route('/process', methods=['POST'])
def process_file():
    """Handles file upload and calls the Huffman executable."""
    if 'file' not in request.files:
        return "No file part", 400

    file = request.files['file']
    if file.filename == '':
        return "No selected file", 400

    if file:
        filename = file.filename
        operation = request.form.get('operation') # 'compress' or 'decompress'

        # Sanitize filename (basic example, more robust needed for production)
        sanitized_filename = os.path.basename(filename)
        input_filepath = os.path.join(UPLOAD_FOLDER, sanitized_filename)

        # Save the uploaded file
        file.save(input_filepath)

        # Determine output filename based on operation
        if operation == 'compress':
            # For compression, add .huf extension
            name_without_ext, ext = os.path.splitext(sanitized_filename)
            output_filename = f"{name_without_ext}.huf"
        elif operation == 'decompress':
            # For decompression, remove .huf or assume original extension
            if sanitized_filename.endswith('.huf'):
                output_filename = sanitized_filename[:-len('.huf')] # Remove .huf
                if not output_filename: # Handle case like ".huf" file
                    output_filename = "decompressed_file"
            else:
                output_filename = f"decompressed_{sanitized_filename}" # Fallback
        else:
            os.remove(input_filepath) # Clean up uploaded file
            return "Invalid operation selected.", 400

        output_filepath = os.path.join(PROCESSED_FOLDER, output_filename)

        # Build the command to execute your C++ program
        # Example: huffman_app.exe compress uploads\input.txt processed\output.huf
        command = [HUFFMAN_EXECUTABLE, operation, input_filepath, output_filepath]

        try:
            # Execute the C++ program
            # capture_output=True to get stdout/stderr, text=True for string output
            result = subprocess.run(command, check=True, capture_output=True, text=True)
            print(f"C++ stdout:\n{result.stdout}")
            if result.stderr:
                print(f"C++ stderr:\n{result.stderr}")

            # Check if output file was created successfully
            if os.path.exists(output_filepath) and os.path.getsize(output_filepath) > 0:
                # Provide a link to download the processed file
                return render_template('download.html', filename=output_filename)
            else:
                return f"Error: Processed file was not created or is empty. Check C++ program output. Stderr: {result.stderr}", 500

        except subprocess.CalledProcessError as e:
            # If the C++ program returns a non-zero exit code
            print(f"Error executing C++ program: {e}")
            print(f"C++ stdout on error:\n{e.stdout}")
            print(f"C++ stderr on error:\n{e.stderr}")
            return f"Error processing file: {e.stderr or e.stdout or e}", 500
        except FileNotFoundError:
            return f"Error: Huffman executable not found at '{HUFFMAN_EXECUTABLE}'. Please ensure it's compiled and the path is correct.", 500
        finally:
            # Clean up the uploaded input file after processing
            if os.path.exists(input_filepath):
                os.remove(input_filepath)

    return "Something went wrong.", 500

@app.route('/download/<filename>', methods=['GET'])
def download_file(filename):
    """Allows users to download the processed file."""
    filepath = os.path.join(PROCESSED_FOLDER, filename)
    if os.path.exists(filepath):
        # Determine mimetype based on filename for better browser handling
        mimetype = 'application/octet-stream' # Default binary stream
        if filename.endswith('.txt'):
            mimetype = 'text/plain'
        elif filename.endswith('.pdf'):
            mimetype = 'application/pdf'
        elif filename.endswith('.huf'): # Custom extension for compressed files
            mimetype = 'application/x-huffman-compressed' # Custom mimetype if desired

        return send_file(filepath, as_attachment=True, download_name=filename, mimetype=mimetype)
    else:
        return "File not found.", 404

if __name__ == '__main__':
    # When running locally, Flask automatically restarts on code changes.
    # For production, use a production-ready WSGI server like Gunicorn or uWSGI.
    app.run(debug=True, port=5000)
