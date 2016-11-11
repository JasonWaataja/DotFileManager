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

#ifndef _MODULE_H_
#define _MODULE_H_

#include <sys/queue.h>

struct str_list {
	char	*data;
	LIST_ENTRY(str_list) entry;
};

struct install_file_list {
	/* This is just one name or relative path. */
	char	*file_name;
	/* This is a directory, probably. */
	char	*install_location;
	LIST_ENTRY(install_file_list) entry;
};
enum module_cmd_type {
	DFM_PRINT,
	DFM_SHELL
};

struct module_cmd {
	enum module_cmd_type type;
	char	*message;
	char	*shell_cmd;
};


struct module_cmd_list {
	struct module_cmd cmd;
	LIST_ENTRY(module_cmd_list) entry;
};

struct module_info {
	/* The list of packages that need to be installed by the user. */
	LIST_HEAD(str_list_head, str_list) packages;
	/* The list of files with their name and install location. */
	LIST_HEAD(install_file_list_head, install_file_list) files;
	/* The list of module cmd to carry out. */
	LIST_HEAD(module_cmd_list_head, module_cmd_list) cmd;
};

void	 str_list_add(struct str_list_head *list, const char *str);
void	 free_str_list(struct str_list_head *list);
void	 install_file_list_add(struct install_file_list_head *list, const char
    *file_name, const char *install_location);
void	 free_install_file_list(struct install_file_list_head *list);
void	 module_cmd_list_add(struct module_cmd_list_head *list, enum
    module_cmd_type type, const char *message, const char *shell_cmd);
void	 free_module_cmd_list(struct module_cmd_list_head *list);
void	 module_info_init(struct module_info *info);
void	 module_info_destroy(struct module_info *info);

#endif /* !_MODULE_H_ */
