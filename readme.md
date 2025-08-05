# 🖥️ Basic HTTP Server in C

This project implements a **simple HTTP web server in C** using low-level socket programming. The server can:

- Handle HTTP `GET` requests
- Serve static files like `.html`, `.txt`, etc.
- Return a default HTML page when accessing the root (`/`)
- Send a `404 Not Found` error for missing files

---

## 📂 Project Structure


├── server.c # The main server implementation in C

├── README.md # This file

└── [your static files like index.html, hello.txt, etc.]


---

## ⚙️ Features

- 🧠 Parses HTTP request lines (`GET /filename HTTP/1.1`)
- 📁 Serves files from the local directory (same as `server.c`)
- 📄 Returns basic 404 HTML page when file doesn't exist
- 🔁 Handles multiple client requests in a loop
- ✅ Clean and simple structure, perfect for learning purposes

---

## 🏗️ How It Works

1. **Socket Setup**:
   - Creates a TCP socket using `socket(AF_INET, SOCK_STREAM, 0)`
   - Binds it to port `8080` and `INADDR_ANY` to accept connections from any IP
   - Starts listening using `listen()`

2. **Accepting Requests**:
   - Waits for client connections using `accept()`
   - Reads HTTP request with `recv()`
   - Parses the request line with `strtok()` to extract method, path, and HTTP version

3. **Serving Content**:
   - If the request is for `/`, sends a hardcoded HTML response
   - If a file is requested (e.g., `/file.txt`), it:
     - Removes the leading `/`
     - Tries to `open()` the file
     - Uses `sendfile()` to stream file content directly to client
   - If the file doesn't exist, responds with a basic 404 HTML page

4. **Looping**:
   - Repeats the above for every new client connection
   - Keeps the server running until manually stopped

---

## 🚀 Running the Server

### 🧱 Prerequisites

- GCC or any C compiler
- Linux-based system (uses `sendfile()` and POSIX headers)

### 🛠️ Compile

```bash
gcc server.c -o server
./server
You’ll see:

Waiting for connection.........
Now, visit http://localhost:8080 in your browser.

🌐 Example URLs
http://localhost:8080/ → returns hardcoded HTML

http://localhost:8080/hello.txt → tries to serve hello.txt from the same directory

http://localhost:8080/missing.html → returns a 404 page

🧪 Testing
To test different scenarios:

Place files like hello.txt, index.html, or image.jpg in the same folder as the server

Access them from the browser using http://localhost:8080/filename

📌 Notes
Server handles only GET requests. Other HTTP methods are ignored.

No MIME type detection—currently assumes HTML (can be extended).

Designed for educational purposes. Not production-ready.

📚 Learnings
This project helped understand:

Low-level networking in C

HTTP protocol basics

Socket functions: socket(), bind(), listen(), accept(), recv(), sendfile()

File handling and error management

🧑‍💻 Author
Made with ❤️ by [Your Name]

Feel free to fork, improve, or use this as a base for your own webserver projects.
