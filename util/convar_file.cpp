/* SPDX-License-Identifier: MIT
 *
 * SPDX-FileCopyrightText: Copyright (c) 2024 Ian Hangartner <icrashstuff at outlook dot com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
#include "convar_file.h"
#include "convar.h"

#include "tetra/gui/console.h"

#include "physfs.h"

#define CFG_NEWLINE "\n"
#define CFG_NEWLINE_LEN 1
#define CFG_MAX_FILE_SIZE (128 * 1024)

static convar_string_t user_config_path("user_config_path", "/user_cfg.txt", "PHYSFS path to user config", CONVAR_FLAG_HIDDEN, convar_file_parser::read);

bool convar_file_parser::write()
{
    PHYSFS_file* fd = PHYSFS_openWrite(user_config_path.get().c_str());
    if (!fd)
    {
        dc_log_warn("Unable to write user config to: \"%s\"", user_config_path.get().c_str());
        return false;
    }

    const std::string cfg_header = "# This file is automatically generated by mcs_b181 Tetra, be careful editing\n";

    PHYSFS_writeBytes(fd, cfg_header.c_str(), cfg_header.length());

    std::vector<convar_t*>* list = convar_t::get_convar_list();
    for (size_t i = 0; i < list->size(); i++)
    {
        convar_t* cvr = list->at(i);
        if (!(cvr->get_convar_flags() & CONVAR_FLAG_SAVE))
            continue;
        switch (cvr->get_convar_type())
        {
        case convar_t::CONVAR_TYPE::CONVAR_TYPE_INT:
        {
            convar_int_t* cvr_int = (convar_int_t*)cvr;
            if (cvr_int->get() == cvr_int->get_default())
                continue;
            std::string cmd = cvr->get_convar_command();
            PHYSFS_writeBytes(fd, cmd.c_str(), cmd.length());
            PHYSFS_writeBytes(fd, CFG_NEWLINE, CFG_NEWLINE_LEN);
            break;
        }
        case convar_t::CONVAR_TYPE::CONVAR_TYPE_FLOAT:
        {
            convar_float_t* cvr_float = (convar_float_t*)cvr;
            if (cvr_float->get() == cvr_float->get_default())
                continue;
            std::string cmd = cvr->get_convar_command();
            PHYSFS_writeBytes(fd, cmd.c_str(), cmd.length());
            PHYSFS_writeBytes(fd, CFG_NEWLINE, CFG_NEWLINE_LEN);
            break;
        }
        case convar_t::CONVAR_TYPE::CONVAR_TYPE_STRING:
        {
            convar_string_t* cvr_string = (convar_string_t*)cvr;
            if (cvr_string->get() == cvr_string->get_default())
                continue;
            std::string cmd = cvr->get_convar_command();
            PHYSFS_writeBytes(fd, cmd.c_str(), cmd.length());
            PHYSFS_writeBytes(fd, CFG_NEWLINE, CFG_NEWLINE_LEN);
            break;
        }
        default:
            break;
        }
    }

    PHYSFS_close(fd);

    return true;
}

void convar_file_parser::read()
{
    PHYSFS_file* fd = PHYSFS_openRead(user_config_path.get().c_str());
    if (!fd)
        return;

    std::vector<Uint8> data;
    data.resize(CFG_MAX_FILE_SIZE);
    Sint64 bytes_read = PHYSFS_readBytes(fd, data.data(), data.size());

    char* line_start = (char*)data.data();
    for (Sint64 i = 0; i < bytes_read; i++)
    {
        char c = data[i];
        if (c == '\n' || c == '\r')
        {
            data[i] = '\0';
            if (line_start && line_start[0] != '#')
                dev_console::run_command("%s", line_start);
            line_start = (char*)data.data() + i + 1;
        }
    }
    if (line_start && line_start[0] != '#')
        dev_console::run_command("%s", line_start);

    PHYSFS_close(fd);
}
