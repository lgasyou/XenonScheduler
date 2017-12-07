"""
    Copyright 2017 Li Zeqing

    This file is part of File Tools.

    File Tools is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    File Tools is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with File Tools. If not, see <http://www.gnu.org/licenses/>.
"""

import os


class LineCounter:
    DEFAULT_FILE_EXTENSIONS = [".cpp", ".c", ".h", ".hpp", ".py", ".java"]
    total_line_count = 0
    file_count = 0

    def __init__(self):
        self.selected_file_extensions = self.DEFAULT_FILE_EXTENSIONS
        self.filters = ''.split()

    def run(self):
        print("{:^8}| {}".format("Line(s)", "Absolute Path"))
        for root, dirs, files in os.walk("../Sources"):
            for file_name in files:
                self.total_line_count += self._process_each_file(root, file_name)
        print("Summary: {} line(s) of {} file(s)".format(self.total_line_count, self.file_count))

    def _count_single_file_lines(self, absolute_path):
        # this program only cares about '\n', so it's no need to consider errors of converting.
        with open(absolute_path, 'r', encoding="utf-8", errors="ignore") as file:
            line_count = 0
            while file.readline():
                line_count += 1
            return line_count

    def _process_each_file(self, root, file_name):
        line_count = 0
        absolute_path = os.path.join(root, file_name)
        file_extension = os.path.splitext(absolute_path)[1]

        if file_extension in self.selected_file_extensions:
            for selector in self.filters:
                if selector in file_name:
                    break
            else:
                line_count = self._count_single_file_lines(absolute_path)
                print("{:^8}| {}".format(line_count, absolute_path))
                self.file_count += 1
        return line_count


if __name__ == '__main__':
    LineCounter().run()
    input()