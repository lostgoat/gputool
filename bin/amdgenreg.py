#!/usr/bin/env python

# Copyright (C) 2017 Andres Rodriguez
#
# This file is part of gputool.
#
# Foobar is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# (at your option) any later version.
#
# Foobar is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

import sys
import os

ASICREG_DIR = "/home/andres/git/linux/drivers/gpu/drm/amd/include/asic_reg/"
OUTPUT_DIR = "/home/andres/git/gputool/amdregdb/"

__enable_debug = True
def pdebug (msg):
	if __enable_debug:
		print >> sys.stderr, msg

def writeFile (filename, string):
	pdebug ("Writing: " + filename)
	if not os.path.exists(os.path.dirname(filename)):
		os.makedirs(os.path.dirname(filename))

	with open(filename, "w+") as fileObj:
		fileObj.write(string)


# for file in $(ls */*_sh*.h); do echo "        \"${file%_sh_mask.h}\","; done
SUPPORTED_BLOCKS = [
		"bif/bif_3_0",
        "bif/bif_4_1",
        "bif/bif_5_0",
        "bif/bif_5_1",
        "dce/dce_10_0",
        "dce/dce_11_0",
        "dce/dce_11_2",
        "dce/dce_6_0",
        "dce/dce_8_0",
        "gca/gfx_6_0",
        "gca/gfx_7_2",
        "gca/gfx_8_0",
        "gca/gfx_8_1",
        "gmc/gmc_6_0",
        "gmc/gmc_7_0",
        "gmc/gmc_7_1",
        "gmc/gmc_8_1",
        "gmc/gmc_8_2",
        "oss/oss_1_0",
        "oss/oss_2_0",
        "oss/oss_2_4",
        "oss/oss_3_0_1",
        "oss/oss_3_0",
        "smu/smu_6_0",
        "smu/smu_7_0_0",
        "smu/smu_7_0_1",
        "smu/smu_7_1_0",
        "smu/smu_7_1_1",
        "smu/smu_7_1_2",
        "smu/smu_7_1_3",
        "smu/smu_8_0",
        "uvd/uvd_4_0",
        "uvd/uvd_4_2",
        "uvd/uvd_5_0",
        "uvd/uvd_6_0",
        "vce/vce_1_0",
        "vce/vce_2_0",
        "vce/vce_3_0"
		]

class RegField:
	""" Class to contain register field data """
	def __init__(self):
		self.name = None
		self.mask = None
		self.shift = None

	def __str__(self):
		fieldspec = '{"' + self.name + '", ' + self.mask + ', ' + self.shift + '}'
		return fieldspec

class Register:
	""" Class to contain register data """

	def __init__(self, name, addr):
		self.name = name
		self.addr = addr
		self.fields = {}

	def __str__(self):
		fieldspec = '{\n'
		for k,v in self.fields.iteritems():
			fieldspec += '\t\t' + str(v) + ',\n'
		if self.fields:
			fieldspec = fieldspec[:-2]
		fieldspec += '\n\t}'

		regspec = '\t{"' + self.name + '", ' + self.addr + ', ' + fieldspec + " }"
		return regspec

class GpuBlock:
	""" Stores all registers for a block """
	def __init__(self, name):
		self.pathBase = ASICREG_DIR + name
		self.name = name.replace('/', '_')
		self.registers = {}

	def importRegDefFile(self, filename):
		pdebug("Importing " + filename)
		with open(filename) as fileObj:
			for line in fileObj:
				if "define" in line:
					if "mm" in line or "ix" in line:
						line = " ".join(line.split())
						tokens = line.split(" ");
						name = tokens[1][2:].strip()
						self.registers[name] = Register(name, tokens[2].strip())

	def importRegMaskFile(self, filename):
		pdebug("Importing " + filename)
		with open(filename) as fileObj:
			for line in fileObj:
				if "define" in line and "__" in line:
					line = " ".join(line.split())
					tokens = line.split("__");
					if "__MASK " in line:
						# print tokens
						regName = tokens[0].split(" ")[1].strip()
						fieldName = tokens[1].strip()
						mask = tokens[2].split(" ")[1].strip()

						if regName in self.registers:
							self.registers[regName].fields[fieldName] = RegField()
							self.registers[regName].fields[fieldName].name = fieldName
							self.registers[regName].fields[fieldName].mask = mask
						else:
							pdebug("Warning: dropping field mask " + regName + " : " + fieldName)

					elif "_MASK " in line:
						# print tokens
						regName = tokens[0].split(" ")[1].strip()
						fieldName = tokens[1].split("_MASK ")[0].strip()
						mask = tokens[1].split("_MASK ")[1].strip()

						if regName in self.registers:
							self.registers[regName].fields[fieldName] = RegField()
							self.registers[regName].fields[fieldName].name = fieldName
							self.registers[regName].fields[fieldName].mask = mask
						else:
							pdebug("Warning: dropping field mask " + regName + " : " + fieldName)
					elif "_SHIFT " in line:
						# print tokens
						regName = tokens[0].split(" ")[1].strip()
						fieldName = tokens[1].strip()
						shift = tokens[2].split(" ")[1].strip()

						if regName in self.registers and fieldName in self.registers[regName].fields:
							self.registers[regName].fields[fieldName].shift = shift
						else:
							pdebug("Warning: dropping field shift " + regName + " : " + fieldName)

	def doImport(self):
		self.importRegDefFile(self.pathBase + "_d.h")
		self.importRegMaskFile(self.pathBase + "_sh_mask.h")

		return self

	def __str__(self):
		blockspec = "static const RegSpec " + self.name + "_regs[] = {\n"
		for k,v in self.registers.iteritems():
			blockspec += str(v) + ",\n"
		if self.registers:
			blockspec = blockspec[:-2]
		blockspec += '\n};'
		return blockspec

