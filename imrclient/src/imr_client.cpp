//
// Created by Alexander Dovzhikov on 3/5/20.
//

#include <imr_client.h>

namespace xlc {

  std::future<bool> imr_client::get_project(Project &project, std::string &name)
  {
      return exec_function([&] {
          client.getProject(project, name);
          return true;
      });
  }

  std::future<bool> imr_client::advance_major_version(std::string& name)
  {
      return exec_function([&] {
          client.advanceMajorVersion(name);
          return true;
      });
  }

}
