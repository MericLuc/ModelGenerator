/*!
 * @file   version_util.h
 * @brief  Contains the methods to get the library versions
 * @author lhm
 * @date   17/07/2019
 */

#ifndef VERSION_UTIL_MODELGENERATOR
#define VERSION_UTIL_MODELGENERATOR

#include <string>

#include "version.h"

namespace ModGen {

std::string getVersionBranch(void) { return std::string(GIT_BRANCH_MODELGENERATOR); 	 }
std::string getVersionCommit(void) { return std::string(GIT_COMMIT_HASH_MODELGENERATOR); }
std::string getVersion(void)	   { return std::string(GIT_BRANCH_MODELGENERATOR) 	 +
											std::string("-")			 			 +
											std::string(GIT_COMMIT_HASH_MODELGENERATOR); }
} // namespace ModGen

#endif // VERSION_UTIL_MODELGENERATOR
