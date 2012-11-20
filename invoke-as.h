/*
 * Copyright (C) 2012 The ChameleonOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef INVOKEAS_h 
#define INVOKEAS_h 1

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "invoke-as"

#define VERSION "1.0"

#define DEFAULT_SHELL "/system/bin/sh"

#include <cutils/log.h>
#ifndef LOGE
#define LOGE(...) ALOGE(__VA_ARGS__)
#endif
#ifndef LOGD
#define LOGD(...) ALOGD(__VA_ARGS__)
#endif
#ifndef LOGW
#define LOGW(...) ALOGW(__VA_ARGS__)
#endif

#include <errno.h>
#include <string.h>
#define PLOGE(fmt,args...) LOGE(fmt " failed with %d: %s", ##args, errno, strerror(errno))
#define PLOGEV(fmt,err,args...) LOGE(fmt " failed with %d: %s", ##args, err, strerror(err))

#endif
