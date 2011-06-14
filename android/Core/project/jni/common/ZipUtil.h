#ifndef _ZIP_UTIL_H_
#define _ZIP_UTIL_H_
#include <zip.h>
#include "log.h"


/**
 * 根据路径读取apk文件
 */
zip* loadAPK (const char* apkPath) {
  LOGI("AndroidGL","Loading APK %s", apkPath);
  zip* APKArchive = zip_open(apkPath, 0, NULL);
  if (APKArchive == NULL) {
    LOGE("AndroidGL","Error loading APK",0);
    return NULL;
  }

  //Just for debug, print APK contents
  int numFiles = zip_get_num_files(APKArchive);
  for (int i=0; i<numFiles; i++) {
    const char* name = zip_get_name(APKArchive, i, 0);
    if (name == NULL) {
      LOGE("AndroidGL","Error reading zip file name at index %i : %s", zip_strerror(APKArchive));
      return NULL;
    }
    LOGI("AndroidGL","File %i : %s\n", i, name);
  }

  return APKArchive;
}

#endif
