
import sys

license_string = """/*
This file is part of Engine3D.

Engine3D is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Engine3D is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Engine3D.  If not, see <https://www.gnu.org/licenses/>.
*/
"""

def process_file(file_path: str):
    print("Processing {}...".format(file_path))
    input_file = open(file_path, "r")
    buffer = input_file.read()
    input_file.close()

    out_file = open(file_path, "w")
    out_file.write(license_string)
    out_file.write(buffer)
    out_file.close()
    print("Done")
    
for i in range(1, len(sys.argv)):
    process_file(sys.argv[i])
