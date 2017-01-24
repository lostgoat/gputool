/*
 * Copyright (C) 2017  Andres Rodriguez
 *
 * This file is part of gputool.
 *
 * gputool is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * gputool is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with gputool.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <gputool/config.h>
#include <stdio.h>
#include <util/util.h>
#include "GpuToolUi.h"

int main(int argc, char *argv[])
{
    int r = 0;
    std::string command = "";

    try {
        auto ui = util::make_unique<gputool::GpuToolUi>();

        if (argc > 1) {
            for (int i = 1; i < argc; ++i) {
                command += argv[i];
                command += " ";
            }
            r = ui->runSingle(command);
        } else {
            r = ui->run();
        }
    } catch (...) {
        std::terminate();
    }

    return r;
}