class FileOutput:
	""" Class to create the relevant output files """
	def __init__(self, name):
		self.originalname = name
		self.name = name.replace('/', '_')
		self.outputBase = OUTPUT_DIR + name
		self.headerFileName = self.outputBase + ".h"
		self.cppFileName = self.outputBase + ".cpp"
		self.block = GpuBlock(name).doImport()

	def writeFiles(self):
		headerFile = "/* AUTOGENERATED FILE, DO NOT EDIT MANUALLY */\n"
		headerFile += '''/*

 *
 * Copyright (C) 2016 Advanced Micro Devices, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

'''
		headerFile += "#pragma once\n"
		headerFile += "\n"
		headerFile += "namespace amdregdb {\n"
		headerFile += "\n"
		headerFile += "void load_" + self.name + "();\n"
		headerFile += "\n"
		headerFile += str(self.block) + "\n"
		headerFile += "\n"
		headerFile += "}; // namespace amdregdb\n"

		cppFile = "/* AUTOGENERATED FILE, DO NOT EDIT MANUALLY */\n"
		cppFile += "#include <amdregdb/AmdRegDb.h>\n"
		cppFile += "#include <util/util.h>\n"
		cppFile += "#include \"" + os.path.basename(self.headerFileName) + "\"\n"
		cppFile += "\n"
		cppFile += "namespace amdregdb {\n"
		cppFile += "\n"
		cppFile += "static const RegBlock regBlock = {\n"
		cppFile += "    .name = \"" + self.name + "\",\n"
		cppFile += "    .regs = " + self.name + "_regs,\n"
		cppFile += "    .size = ARRAY_SIZE(" + self.name + "_regs)\n"
		cppFile += "};\n"
		cppFile += "\n"
		cppFile += "void load_" + self.name + "() {\n"
		cppFile += "    static bool hasLoaded = false;\n"
		cppFile += "    if (hasLoaded) {\n"
		cppFile += "        return;\n"
		cppFile += "    }\n"
		cppFile += "    gRegDb.push_back(&regBlock);\n"
		cppFile += "    hasLoaded = true;\n"
		cppFile += "}\n"
		cppFile += "\n"
		cppFile += "}; //namespace amdregdb\n"

		writeFile(self.headerFileName, headerFile)
		writeFile(self.cppFileName, cppFile)

		return self

def main():
	files = []
	for blockName in SUPPORTED_BLOCKS:
		files.append(FileOutput(blockName).writeFiles())

	regDbHeader = "/* AUTOGENERATED FILE, DO NOT EDIT MANUALLY */\n"
	regDbHeader += "#pragma once\n"
	regDbHeader += "\n"
	regDbHeader += "#include <vector>\n"
	regDbHeader += "#include <util/util.h>\n"
	regDbHeader += "#include <amdregdb/RegSpec.h>\n"
	regDbHeader += "\n"
	regDbHeader += "namespace amdregdb {\n"
	regDbHeader += "\n"

	for f in files:
		regDbHeader += "void load_" + f.name + "();\n"

	regDbHeader += "\n"
	regDbHeader += "extern std::vector<const RegBlock*> gRegDb;\n"
	regDbHeader += "\n"
	regDbHeader += "static inline void load_all_blocks() {\n"

	for f in files:
		regDbHeader += "    load_" + f.name + "();\n"

	regDbHeader += "}\n"
	regDbHeader += "\n"
	regDbHeader += "}; //namespace amdregdb\n"

	regDbHeaderFileName = OUTPUT_DIR + "/include/amdregdb/AmdRegDb.h"
	writeFile(regDbHeaderFileName, regDbHeader)

	regDbCpp = "/* AUTOGENERATED FILE, DO NOT EDIT MANUALLY */\n"
	regDbCpp += "#include <amdregdb/AmdRegDb.h>\n"
	regDbCpp += "\n"
	regDbCpp += "namespace amdregdb {\n"
	regDbCpp += "\n"
	regDbCpp += "std::vector<const RegBlock*> gRegDb;\n"
	regDbCpp += "\n"
	regDbCpp += "}; //namespace amdregdb\n"

	regDbCppFileName = OUTPUT_DIR + "/AmdRegDb.cpp"
	writeFile(regDbCppFileName, regDbCpp)

	regDbCmake = '''# gputool amdregdb/ cmake file
#
# Copyright (C) 2017 Andres Rodriguez
#
# This file is part of gputool.
#
# gputool is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# (at your option) any later version.
#
# gputool is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with gputool.  If not, see <http://www.gnu.org/licenses/>.

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

set(SOURCES
	AmdRegDb.cpp
'''
	for f in files:
		regDbCmake += "    " + f.originalname + ".cpp\n"

	regDbCmake += '''
)

add_library(amdregdb ${SOURCES})

set_property(TARGET amdregdb PROPERTY CXX_STANDARD 11)
set_property(TARGET amdregdb PROPERTY CXX_STANDARD_REQUIRED ON)

install (TARGETS amdregdb DESTINATION lib)
'''
	regDbCmakeFileName = OUTPUT_DIR + "/CMakeLists.txt"
	writeFile(regDbCmakeFileName, regDbCmake)

	return 0

if __name__ == "__main__":
	main()
