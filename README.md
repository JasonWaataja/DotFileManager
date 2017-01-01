# DotFileManager
A program for managing your dot files

## Synopsis
DFM is a program that manages configuration files which are meant to be synced
with another service. What makes DFM special is that it copies files without
using symlinks, meaning that you can change them once you copy them into their
locations. DFM's man feauter, though, is that it groups files and scripts into
modules, or groups of information about files. Using a simple configuration
file, you can quickly set up a new system with all your configuration files and
automatically run the necessary scripts to install, uninstall, and update them.
Another special feature is that DFM can check if files in your syncing directory
are different from the installed versions and update them for you.
## Installation
DFM only compiles on POSIX platforms, or optionally on Cygwin or MSYS. Because
it uses POSIX, though, it requires no dependencies outside of the system's libc.
However, DFM does rely on some features in C++11, namely the regex and smart
pointers features. This, sadly, rules out OpenBSD 6.0, I believe.

DFM is compiled using CMake and supports compiling in-source if desired.
Navigate into the build directory and run `cmake /path/to/dfm`. Then, assuming
you are using the Unix Makefiles generator, run `make install clean`, which may
require root permissions.

When testing on FreeBSD, I noticed that the `man` command on FreeBSD doesn't
search /usr/local/shar/man on 12.0-CURRENT. I may be missing something, and I
could change it to install to /usr/share/man, but it makes more sense to me to
install to `CMAKE_INSTALL_PREFIX`. To get the man pages in the default search
path on FreeBSD, run `cp man/dfm.1 /usr/share/man/man1`, or run cmake with
`-DCMAKE_INSTALL_PREFIX=/usr`.
## Usage
DFM's man page can be consulted for basic options. DFM requires on operation,
which can be install, uninstall, or update, which operate on modules. These are
activated with the `-i`, `-u`, or `-c` flags respectively. In addition to that,
DFM can also generate config files using the `-g` or `-G` options. These gather
a list of files and writes a basic config file with them.

DFM operates on a directory, which can be specified with the `-d` option, and
defaults to the current directory. This means you can specify the directory or
change into the directory to run the command. DFM then attempts to open a file
named "config.dfm" in the directory. Specifying an operation operates on the
modules in the config file.
## Config File
### Modules
The file called "config.dfm", contains sections called modules. To begin a
section, create a line with no indentation with the name of the module and a
colon. Then there is a list of files, and various sections corresponding to
actions to be taken when the module is installed, uninstalled, or updated.
### Module Files
After the module name, you may specify a list of files that the module contains.
These lines have one level of indentation, and contain one to four parts,
separated by whitespace. If one token is specified, it is taken as a file in the
operating directory that is to be installed into the user's home directory. If
two arguments are specified, then the second is taken to be the directory to
install the file to. If three are specified, then the second is a directory to
find the file to copy in and the third is then path of the directory to copy it
into. If four arguments are specified, then the third is the name to install the
file as, and the fourth is the directory to install into. Files specified in
this section are installed when the module is installed, uninstalled when the
module is uninstalled, and checked for updates when the module is updated.

Example:

	vim:
		.vimrc
		c.vim ~/.vim/after/ftplugin
### Install, Uninstall, and Update Sections
After the list of files, there are more optional sections which specify specific
actions for installing, uninstalling, and updating modules. To begin one of
these sections, create a line that starts with "install", "uninstall", or
"update", and then a colon. These lines have no indentation. After this, a list
of commands specify what to do during those actions.

Example:

	my-module:
		install:
		message "Hello World"
	update:
		message "This is messaged when updating."
### Commands
Commands are available in the install, uninstall, and update sections. Lines
specifying a command start with one level of indentation and then the the name
of the command. It can then be followed by arguments. Arguments may be quoted.
Quoted arguments don't have any escape sequences except for \", which creates
allows for using quote characters inside of quotations.

Most commands have aliases that can be used to call the command. For example,
the message command may be called with `msg" or "m`.

The most important commands are the `message" command and "shell` commands. The
message command takes one argument and prints it to standard output. The shell
command takes the rest of the line and runs it as a shell command. In addition
to specifying a command on the same line as the `shell` command, the shell
command also starts a special mode that allows for extra shell commands to be
run in the same shell. Any lines directly following a `shell` command and have
at least two levels of indentation are passed to the same shell that is started
with the command, which allows for creating install scripts.

Example:

	my-modle:
		install:
		message "This message is printed when installing."
	uninstall:
		sh echo "This shell command is executed when uninstalling."
			echo "This command is run as well"
		message "This is another message that is printed."
## Summary of Commands
* `message [message-text]`: Print message-text to the user. Can also be called
  with `msg`, `echo`, or `m`.
* `dependencies [dependencies]`: A list of packages that are required for the
  module to work. This is means to be used in an install section because when
using the `--interactive` flags, it prompts the user for a comand to install the
packages for the modules. Fox example, the vim module might have a command
`dependencies vim python`, and if DFM was called interactively, then it would
prompt the user for a shell command such as `sudo pacman -S vim python` which
would then be executed. This command can also be called with `depend` or `dep`.
* `shell` [optional-command]: The `shell` command is not processed like others
  in that it interprets the remainder of the line as a series of whitespace
separated arguments. The first non-whitespace character after the command to the
end of the line is interpreted as a shell command which is executed. This
initial command is not necessary, though, because any line with two levels of
indentation immediately following this command will also be passed to the shell.
The `shell` command may also be called with `sh`.
* `install [arguments]`: This command is installs files with the arguments
  interpreted the same way as in the files section of a module. May also be
called with `in` or `i`.
* `remove [path]`: Removes the file or directory at the given path. May also be
  called with `rem`, `rm`, `delete` or `uninstall`.

## Examples
Install all modules in the directory "$HOME/git/dotfiles".

```bash
dfm -d ~/git/dotfiles -ia
```

Uninstall the vim and i3 modules from the config file in the current directory.
Print extra information and prompt before deleting any modules or files.

```bash
dfm -uvI vim i3
```

Generate a default config.dfm file in the directory "$HOME/git/dotfiles".

```bash
dfm -d ~/git/dotfiles -g
```
## Example config.dfm
The config.dfm file in my git repository of dotfiles looks something like this:

	vim:
		.vimrc
		c.vim ~/.vim/after/ftplugin
		cpp.vim ~/.vim/after/ftplugin
		python.vim ~/.vim/after/ftplugin
		.ycm_extra_conf.py
	install:
		dep vim git
		sh
			git clone https://github.com/VundleVim/Vundle.vim.git ~/.vim/bundle/Vundle.vim
	emacs:
		.emacs
		.emacs.d
	i3:
		.i3
		.i3status.conf
	install:
		dep i3
		msg "Please reload i3"
	uninstall:
		msg "Please reload i3"
	update:
		msg "Please reload i3"
	redshift:
		.config/redshift.conf
	zsh:
		.zshrc
	install:
		dep zsh curl
		sh
			sh -c "$(curl -fsSL https://raw.githubusercontent.com/robbyrussell/oh-my-zsh/master/tools/install.sh)"
	X:
		.Xresources
