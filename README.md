# Tabula

Tabula is a distraction-free writing application for GNOME, built using GTK. It is designed to be a simple, lightweight, and customizable virtual writer's desk for medium- and long-form works.

Aesthetically, like other minimalist writing applications, it aims to offer a blank slate, a _tabula rasa_, for the author's creativity to flourish. Where really great contemporary text editors like FocusWriter, Typora, and Ghostwriter focus on the [KISS principle](https://en.wikipedia.org/wiki/KISS_principle) to reduce distraction, Tabula focuses on enabling the writer to create a bespoke editing environment tailored for their personal workflow.

## Features

- Full-screen mode with the text editing area in the center.
- Customizable CSS themes.
- Auto-save with a "Save As" option for branching files.
- Basic text editing features: copy, paste, cut, undo, redo, find, and replace.
- Advanced text editing features: multiline editing, bracket matching, and page and bookmark navigation.
- Plugin support for third-party extensions, such as spell checkers, text formatting, and syntax highlighters. Includes an example integration with LanguageTool AI.

## Installation

To install Tabula, run the following command:

```sh
sudo apt-get install tabula
```

Then, run `tabula` from the command line.

## Building from Source

### Prerequisites

- GTK 3
- gcc
- pkg-config
- cmocka (for unit testing)

### Installing Dependencies

#### Debian/Ubuntu

```sh
sudo apt-get update  
sudo apt-get install -y gcc make pkg-config libgtk-3-dev cmocka
```

#### Manually Installing CMocka

If CMocka is not available in your package manager, you can install it manually:

```sh
sudo apt-get update  
sudo apt-get install -y wget cmake  
wget https://cmocka.org/files/1.1/cmocka-1.1.5.tar.xz  
tar -xf cmocka-1.1.5.tar.xz  
cd cmocka-1.1.5  
mkdir build  
cd build  
cmake ..  
make  
sudo make install  
sudo ldconfig
```

If you are installing CMocka for the first time, add the library path to the system’s library search path:

```sh
echo "/usr/local/lib" | sudo tee /etc/ld.so.conf.d/cmocka.conf  
sudo ldconfig
```

### Build

To build the project, run:

```sh
make
```

### Running Tests

To run the tests, execute:

```sh
make test
```

### Running the Application

To run the application, use:

```sh
make run
```

To run the application in debug mode, use:

```sh
make debug
```

### Cleaning Up

To clean up build artifacts, run:

```sh
make clean
```

## Contributing

We welcome contributions! Please fork the repository and submit pull requests.

1. Fork the repository.
2. Create a new branch: `git checkout -b my-branch-name`.
3. Make your changes and commit them: `git commit -m 'Add some feature'`.
4. Push to the branch: `git push origin my-branch-name`.
5. Submit a pull request.

### Code Style

Please follow the existing code style and conventions.

### Reporting Issues

If you encounter any issues, please create an issue in the GitHub repository.
