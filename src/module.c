/*-
 * Copyright (c) 2016, Jason Waataja
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from this
 * software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <assert.h>
#include <err.h>
#include <stdlib.h>
#include <string.h>

#include "module.h"

/*
 * Initializes a struct module_info that's already been allocated, usually on
 * the heap.
 */
void
module_info_init(struct module_info *info)
{
	
	assert(info != NULL);

	LIST_INIT(&info->packages);
	LIST_INIT(&info->files);
	LIST_INIT(&info->cmd);
}

/*
 * Cleans up a struct module_info that has been allocated with
 * module_info_init. It also goes through the lists and deletes them.
 */
void
module_info_destroy(struct module_info *info)
{
	struct str_list_head *packages;
	struct install_file_list_head *files;
	struct module_cmd_list_head *commands;

	assert(info != NULL);

	packages = &info->packages;
	files = &info->files;
	commands = &info->cmd;

	free_str_list(packages);
	free_install_file_list(files);
	free_module_cmd_list(commands);
}

/* Free a string list. */
void
free_str_list(struct str_list_head *list)
{
	struct str_list *packages_entry;

	while (!LIST_EMPTY(list)) {
		packages_entry = LIST_FIRST(list);
		free(packages_entry->data);
		LIST_REMOVE(packages_entry, entry);
		free(packages_entry);
	}
}

/* Free a install file list. */
void
free_install_file_list(struct install_file_list_head *list)
{
	struct install_file_list *files_entry;

	while (!LIST_EMPTY(list)) {
		files_entry = LIST_FIRST(list);
		free(files_entry->file_name);
		free(files_entry->install_location);
		LIST_REMOVE(files_entry, entry);
		free(files_entry);
	}
}

/* Free a command list. */
void
free_module_cmd_list(struct module_cmd_list_head *list)
{
	struct module_cmd_list *cmd_entry;

	while (!LIST_EMPTY(list)) {
		cmd_entry = LIST_FIRST(list);
		free(cmd_entry->cmd.message);
		free(cmd_entry->cmd.shell_cmd);
		LIST_REMOVE(cmd_entry, entry);
		free(cmd_entry);
	}
}

/* Add a string to the given list. Copies the string to the new location. */
void
str_list_add(struct str_list_head *list, const char *str)
{
	struct str_list *entry;

	entry = malloc(sizeof(struct str_list));
	if (entry == NULL)
		err(1, NULL);
	if (str != NULL) {
		entry->data = strdup(str);
		if (entry->data == NULL)
			err(1, NULL);
	} else
		warnx("Appending NULL string to list");

	LIST_INSERT_HEAD(list, entry, entry);
}

/*
 * Add a new file to be installed with the given file name and install
 * location. Copies the strings and allocates memory for them.
 */
void
install_file_list_add(struct install_file_list_head *list, const char
    *file_name, const char *install_location)
{
	struct install_file_list *entry;

	entry = malloc(sizeof(struct install_file_list));
	if (entry == NULL)
		err(1, NULL);

	if (file_name != NULL) {
		entry->file_name = strdup(file_name);
		if (entry->file_name == NULL)
			err(1, NULL);
	} else
		warnx("Appending NULL file name");

	if (install_location != NULL) {
		entry->install_location = strdup(install_location);
		if (entry->file_name == NULL)
			err(1, NULL);
	} else
		warnx("Appending NULL install location");

}

/*
 * Add a new module command to the list. It allocates a new module and sets it
 * to the given type ans message or shell command.
 */
void
module_cmd_list_add(struct module_cmd_list_head *list, enum module_cmd_type
    type, const char *message, const char *shell_cmd)
{
	struct module_cmd_list *entry;

	entry = malloc(sizeof(struct module_cmd_list));
	if (entry == NULL)
		err(1, NULL);

	entry->cmd.type = type;

	if (message != NULL) {
		entry->cmd.message = strdup(message);
		if (entry->cmd.message == NULL)
			err(1, NULL);
	}

	if (shell_cmd != NULL) {
		entry->cmd.shell_cmd = strdup(message);
		if (entry->cmd.shell_cmd == NULL)
			err(1, NULL);
	}
}
