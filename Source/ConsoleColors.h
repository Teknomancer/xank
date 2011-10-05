/** @file
 * xank - Console Colors, header.
 */

/*
 * Copyright (C) 2011 Ramshankar (aka Teknomancer)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef XANK_CONSOLE_COLORS_H
# define XANK_CONSOLE_COLORS_H

/** @todo  make this runtime toggle-able */

# define CIOCOLOR_RESET           "\033[0m"
# define CIOCOLOR_BLACK           "\033[0;30m"
# define CIOCOLOR_RED             "\033[0;31m"
# define CIOCOLOR_GREEN           "\033[0;32m"
# define CIOCOLOR_YELLOW          "\033[0;33m"
# define CIOCOLOR_BLUE            "\033[0;34m"
# define CIOCOLOR_PURPLE          "\033[0;35m"
# define CIOCOLOR_CYAN            "\033[0;36m"
# define CIOCOLOR_WHITE           "\033[0;37m"
# define CIOCOLOR_BOLD_BLACK      "\033[1;30m"
# define CIOCOLOR_BOLD_RED        "\033[1;31m"
# define CIOCOLOR_BOLD_GREEN      "\033[1;32m"
# define CIOCOLOR_BOLD_YELLOW     "\033[1;33m"
# define CIOCOLOR_BOLD_BLUE       "\033[1;34m"
# define CIOCOLOR_BOLD_PURPLE     "\033[1;35m"
# define CIOCOLOR_BOLD_CYAN       "\033[1;36m"
# define CIOCOLOR_BOLD_WHITE      "\033[1;37m"

/*
 '\033[0;31m' # Red
 '\033[0;32m' # Green
 '\033[0;33m' # Yellow
 '\033[0;34m' # Blue
 '\033[0;35m' # Purple
 '\033[0;36m' # Cyan
 '\033[0;37m' # White
 '\033[1;30m' # Black - Bold
 '\033[1;31m' # Red
 '\033[1;32m' # Green
 '\033[1;33m' # Yellow
 '\033[1;34m' # Blue
 '\033[1;35m' # Purple
 '\033[1;36m' # Cyan
 '\033[1;37m' # White
 '\033[4;30m' # Black - Underline
 '\033[4;31m' # Red
 '\033[4;32m' # Green
 '\033[4;33m' # Yellow
 '\033[4;34m' # Blue
 '\033[4;35m' # Purple
 '\033[4;36m' # Cyan
 '\033[4;37m' # White
 '\033[40m'   # Black - Background
 '\033[41m'   # Red
 '\033[42m'   # Green
 '\033[43m'   # Yellow
 '\033[44m'   # Blue
 '\033[45m'   # Purple
 '\033[46m'   # Cyan
 '\033[47m'   # White
 '\033[0m'    # Text Reset
*/

#endif /* XANK_CONSOLE_COLORS_H */

